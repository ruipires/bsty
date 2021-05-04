#ifndef BSTY_CGD_CONTAORDEM_H
#define BSTY_CGD_CONTAORDEM_H

#include <string>
#include <optional>
#include "core/BstyData.h"

namespace cgd
{
    class ContaOrdem
    {
        public:
            static ContaOrdem loadFromCsv(std::string const& filename);

            bsty::core::Data const& getData() const;

        private:
            explicit ContaOrdem(bsty::core::Data data);

            static bsty::core::Data loadCsv(std::string const& filename);
            static bsty::core::Data loadCsv(std::string const& filename, std::istream & in);

            static bool loadFileHeader(std::istream &inFile, bsty::core::Data &data);

            bsty::core::Data data;
    };

    namespace util
    {
        std::optional<std::string> parseCsvHeaderLineStatementDate(std::string const& str);
        std::optional<std::pair<std::string,std::string>> parseCsvHeaderLineAccountNumberAndDescription(std::string const& str);
        std::optional<std::string> parseCsvHeaderLineBeginningDate(std::string const& str);
        std::optional<std::string> parseCsvHeaderLineEndingDate(std::string const& str);

        std::string_view trim_left(std::string_view sv);
        std::string_view trim_right(std::string_view sv);
        std::string_view trim(std::string_view sv);
    }
}

#endif
