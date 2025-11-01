#include "descontos.h"
#include <iostream>
#include <vector>

PoliticaDesconto::~PoliticaDesconto() = default;
// Concrete Strategies

double SemDesconto::calcularDesconto(double valorBase) const
{
    return valorBase; // Sem desconto
};

std::string SemDesconto::getNome() const
{
    return "Sem desconto";
};

std::string SemDesconto::getDescricao() const
{
    return "Tarifa padrão";
};

double SemDesconto::getPercentual() const
{
    return 0.0;
}

double ClienteFidelidade::calcularDesconto(double valorBase) const
{
    return valorBase * 0.95; // 5% de desconto
}

std::string ClienteFidelidade::getNome() const
{
    return "Cliente Fidelidade";
}

std::string ClienteFidelidade::getDescricao() const
{
    return "Cliente já realizou compra anterior";
}

double ClienteFidelidade::getPercentual() const
{
    return 5.0;
}

double PromocaoEspecial::calcularDesconto(double valorBase) const
{
    return valorBase * 0.90; // 10% de desconto
}

std::string PromocaoEspecial::getNome() const
{
    return "Promoção Especial";
}

std::string PromocaoEspecial::getDescricao() const
{
    return "Desconto sazonal ou campanha da marca";
}

double PromocaoEspecial::getPercentual() const
{
    return 10.0;
}

double VendasCorporativas::calcularDesconto(double valorBase) const
{
    return valorBase * 0.85; // 15% de desconto
}

std::string VendasCorporativas::getNome() const
{
    return "Vendas Corporativas";
}

std::string VendasCorporativas::getDescricao() const
{
    return "Empresa comprando frota (2 ou + veículos)";
}

double VendasCorporativas::getPercentual() const
{
    return 15.0;
}

// Construtor padrão com SemDesconto
CalculadoraDesconto::CalculadoraDesconto() : politica(new SemDesconto()) {};

// Construtor com política específica
CalculadoraDesconto::CalculadoraDesconto(PoliticaDesconto *politica) : politica(politica) {}
// Destrutor
CalculadoraDesconto::~CalculadoraDesconto()
{
    delete politica;
}

// Setter para mudar a política em tempo de execução
void CalculadoraDesconto::setPoliticaDesconto(PoliticaDesconto *novaPolitica)
{
    delete politica; // Libera a política anterior
    politica = novaPolitica;
}

// Método principal que usa a strategy
double CalculadoraDesconto::calcularValorComDesconto(double valorBase) const
{
    return politica->calcularDesconto(valorBase);
}

std::string CalculadoraDesconto::getNomePolitica() const
{
    return politica->getNome();
}

std::string CalculadoraDesconto::getDescricaoPolitica() const
{
    return politica->getDescricao();
}

double CalculadoraDesconto::getPercentualDesconto() const
{
    return politica->getPercentual();
}

// Factory para criar políticas de desconto

PoliticaDesconto *FabricaPoliticasDesconto::criarPolitica(const std::string &tipo)
{
    if (tipo == "SemDesconto" || tipo == "sem desconto")
    {
        return new SemDesconto();
    }
    else if (tipo == "ClienteFidelidade" || tipo == "cliente fidelidade")
    {
        return new ClienteFidelidade();
    }
    else if (tipo == "PromocaoEspecial" || tipo == "promoção especial")
    {
        return new PromocaoEspecial();
    }
    else if (tipo == "VendasCorporativas" || tipo == "vendas corporativas")
    {
        return new VendasCorporativas();
    }
    else
    {
        // Default: Sem desconto
        return new SemDesconto();
    }
}

void FabricaPoliticasDesconto::listarPoliticasDisponiveis()
{
    std::cout << "=== POLÍTICAS DE DESCONTO DISPONÍVEIS ===" << std::endl;

    SemDesconto sem;
    std::cout << "1. " << sem.getNome() << " (" << sem.getPercentual() << "%) - "
              << sem.getDescricao() << std::endl;

    ClienteFidelidade fidelidade;
    std::cout << "2. " << fidelidade.getNome() << " (" << fidelidade.getPercentual() << "%) - "
              << fidelidade.getDescricao() << std::endl;

    PromocaoEspecial promocao;
    std::cout << "3. " << promocao.getNome() << " (" << promocao.getPercentual() << "%) - "
              << promocao.getDescricao() << std::endl;

    VendasCorporativas corporativas;
    std::cout << "4. " << corporativas.getNome() << " (" << corporativas.getPercentual() << "%) - "
              << corporativas.getDescricao() << std::endl;

    std::cout << "========================================" << std::endl;
}
