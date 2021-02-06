#include "ContaOrdem.h"
#include <spdlog/spdlog.h>
#include <fast-cpp-csv-parser/csv.h>
#include <fstream>

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

        std::string tmp;
        for(int i = 0; i != 6; ++i)
            std::getline(in_file, tmp);

        io::CSVReader<8> in(filename, in_file);
        in.read_header(io::ignore_extra_column, "Data mov. ", "Data valor", "Descri��o", "D�bito", "Cr�dito", "Saldo contabil�stico", "Saldo dispon�vel", "Categoria");

        char * raw_date_mov, raw_date_val;
        std::string desc, outflow, inflow, balance, available_balance, categoria;

        while(in.read_row(raw_date_mov, raw_date_val, desc, outflow, inflow, balance, available_balance, categoria))
        {
            spdlog::trace("* row {{ date='{}', payee='{}', memo='{}' outflow='{}', inflow='{}' }}");
        }
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

std::string cgd::cleanup_cgd_co_file(std::string const &data)
{
    return std::string();
}
