#ifndef BSTY_CODEPAGETOUTF8CONVERTER_H
#define BSTY_CODEPAGETOUTF8CONVERTER_H

#include <string>

class CodePageToUtf8Converter
{
    public:
        explicit CodePageToUtf8Converter(char const * datLocation);
        std::string fromCp1252(std::string const& in);
};


#endif
