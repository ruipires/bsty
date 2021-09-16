#include "ContaOrdem.h"
#include "util/CodePageToUtf8Converter.h"
#include "util/DateParser.h"

#include <spdlog/spdlog.h>
#include <fast-cpp-csv-parser/csv.h>

#include <fstream>
#include <sstream>

namespace
{
    char const * const kind = "CGD - Conta Ordem";
}

bsty::core::Data cgd::ContaOrdem::loadCsv(std::string const &filename)
{
    bsty::core::Data result;

    spdlog::trace("Loading {} from {}.", kind, filename);

    try
    {
        std::ifstream in_file(filename);

        std::stringstream buffer;
        buffer << in_file.rdbuf();
        auto const fileContents = buffer.str();

        CodePageToUtf8Converter converter(".");
        auto const utf8FileContents = converter.fromCp1252(fileContents);
        spdlog::trace("Converting file contents from cp-1252 to UTF-8 (read {} bytes)->(conversion resulted in {} bytes)",
                      fileContents.size(), utf8FileContents.size());

        std::stringstream utf8Stream(utf8FileContents);
        result = loadCsv(filename, utf8Stream);
    }
    catch (std::exception const& e)
    {
        spdlog::error("Failed loading {} from {}, reason: {}", kind, filename, e.what());
    }
    catch(...)
    {
        spdlog::error("Failed loading {} from {}, due to an unknown exception.", kind, filename);
    }

    return result;
}

bsty::core::Data cgd::ContaOrdem::loadCsv(std::string const& filename, std::istream &in_file)
{
    bsty::core::Data result;
    std::string tmp;

    loadFileHeader(in_file, result);

    io::CSVReader<8, io::trim_chars<' ', '\t'>, io::no_quote_escape<';'>> in(filename, in_file);
    in.read_header(io::ignore_extra_column, "Data mov.", "Data valor", "Descrição", "Débito", "Crédito", "Saldo contabilístico", "Saldo disponível", "Categoria");

    std::string raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category;

    DateParser parser;
    std::string const payee;
    bool processingFileEnding = false;
    while(in.read_row(raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category))
    {
        if(auto date = parser.parse(raw_date_mov))
        {
            if(processingFileEnding)
            {
                // after we start processing the last lines of the file,
                // we should not read any more valid dates.
                // final lines are in a different format.
                spdlog::error("Read valid date after an invalid date (was already in processing file ending mode). line was:\"{}\"", raw_date_mov);
                spdlog::error("parse contents from offending line: {{ date_mov='{}', date_val='{}', desc='{}' outflow='{}', inflow='{}',"
                              " balance='{}', available_balance='{}', category='{}' }}",
                              raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category);
                // mark whole data read as invalid
                result = bsty::core::Data();
                break;
            }
            else
            {
                bsty::core::Date const transactionDate(date->year, date->month, date->day);
                // TODO read currency unit from file
                bsty::core::Money const outflowMoney(outflow, "EUR");
                bsty::core::Money const inflowMoney(inflow, "EUR");

                bsty::core::Row row(transactionDate, payee, desc, outflowMoney, inflowMoney);
                result.add(row);
            }
        }
        else
        {
            if(!processingFileEnding)
            {
                // at first invalid date, enter processing file ending mode
                spdlog::trace("Entered processing file ending mode");
                processingFileEnding = true;
            }
        }

        if(processingFileEnding)
        {
            spdlog::trace("file ending line: {{ date_mov='{}', date_val='{}', desc='{}' outflow='{}', inflow='{}',"
                          " balance='{}', available_balance='{}', category='{}' }}",
                          raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category);
        }
    }

    return result;
}

cgd::ContaOrdem cgd::ContaOrdem::loadFromCsv(std::string const &filename)
{
    return ContaOrdem(loadCsv(filename));
}

