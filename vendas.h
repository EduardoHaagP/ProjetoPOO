#ifndef VENDAS_H
#define VENDAS_H
#include <iostream>
#include <string>
#include "veiculos.h"
#include "clientes.h"
#include "vendedor.h"
#include "descontos.h"

typedef struct Data
{
    int dia;
    int mes;
    int ano;
};

class Vendas
{
private:
    Vendedor *vendedor_responsavel;
    Clientes *cliente_comprador;
    string filial_venda;
    Veiculos *veiculo_vendido;
    Data data_venda;
    float valor_base;
    PoliticaDesconto *politica_de_desconto;
    float valor_final;
    float valor_entrada;
    string forma_pagamento;
    string status_venda;
public:
    Vendas();
    Vendas(Vendedor* vendedor, Clientes* cliente, Veiculos* veiculo, float valorBase, PoliticaDesconto* politicaDesconto, float valorEntrada, string formaPagamento, string statusVenda);
    Vendedor* getVendedor();
    Clientes* getCliente();
    Veiculos* getVeiculo();
    float getValorBase();
    float getValorFinal();
    float getValorEntrada();
    string getFormaPagamento();
    string getStatusVenda();
    PoliticaDesconto* getPoliticaDesconto();
    void setVendedor(Vendedor* vendedor);
    void setCliente(Clientes* cliente);
    void setVeiculo(Veiculos* veiculo);
    void setValorBase(float valor);
    void setValorFinal(float valor);
    void setValorEntrada(float valor);
    void setFormaPagamento(string forma);
    void setStatusVenda(string status);
    void setPoliticaDesconto(PoliticaDesconto* politica);

};

#endif // VENDAS_H
