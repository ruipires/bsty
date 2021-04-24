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
    for(int i = 0; i != 6; ++i)
    {
        std::getline(in_file, tmp);
        spdlog::trace("Skipping over line #{}, contents:\"{}\"", i, tmp);
    }

    io::CSVReader<8, io::trim_chars<' ', '\t'>, io::no_quote_escape<';'>> in(filename, in_file);;
    in.read_header(io::ignore_extra_column, "Data mov.", "Data valor", "Descrição", "Débito", "Crédito", "Saldo contabilístico", "Saldo disponível", "Categoria");

    std::string raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category;

    DateParser parser;
    std::string const payee;
    while(in.read_row(raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category))
    {
        spdlog::trace(
                "* row {{ date_mov='{}', date_val='{}', desc='{}' outflow='{}', inflow='{}', balance='{}', available_balance='{}', category='{}' }}",
                raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category);

        if(auto date = parser.parse(raw_date_mov))
        {
            spdlog::trace("** year:{} month:{} day:{}", date->year, date->month, date->day);

            bsty::core::Date const transactionDate(date->year, date->month, date->day);
            // TODO read currency unit from file
            bsty::core::Money const outflowMoney(outflow, "EUR");
            bsty::core::Money const inflowMoney(inflow, "EUR");

            bsty::core::Row row(transactionDate, payee, desc, outflowMoney, inflowMoney);
            result.add(row);
        }
        else
        {
            spdlog::error("!invalid date! \"{}\"", raw_date_mov);
        }
    }

    return result;
}

cgd::ContaOrdem cgd::ContaOrdem::loadFromCsv(std::string const &filename)
{
    return ContaOrdem(loadCsv(filename));
}

bsty::core::Data const &cgd::ContaOrdem::getData() const
{
    return data;
}

cgd::ContaOrdem::ContaOrdem(bsty::core::Data data)
    :data(data)
{
}
