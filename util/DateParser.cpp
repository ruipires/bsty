#include "DateParser.h"
#include <date/date.h>

DateParser::DateParser(char const *formatString)
:formatString_(formatString)
{
}

std::optional<DateParser::YearMonthDay> DateParser::parse(std::string_view inputData)
{
    std::optional<YearMonthDay> result;

    std::istringstream date{inputData.data()};
    date::sys_time<date::days> tp;
    date >> date::parse(formatString_, tp);
    date::year_month_day x{tp};

    if(date)
    {
        YearMonthDay ymd;
        ymd.year = static_cast<int>(x.year());
        ymd.month = static_cast<unsigned int>(x.month());
        ymd.day= static_cast<unsigned int>(x.day());
        result = ymd;
    }

    return result;
}
