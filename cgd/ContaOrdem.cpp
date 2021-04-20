#include "ContaOrdem.h"
#include <spdlog/spdlog.h>
#include <fast-cpp-csv-parser/csv.h>
#include <fstream>
#include <sstream>
#include "util/CodePageToUtf8Converter.h"

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
        std::getline(in_file, tmp);

    io::CSVReader<8> in(filename, in_file);
    in.read_header(io::ignore_extra_column, "Data mov.", "Data valor", "Descrição", "Débito", "Crédito", "Saldo contabilístico", "Saldo disponível", "Categoria");

    char * raw_date_mov, raw_date_val;
    std::string desc, outflow, inflow, balance, available_balance, category;

    while(in.read_row(raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, category))
    {
        spdlog::trace("* row {{ date='{}', payee='{}', memo='{}' outflow='{}', inflow='{}' }}");
    }
}
