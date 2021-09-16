#ifndef BSTY_PARSERMAPPING_H
#define BSTY_PARSERMAPPING_H
#include "AccountStatementParser.h"

#include <map>
#include <string>
#include <memory>
#include <optional>

namespace bsty::core
{
    class ParserMapping
    {
        public:
            void registerParser(std::string const& accountType,
                                std::string const& fileFormat,
                                std::unique_ptr<AccountStatementParser>);
            AccountStatementParser const * getParser(std::string const& bankAcronym,
                                                     std::string const& fileFormat) const;

        private:
            /** First element is account type, second is file format */
            typedef std::tuple<std::string, std::string> Key;

            std::map<Key, std::unique_ptr<AccountStatementParser>> mapping;
    };
}

#endif
