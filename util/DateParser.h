#ifndef BSTY_DATEPARSER_H
#define BSTY_DATEPARSER_H

#include <optional>
#include <string>

class DateParser
{
    public:
        struct YearMonthDay
        {
            YearMonthDay()
                :year(0)
                ,month(0)
                ,day(0)
            {}

            int year;
            unsigned int month;
            unsigned int day;
        };

        explicit DateParser(char const* formatString="%d-%m-%Y");

        std::optional<YearMonthDay> parse(std::string_view inputData);

    private:
        std::string const formatString_;
};


#endif