bool cgd::ContaOrdem::loadFileHeader(std::istream &inFile, bsty::core::Data &data)
{
    bool success = true;

    std::string line;
    for(int lineNumber = 0; lineNumber != 6 && success; ++lineNumber)
    {
        std::getline(inFile, line);

        success = success && inFile;

        switch(lineNumber)
        {
            case 0:
            {
                auto value = util::parseCsvHeaderLineStatementDate(line);
                if(value)
                {
                    data.setReportDate(*value);
                }

                break;
            }
            case 2:
            {
                auto value = util::parseCsvHeaderLineAccountNumberAndDescription(line);
                if(value)
                {
                    data.setAccountCode(value->first);
                    data.setAccountDescription(value->second);
                }
                break;
            }
            case 3:
            {
                auto value = util::parseCsvHeaderLineBeginningDate(line);
                if(value)
                {
                    data.setBeginDate(*value);
                }
                break;
            }
            case 4:
            {
                auto value = util::parseCsvHeaderLineBeginningDate(line);
                if(value)
                {
                    data.setEndDate(*value);
                }
                break;
            }
        }
    }


    return success;
}

bsty::core::Data const &cgd::ContaOrdem::getData() const
{
    return data;
}

cgd::ContaOrdem::ContaOrdem(bsty::core::Data data)
    :data(std::move(data))
{
}

std::optional<bsty::core::Date> cgd::util::parseCsvHeaderLineStatementDate(std::string_view sv)
{
    std::optional<bsty::core::Date> result;

    auto pos = sv.find('-');

    if(pos != std::string::npos && (pos + 1 != sv.size()))
    {
        auto const contents = sv.substr(pos+1);
        auto const raw_date = cgd::util::trim(contents);
        DateParser parser;
        if(auto date = parser.parse(raw_date))
        {
            bsty::core::Date const convertedDate(date->year, date->month, date->day);
            result = convertedDate;
        }
    }

    return result;
}

std::optional<std::pair<std::string, std::string>>
cgd::util::parseCsvHeaderLineAccountNumberAndDescription(std::string_view sv)
{
    std::optional<std::pair<std::string, std::string>> result;

    auto pos = sv.find(';');
    auto column1 = sv.substr(pos);
    column1.remove_prefix(1); // remove ;

    auto field1End = column1.find('-');
    auto field1 = column1.substr(0, field1End);
    field1 = cgd::util::trim(field1);

    auto remaining = column1.substr(field1End);
    remaining.remove_prefix(1); // remove -
    auto lastFieldPos = remaining.find('-');
    auto lastField = remaining.substr(lastFieldPos);
    lastField.remove_prefix(1);
    lastField = cgd::util::trim(lastField);

    result = std::make_pair(field1, lastField);

    return result;
}


std::optional<bsty::core::Date> cgd::util::parseCsvHeaderLineBeginningDate(std::string_view sv)
{
    std::optional<bsty::core::Date> result;

    auto pos = sv.find(';');

    if(pos != std::string::npos)
    {
        auto contents = sv.substr(pos);
        contents.remove_prefix(1);
        auto const raw_date = cgd::util::trim(contents);

        DateParser parser;
        if(auto date = parser.parse(raw_date))
        {
            bsty::core::Date const convertedDate(date->year, date->month, date->day);
            result = convertedDate;
        }
    }

    return result;
}

std::optional<bsty::core::Date> cgd::util::parseCsvHeaderLineEndingDate(std::string_view sv)
{
    // TODO: remove code duplication with other parseCsvHeaderLine* methods
    std::optional<bsty::core::Date> result;

    auto pos = sv.find(';');

    if(pos != std::string::npos)
    {
        auto contents = sv.substr(pos);
        contents.remove_prefix(1);
        auto const raw_date = cgd::util::trim(contents);

        DateParser parser;
        if(auto date = parser.parse(raw_date))
        {
            bsty::core::Date const convertedDate(date->year, date->month, date->day);
            result = convertedDate;
        }
    }

    return result;
}

std::string_view cgd::util::trim_left(std::string_view sv)
{
    auto const pos = std::find_if_not(sv.begin(), sv.end(), [](auto ch){ return std::isspace(ch); });
    auto const count = std::distance(sv.begin(), pos);

    sv.remove_prefix(count);
    return sv;
}

std::string_view cgd::util::trim_right(std::string_view sv)
{
    auto const pos = std::find_if_not(sv.rbegin(), sv.rend(), [](auto ch){ return std::isspace(ch); });
    auto const count = std::distance(sv.rbegin(), pos);

    sv.remove_suffix(count);
    return sv;
}

std::string_view cgd::util::trim(std::string_view sv)
{
    return trim_left(trim_right(sv));
}

bsty::core::Data cgd::ContaOrdemParser::loadFrom(std::string const &filename) const
{
    return ContaOrdem::loadFromCsv(filename).getData();
}