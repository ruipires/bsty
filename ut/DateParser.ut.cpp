#include <catch2/catch.hpp>
#include "util/DateParser.h"

TEST_CASE( "regular date parsing", "DateParser" )
{
    DateParser p;

    auto const d = p.parse("24-07-1982");

    REQUIRE(d);
    CHECK(d->year == 1982);
    CHECK(d->month == 7);
    CHECK(d->day == 24);
}

TEST_CASE( "empty string", "DateParser" )
{
    DateParser p;

    auto const d = p.parse("");

    REQUIRE(!d);
}

TEST_CASE( "failing case", "DateParser" )
{
    DateParser p;

    auto const d = p.parse("1982");

    REQUIRE(!d);
}

TEST_CASE( "custom format", "DateParser" )
{
    DateParser p("%Y-%m-%d");

    auto const d = p.parse("1985-05-19");

    REQUIRE(d);
    CHECK(d->year == 1985);
    CHECK(d->month == 5);
    CHECK(d->day == 19);
}
