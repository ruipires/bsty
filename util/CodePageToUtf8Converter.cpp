#include "CodePageToUtf8Converter.h"

#include "unicode/putil.h"
#include "unicode/unistr.h"

std::string CodePageToUtf8Converter::fromCp1252(std::string const &in)
{
    icu::UnicodeString us(in.data(), static_cast<int32_t>(in.size()), "cp1252");

    std::string result;
    icu::StringByteSink<std::string> sink(&result);
    us.toUTF8(sink);

    return result;
}

CodePageToUtf8Converter::CodePageToUtf8Converter(char const *datLocation)
{
    u_setDataDirectory(datLocation);
}
