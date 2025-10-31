#include "vendas.h"

// IMPLEMENTAÇÃO da função DataUtils::fromString (no .cpp)
namespace DataUtils {
    Data fromString(const std::string& dataStr) {
        Data data;
        size_t pos1 = dataStr.find('/');
        size_t pos2 = dataStr.find('/', pos1 + 1);
        
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            data.setDia(std::stoi(dataStr.substr(0, pos1)));
            data.setMes(std::stoi(dataStr.substr(pos1 + 1, pos2 - pos1 - 1)));
            data.setAno(std::stoi(dataStr.substr(pos2 + 1)));
        }
        return data;
    }
}

Vendas::Vendas() : data_venda(Data(0,0,0)), valor_base(0), valor_final(0), valor_entrada(0) 
{
}

Vendas::Vendas(Vendedor* vendedor, Clientes* cliente, Veiculos* veiculo, 
               float valorBase, PoliticaDesconto* politicaDesconto, 
               float valorEntrada, string formaPagamento, string statusVenda, 
               Data dataVenda)
    : vendedor_responsavel(vendedor), cliente_comprador(cliente), 
      veiculo_vendido(veiculo), data_venda(dataVenda), valor_base(valorBase), 
      politica_de_desconto(politicaDesconto), valor_entrada(valorEntrada), 
      forma_pagamento(formaPagamento), status_venda(statusVenda) 
{
    // Calcular valor final baseado na política de desconto
    this->valor_final = politicaDesconto->calcularDesconto(valorBase);
}

// Resto das implementações permanecem iguais...
Vendedor* Vendas::getVendedor() { return vendedor_responsavel; }
Clientes* Vendas::getCliente() { return cliente_comprador; }
Veiculos* Vendas::getVeiculo() { return veiculo_vendido; }
float Vendas::getValorBase() { return valor_base; }
float Vendas::getValorFinal() { return valor_final; }
float Vendas::getValorEntrada() { return valor_entrada; }
string Vendas::getFormaPagamento() { return forma_pagamento; }
string Vendas::getStatusVenda() { return status_venda; }   
PoliticaDesconto* Vendas::getPoliticaDesconto() { return politica_de_desconto; }

Data Vendas::getDataVenda() { return data_venda; }
string Vendas::getDataVendaString() { return data_venda.toString(); }

void Vendas::setVendedor(Vendedor* vendedor) { this->vendedor_responsavel = vendedor; }
void Vendas::setCliente(Clientes* cliente) { this->cliente_comprador = cliente; }
void Vendas::setVeiculo(Veiculos* veiculo) { this->veiculo_vendido = veiculo; }
void Vendas::setValorBase(float valor) { this->valor_base = valor; }
void Vendas::setValorFinal(float valor) { this->valor_final = valor; }
void Vendas::setValorEntrada(float valor) { this->valor_entrada = valor; }
void Vendas::setFormaPagamento(string forma) { this->forma_pagamento = forma; }
void Vendas::setStatusVenda(string status) { this->status_venda = status; }
void Vendas::setPoliticaDesconto(PoliticaDesconto* politica) { 
    this->politica_de_desconto = politica; 
}

void Vendas::setDataVenda(Data data) { this->data_venda = data; }
void Vendas::setDataVendaFromString(const std::string& dataStr) { 
    this->data_venda = DataUtils::fromString(dataStr); 
}