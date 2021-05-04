#include <catch2/catch.hpp>
#include "cgd/ContaOrdem.h"

using std::nullopt;

// TODO: add failing case tests

TEST_CASE( "csv_header_line_1", "ContaOrdem" )
{
    auto const data = "Consultar saldos e movimentos à ordem - 14-10-2020";

    auto const result = cgd::util::parseCsvHeaderLineStatementDate(data);

    REQUIRE(result != nullopt);

    CHECK(result.value() == "14-10-2020");
}

TEST_CASE( "csv_header_line_2", "ContaOrdem" )
{
    auto const data = "Conta ;0012345678910 - EUR - Nossa Conta ";

    auto const result = cgd::util::parseCsvHeaderLineAccountNumberAndDescription(data);

    REQUIRE(result != nullopt);
    CHECK(result.value().first == "0012345678910");
    CHECK(result.value().second == "Nossa Conta");
}

TEST_CASE( "csv_header_line_3", "ContaOrdem" )
{
    auto const data = "Data de início ;01-09-2020";

    auto const result = cgd::util::parseCsvHeaderLineBeginningDate(data);

    REQUIRE(result != nullopt);
    CHECK(result.value() == "01-09-2020");
}

TEST_CASE( "csv_header_line_4", "ContaOrdem" )
{
    auto const data = "Data de fim ;30-09-2020";

    auto const result = cgd::util::parseCsvHeaderLineEndingDate(data);

    REQUIRE(result != nullopt);
    CHECK(result.value() == "30-09-2020");
}

TEST_CASE( "trim_left", "ContaOrdem" )
{
    auto const data = "   text   ";

    auto const result = cgd::util::trim_left(data);

    CHECK(result == "text   ");
}

TEST_CASE( "trim_right", "ContaOrdem" )
{
    auto const data = "   text   ";

    auto const result = cgd::util::trim_right(data);

    CHECK(result == "   text");
}

TEST_CASE( "trim", "ContaOrdem" )
{
    auto const data = "   text   ";

    auto const result = cgd::util::trim(data);

    CHECK(result == "text");
}
