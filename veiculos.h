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
public:
    Veiculos(string n_modelo, string n_cor, float n_valor_base, int n_ano): modelo(n_modelo), cor(n_cor), valor_base(n_valor_base), ano(n_ano) {};
    string getModelo(){return this->modelo;}
    string getCor(){return this->cor;}
    float getValorBase(){return this->valor_base;}
    int getAno(){return this->ano;}
    void setModelo(string n_modelo){ this->modelo = n_modelo;};
    void setCor(string n_cor){ this->cor = n_cor;};
    void setValorBase(int n_valor_base){ this->valor_base = n_valor_base;};
    void setAno(int n_ano){ this->ano = n_ano;};
    virtual string motoOuCarro () = 0;
};

class Moto: public Veiculos{
    Moto();
    Moto(string n_modelo, string n_cor, float n_valor_base, int n_ano): Veiculos(n_modelo, n_cor, n_valor_base, n_ano){};
    string motoOuCarro() override{return "moto";}
};
class Carro: public Veiculos{
    Carro();
    Carro(string n_modelo, string n_cor, float n_valor_base, int n_ano): Veiculos(n_modelo, n_cor, n_valor_base, n_ano){};
    string motoOuCarro() override{return "carro";}
};

#endif // VEICULOS_H
