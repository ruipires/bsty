#include "ContaOrdem.h"
#include "util/CodePageToUtf8Converter.h"

#include <spdlog/spdlog.h>
#include <fast-cpp-csv-parser/csv.h>
#include <date/date.h>

#include <fstream>
#include <sstream>


namespace
{
    char const * const kind = "CGD - Conta Ordem";
}

void cgd::ContaOrdem::load_csv(std::string const &filename)
{
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
        load_csv(filename, utf8Stream);
    }
    catch (std::exception const& e)
    {
        spdlog::error("Failed loading {} from {}, reason: {}", kind, filename, e.what());
    }
    catch(...)
    {
        spdlog::error("Failed loading {} from {}, due to an unknown exception.", kind, filename);
    }
}

void cgd::ContaOrdem::load_csv(std::string const& filename, std::istream &in_file)
{
    std::string tmp;
    for(int i = 0; i != 6; ++i)
    {
        std::getline(in_file, tmp);
        spdlog::trace("Skipping over line #{}, contents:\"{}\"", i, tmp);
    }

    //io::CSVReader<8> in(filename, in_file);
    io::CSVReader<8, io::trim_chars<' ', '\t'>, io::no_quote_escape<';'>> in(filename, in_file);;
    in.read_header(io::ignore_extra_column, "Data mov.", "Data valor", "Descrição", "Débito", "Crédito", "Saldo contabilístico", "Saldo disponível", "Categoria");

    std::string raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category;

    while(in.read_row(raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category))
    {
        spdlog::trace(
                "* row {{ date_mov='{}', date_val='{}', desc='{}' outflow='{}', inflow='{}', balance='{}', available_balance='{}', category='{}' }}",
                raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category);

        // parse dates - https://stackoverflow.com/a/41613816

        std::istringstream date{raw_date_mov};
        date::sys_time<date::days> tp;
        date >> date::parse("%d-%m-%Y", tp);
        date::year_month_day x{tp};
        spdlog::trace("** raw_date={} => year:{} month:{} day:{}", raw_date_mov, int(x.year()), unsigned(x.month()),
                      unsigned(x.day()));
    }
}
