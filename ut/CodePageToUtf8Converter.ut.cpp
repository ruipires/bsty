#include <catch2/catch.hpp>
#include "util/CodePageToUtf8Converter.h"
#include <fstream>

TEST_CASE( "cp1252_to_utf8", "CodePageToUtf8Converter" )
{
    CodePageToUtf8Converter conv(".");

    std::ifstream sample("windows-1252.sample.txt");
    std::string in;
    std::getline(sample, in);

    std::string const out = conv.fromCp1252(in);
    std::string const expected = "Data mov. 	Data valor 	Descrição 	Débito 	Crédito 	Saldo contabilístico 	Saldo disponível 	Categoria";

    REQUIRE( out == expected);
}

