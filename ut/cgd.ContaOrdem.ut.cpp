#include <catch2/catch.hpp>
#include "cgd/ContaOrdem.h"

TEST_CASE("cleanup_cgd_co_file", "cgd" )
{
    std::string const in = R"raw(Consultar saldos e movimentos ? ordem - 02-02-2020

Conta ;0099048884300 - EUR - Nossa Conta
Data de in?cio ;02-01-2020
Data de fim ;02-02-2020

Data mov. ;Data valor ;Descri??o ;D?bito ;Cr?dito ;Saldo contabil?stico ;Saldo dispon?vel ;Categoria ;
01-02-2020;01-02-2020;COMISSAO CONTA CAIXA ;4,16;;23,27;23,27;Diversos ;
01-02-2020;01-02-2020;PAGAMENTO ;40,00;;27,43;27,43;INST. FINANCEIRAS ;
 ; ; ; ;Saldo contabil?stico ;23,27 EUR ; ; ;
 ; ; Saldo dispon?vel  ;23,27 EUR ; ; ; ; ;
 Limite descoberto contratado  ;10,00 EUR ; ; ; ; ; Limite descoberto utilizado  ;0,00 EUR ;)raw";

    std::string const expected = R"raw(Data mov. ;Data valor ;Descri??o ;D?bito ;Cr?dito ;Saldo contabil?stico ;Saldo dispon?vel ;Categoria ;
01-02-2020;01-02-2020;COMISSAO CONTA CAIXA ;4,16;;23,27;23,27;Diversos ;
01-02-2020;01-02-2020;PAGAMENTO ;40,00;;27,43;27,43;INST. FINANCEIRAS ;
)raw";

    auto const out = cgd::cleanup_cgd_co_file(in);

    REQUIRE(out == expected);
}

