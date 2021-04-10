#include <catch2/catch.hpp>
#include "util/CodePageToUtf8Conterter.h"
#include <fstream>
#include <spdlog/spdlog.h>

TEST_CASE( "cp1252_to_utf8", "CodePageToUtf8Converter" )
{
    CodePageToUtf8Conterter conv;

    std::ifstream sample("windows-1252.sample.txt");
    std::string in;
    std::getline(sample, in);

//    std::wstring const out = conv.cp1252ToUtf16(in);
//    std::wstring const expected = L"Data mov. 	Data valor 	Descrição 	Débito 	Crédito 	Saldo contabilístico 	Saldo disponível 	Categoria";
//    spdlog::info("in :{}", in);
//    spdlog::info("out:{}", out);

    std::string const out = conv.fromCp1252(in);
    std::string const expected = "Data mov. 	Data valor 	Descrição 	Débito 	Crédito 	Saldo contabilístico 	Saldo disponível 	Categoria";

    REQUIRE( out == expected);
}

