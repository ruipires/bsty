#ifndef BSTY_CODEPAGETOUTF8CONTERTER_H
#define BSTY_CODEPAGETOUTF8CONTERTER_H

#include <string>

class CodePageToUtf8Conterter
{
    public:
        std::wstring cp1252ToUtf16(std::string const& in);

        std::string fromCp1252(std::string const& in);
};


#endif
