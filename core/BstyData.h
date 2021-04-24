#ifndef BSTY_BSTYDATA_H
#define BSTY_BSTYDATA_H

#include <utility>
#include <vector>
#include <string>

namespace bsty
{
    namespace core
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

                int year;
                unsigned int month;
                unsigned int day;
        };

        std::string to_string(Date const& date);

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
            std::string const currency;
        };

        std::string to_string(Money const& money);

        struct Row
        {
            inline Row()
            {}

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
                std::vector<Row> const& getRows() const;

            private:
                std::vector<Row> rows;
        };

    }
}

#endif
