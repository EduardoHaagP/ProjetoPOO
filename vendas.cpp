#include "vendas.h"

Vendas::Vendas()
{
}
Vendas::Vendas(Vendedor *vendedor, Clientes *cliente, Veiculos *veiculo, float valorBase, PoliticaDesconto *politicaDesconto, float valorEntrada, string formaPagamento, string statusVenda) : vendedor_responsavel(vendedor), cliente_comprador(cliente), veiculo_vendido(veiculo), valor_base(valorBase), politica_de_desconto(politicaDesconto), valor_entrada(valorEntrada), forma_pagamento(formaPagamento), status_venda(statusVenda)
{
    this->valor_final = politicaDesconto->calcularDesconto(valorBase);
}

Vendedor *Vendas::getVendedor()
{
    return vendedor_responsavel;
}
Clientes *Vendas::getCliente()
{
    return cliente_comprador;
}
Veiculos *Vendas::getVeiculo()
{
    return veiculo_vendido;
}
float Vendas::getValorBase()
{
    return valor_base;
}
float Vendas::getValorFinal()
{
    return valor_final;
}
float Vendas::getValorEntrada()
{
    return valor_entrada;
}
string Vendas::getFormaPagamento()
{
    return forma_pagamento;
}
string Vendas::getStatusVenda()
{
    return status_venda;
}
PoliticaDesconto *Vendas::getPoliticaDesconto()
{
    return politica_de_desconto;
}
void Vendas::setVendedor(Vendedor *vendedor)
{
    this->vendedor_responsavel = vendedor;
}
void Vendas::setCliente(Clientes *cliente)
{
    this->cliente_comprador = cliente;
}
void Vendas::setVeiculo(Veiculos *veiculo)
{
    this->veiculo_vendido = veiculo;
}
void Vendas::setValorBase(float valor)
{
    this->valor_base = valor;
}
void Vendas::setValorFinal(float valor)
{
    this->valor_final = valor;
}
void Vendas::setValorEntrada(float valor)
{
    this->valor_entrada = valor;
}
void Vendas::setFormaPagamento(string forma)
{
    this->forma_pagamento = forma;
}
void Vendas::setStatusVenda(string status)
{
    this->status_venda = status;
}
void Vendas::setPoliticaDesconto(PoliticaDesconto *politica)
{
    this->politica_de_desconto = politica;
}