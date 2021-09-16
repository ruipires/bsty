#ifndef BSTY_CGD_CONTAORDEM_H
#define BSTY_CGD_CONTAORDEM_H

#include <string>
#include <optional>
#include "core/BstyData.h"
#include "core/AccountStatementParser.h"

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

    class ContaOrdemParser: public bsty::core::AccountStatementParser
    {
        public:
            ~ContaOrdemParser() override = default;

            bsty::core::Data loadFrom(std::string const &filename) const override;
    };

    namespace util
    {
        std::optional<bsty::core::Date> parseCsvHeaderLineStatementDate(std::string_view sv);
        std::optional<std::pair<std::string,std::string>> parseCsvHeaderLineAccountNumberAndDescription(std::string_view sv);
        std::optional<bsty::core::Date> parseCsvHeaderLineBeginningDate(std::string_view sv);
        std::optional<bsty::core::Date> parseCsvHeaderLineEndingDate(std::string_view sv);

        std::string_view trim_left(std::string_view sv);
        std::string_view trim_right(std::string_view sv);
        std::string_view trim(std::string_view sv);
    }
}

#endif
