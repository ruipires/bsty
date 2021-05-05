#include "BstyData.h"
#include <fmt/core.h>
#include <ostream>

namespace bsty::core
{
    std::string to_string(bsty::core::Date const &date)
    {
        return fmt::format("{}-{:02d}-{:02d}", date.year, date.month, date.day);
    }

    std::ostream& operator<<(std::ostream& out, Date const& date)
    {
        return out << to_string(date);
    }

    std::string to_string(Money const &money)
    {
        return money.amount;
    }

    std::ostream& operator<<(std::ostream& out, Money const& money)
    {
        return out << to_string(money);
    }

    void Data::add(Row row)
    {
        rows.emplace_back(std::move(row));
    }

    std::vector<Row> const& Data::getRows() const
    {
        return rows;
    }

    std::optional<Date> Data::getBeginDate() const
    {
        return beginDate;
    }

    void Data::setBeginDate(Date const& date)
    {
        beginDate = date;
    }

    std::optional<Date> Data::getEndDate() const
    {
        return endDate;
    }

    void Data::setEndDate(Date const& date)
    {
        endDate = date;
    }

    std::optional<Date> Data::getReportDate() const
    {
        return reportDate;
    }

    void Data::setReportDate(Date const& date)
    {
        reportDate = date;
    }

    std::optional<std::string> Data::getAccountCode() const
    {
        return accountCode;
    }

    void Data::setAccountCode(std::string const& code)
    {
        accountCode = code;
    }

    std::optional<std::string> Data::getAccountDescription() const
    {
        return accountDescription;
    }

    void Data::setAccountDescription(std::string const& description)
    {
        accountDescription = description;
    }

    std::optional<Money> Data::getCurrentAccountBalance() const
    {
        return currentAccountBalance;
    }

    void Data::setCurrentAccountBalance(Money const& balance)
    {
        currentAccountBalance = balance;
    }

    std::optional<Money> Data::getCurrentAvailableBalance() const
    {
        return currentAvailableBalance;
    }

    void Data::setCurrentAvailableBalance(Money const& balance)
    {
        currentAvailableBalance = balance;
    }
}