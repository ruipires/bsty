#ifndef BSTY_ACCOUNTSTATEMENTPARSER_H
#define BSTY_ACCOUNTSTATEMENTPARSER_H
#include "BstyData.h"

namespace bsty::core
{
    class AccountStatementParser
    {
        public:
            virtual ~AccountStatementParser() = default;
            [[nodiscard]] virtual Data loadFrom(std::string const &filename) const = 0;
    };
}

#endif