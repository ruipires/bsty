#ifndef BSTY_CGD_CONTAORDEM_H
#define BSTY_CGD_CONTAORDEM_H

#include <string>
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

            bsty::core::Data data;
    };
}

#endif
