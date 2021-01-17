#include <catch2/catch.hpp>
#include "util/CodePageToUtf8Conterter.h"

TEST_CASE( "cp1252_to_utf_crédito", "CodePageToUtf8Converter" )
{
    CodePageToUtf8Conterter conv;
    std::string const in = "Cr�dito";
    std::string const out = conv.fromCp1252(in);

    REQUIRE( out == "Crédito");
}

TEST_CASE("cp1252_to_utf_descrição", "CodePageToUtf8Converter")
{
    CodePageToUtf8Conterter conv;
    std::string const in = "Descri��o";
    std::string const out = conv.fromCp1252(in);

    REQUIRE( out == "Descrição");
}

