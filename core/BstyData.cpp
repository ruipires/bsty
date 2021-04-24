#include "BstyData.h"
#include <fmt/core.h>

namespace bsty::core
{
    std::string to_string(bsty::core::Date const &date)
    {
        return fmt::format("{}-{}-{}", date.year, date.month, date.day);
    }

    std::string to_string(Money const &money)
    {
        return money.amount;
    }

    void Data::add(Row row)
    {
        rows.emplace_back(std::move(row));
    }

    std::vector<Row> const& Data::getRows() const
    {
        return rows;
    }

}