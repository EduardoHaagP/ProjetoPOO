#ifndef VENDAS_H
#define VENDAS_H
#include <iostream>
#include <string>
#include "veiculos.h"
#include "clientes.h"
#include "vendedor.h"
#include "descontos.h"

class Data
{
private:
    int dia;
    int mes;
    int ano;

public:
    // Construtores
    Data() : dia(0), mes(0), ano(0) {}
    Data(int d, int m, int a) : dia(d), mes(m), ano(a) {}
    
    // Getters
    int getDia() const { return dia; }
    int getMes() const { return mes; }
    int getAno() const { return ano; }
    
    // Setters
    void setDia(int d) { dia = d; }
    void setMes(int m) { mes = m; }
    void setAno(int a) { ano = a; }
    
    // Método para formatar data como string
    std::string toString() const {
        return std::to_string(dia) + "/" + std::to_string(mes) + "/" + std::to_string(ano);
    }
};

// Namespace para funções auxiliares (apenas DECLARAÇÃO no .h)
namespace DataUtils {
    // DECLARAÇÃO da função (sem implementação)
    Data fromString(const std::string& dataStr);
}

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
    string filial;

    
public:
    Vendas();
    Vendas(Vendedor* vendedor, Clientes* cliente, Veiculos* veiculo, 
           float valorBase, PoliticaDesconto* politicaDesconto, 
           float valorEntrada, string formaPagamento, string statusVenda, 
           Data dataVenda, string filial);
    
    // Getters
    Vendedor* getVendedor();
    Clientes* getCliente();
    Veiculos* getVeiculo();
    float getValorBase();
    float getValorFinal();
    float getValorEntrada();
    string getFormaPagamento();
    string getStatusVenda();
    PoliticaDesconto* getPoliticaDesconto();
    Data getDataVenda();
    string getDataVendaString();
    string getFilial();
    // Setters
    void setVendedor(Vendedor* vendedor);
    void setCliente(Clientes* cliente);
    void setVeiculo(Veiculos* veiculo);
    void setValorBase(float valor);
    void setValorFinal(float valor);
    void setValorEntrada(float valor);
    void setFormaPagamento(string forma);
    void setStatusVenda(string status);
    void setPoliticaDesconto(PoliticaDesconto* politica);
    void setDataVenda(Data data);
    void setDataVendaFromString(const std::string& dataStr);
    void setFilial(string filial);
};

#endif // VENDAS_H