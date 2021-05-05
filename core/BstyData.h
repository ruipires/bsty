#ifndef BSTY_BSTYDATA_H
#define BSTY_BSTYDATA_H

#include <utility>
#include <vector>
#include <string>
#include <optional>
#include <iosfwd>

namespace bsty::core
{
    struct Date
    {
        public:
            inline Date()
                :year(0)
                ,month(0)
                ,day(0)
            {}

            inline explicit Date(int year, unsigned int month, unsigned int day)
                :year(year)
                ,month(month)
                ,day(day)
            {}

            bool operator==(Date const &rhs) const
            {
                return std::tie(year, month, day) == std::tie(rhs.year, rhs.month, rhs.day);
            }

            bool operator!=(Date const &rhs) const
            {
                return !(rhs == *this);
            }

            int year;
            unsigned int month;
            unsigned int day;
    };

    std::string to_string(Date const& date);
    std::ostream& operator<<(std::ostream& out, Date const& date);

    struct Money
    {
        inline explicit Money(std::string amount, std::string currency)
            :amount(std::move(amount))
            ,currency(std::move(currency))
        {}

        inline Money()
            :amount("0")
            ,currency("EUR")
        {}

        std::string amount;
        std::string currency;
    };

    std::string to_string(Money const& money);
    std::ostream& operator<<(std::ostream& out, Money const& money);

    struct Row
    {
        Row() = default;

        inline explicit Row(Date date, std::string payee, std::string memo, Money outflow, Money inflow)
            :date(date)
            ,payee(std::move(payee))
            ,memo(std::move(memo))
            ,outflow(std::move(outflow))
            ,inflow(std::move(inflow))
        {}

        Date const date;
        std::string const payee;
        std::string const memo;
        Money const outflow;
        Money const inflow;
    };

    class Data
    {
        public:
            void add(Row row);

            [[nodiscard]] std::vector<Row> const& getRows() const;

            [[nodiscard]] std::optional<Date> getBeginDate() const;
            void setBeginDate(Date const& date);

            [[nodiscard]] std::optional<Date> getEndDate() const;
            void setEndDate(Date const& date);

            [[nodiscard]] std::optional<Date> getReportDate() const;
            void setReportDate(Date const& date);

            [[nodiscard]] std::optional<std::string> getAccountCode() const;
            void setAccountCode(std::string const& code);

            [[nodiscard]] std::optional<std::string> getAccountDescription() const;
            void setAccountDescription(std::string const& description);

            [[nodiscard]] std::optional<Money> getCurrentAccountBalance() const;
            void setCurrentAccountBalance(Money const& balance);

            [[nodiscard]] std::optional<Money> getCurrentAvailableBalance() const;
            void setCurrentAvailableBalance(Money const& balance);

        private:
            std::vector<Row> rows;
            std::optional<Date> beginDate; ///< beginning date of this statement
            std::optional<Date> endDate;   ///< ending date of this statement
            std::optional<Date> reportDate; ///< date this statement was generated
            std::optional<std::string> accountCode; ///< the number/code of the account this report refers to
            std::optional<std::string> accountDescription; ///< the description of the account
            std::optional<Money> currentAccountBalance; ///< The reported account balance at the end of the report (how much is in the account).
            std::optional<Money> currentAvailableBalance; ///< The reported *available* balance in the account at the end of the report.
    };
}

#endif
