#ifndef BSTY_CGD_CONTAORDEM_H
#define BSTY_CGD_CONTAORDEM_H

#include <string>

namespace cgd
{
    class ContaOrdem
    {
        public:
            static void load_csv(std::string const& filename);
    };
}

#endif
