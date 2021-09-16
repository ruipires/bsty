#include "ParserMapping.h"
#include <spdlog/spdlog.h>
#include <utility>

namespace bsty::core
{
    void ParserMapping::registerParser(std::string const& accountType,
                                       std::string const& fileFormat,
                                       std::unique_ptr<AccountStatementParser> parser)
    {
        spdlog::debug("* Registered account parser for '{}' in '{}' format.",
                      accountType, fileFormat);

        mapping[std::make_tuple(accountType, fileFormat)] = std::move(parser);
    }

    AccountStatementParser const * ParserMapping::getParser(std::string const& accountType,
                                                      std::string const& fileFormat) const
    {
        AccountStatementParser * result = nullptr;

        auto const iterator = mapping.find(std::make_tuple(accountType, fileFormat));
        if(iterator != mapping.end())
        {
            result = iterator->second.get();
        }

        return result;
    }
}