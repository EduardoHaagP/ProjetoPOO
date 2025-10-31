#include "veiculos.h"

Veiculos::Veiculos(string n_modelo, int n_ano , float n_valor_base, string n_cor): 
    modelo(n_modelo), cor(n_cor), valor_base(n_valor_base), ano(n_ano) {};

string Veiculos::getModelo(){return this->modelo;}
string Veiculos::getCor(){return this->cor;}
float Veiculos::getValorBase(){return this->valor_base;}
int Veiculos::getAno(){return this->ano;}
void Veiculos::setModelo(string n_modelo){ this->modelo = n_modelo;};
void Veiculos::setCor(string n_cor){ this->cor = n_cor;};
void Veiculos::setValorBase(int n_valor_base){ this->valor_base = n_valor_base;};
void Veiculos::setAno(int n_ano){ this->ano = n_ano;};

Moto::Moto(string n_modelo, int n_ano , float n_valor_base, string n_cor): 
    Veiculos(n_modelo, n_ano, n_valor_base, n_cor){};
    
// CORREÇÃO: Retornar "Moto" para compatibilidade com CSV
string Moto::motoOuCarro() {return "Moto";}

Carro::Carro(string n_modelo, int n_ano , float n_valor_base, string n_cor): 
    Veiculos(n_modelo, n_ano, n_valor_base, n_cor){};
    
// CORREÇÃO: Retornar "Carro" para compatibilidade com CSV  
string Carro::motoOuCarro(){return "Carro";};