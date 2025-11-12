#ifndef VEICULOS_H
#define VEICULOS_H
#include <iostream>
#include <string>
using namespace std;

class Veiculos
{
protected:
    string modelo;
    string cor;
    float valor_base;
    int ano;
    string filial;

public:
    Veiculos(string n_modelo, int n_ano, float n_valor_base, string n_cor, string n_filial);
    string getModelo();
    string getCor();
    float getValorBase();
    int getAno();
    string getFilial();
    void setModelo(string n_modelo);
    void setCor(string n_cor);
    void setValorBase(int n_valor_base);
    void setAno(int n_ano);
    void setFilial(string n_filial);
    virtual string motoOuCarro() = 0;
};

class Moto : public Veiculos
{
public:
    Moto(string n_modelo, int n_ano, float n_valor_base, string n_cor, string n_filial);
    string motoOuCarro() override;
};
class Carro : public Veiculos
{
public:
    Carro(string n_modelo, int n_ano, float n_valor_base, string n_cor, string n_filial);
    string motoOuCarro() override;
};

#endif // VEICULOS_H
