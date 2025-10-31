#ifndef DESCONTOS_H
#define DESCONTOS_H

#include <string>
#include <iostream>

// Interface Strategy
class PoliticaDesconto
{
public:
    virtual ~PoliticaDesconto();
    virtual double calcularDesconto(double valorBase) const = 0;
    virtual std::string getNome() const = 0;
    virtual std::string getDescricao() const = 0;
    virtual double getPercentual() const = 0;
};

// Concrete Strategies
class SemDesconto : public PoliticaDesconto
{
public:
    double calcularDesconto(double valorBase) const override;

    std::string getNome() const override;

    std::string getDescricao() const override;

    double getPercentual() const override;
};

class ClienteFidelidade : public PoliticaDesconto
{
public:
    double calcularDesconto(double valorBase) const override;

    std::string getNome() const override;

    std::string getDescricao() const override;

    double getPercentual() const override;
};

class PromocaoEspecial : public PoliticaDesconto
{
public:
    double calcularDesconto(double valorBase) const override;

    std::string getNome() const override;

    std::string getDescricao() const override;

    double getPercentual() const override;
};

class VendasCorporativas : public PoliticaDesconto
{
public:
    double calcularDesconto(double valorBase) const override;

    std::string getNome() const override;

    std::string getDescricao() const override;

    double getPercentual() const override;
};

// Context class que usa a Strategy
class CalculadoraDesconto
{
private:
    PoliticaDesconto *politica;

public:
    // Construtor padrão com SemDesconto
    CalculadoraDesconto();

    // Construtor com política específica
    CalculadoraDesconto(PoliticaDesconto *politica);

    // Destrutor
    ~CalculadoraDesconto();

    // Setter para mudar a política em tempo de execução
    void setPoliticaDesconto(PoliticaDesconto *novaPolitica);

    // Método principal que usa a strategy
    double calcularValorComDesconto(double valorBase) const;

    std::string getNomePolitica() const;

    std::string getDescricaoPolitica() const;

    double getPercentualDesconto() const;
};

// Factory para criar políticas de desconto
class FabricaPoliticasDesconto
{
public:
    static PoliticaDesconto *criarPolitica(const std::string &tipo);

    static void listarPoliticasDisponiveis();
};

#endif // DESCONTOS_H
