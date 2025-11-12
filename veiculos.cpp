#include "veiculos.h"

Veiculos::Veiculos(string n_modelo, int n_ano, float n_valor_base, string n_cor, string n_filial) : modelo(n_modelo), cor(n_cor), valor_base(n_valor_base), ano(n_ano), filial(n_filial) {};

string Veiculos::getModelo() { return this->modelo; }
string Veiculos::getCor() { return this->cor; }
float Veiculos::getValorBase() { return this->valor_base; }
int Veiculos::getAno() { return this->ano; }
string Veiculos::getFilial() { return this->filial; }
void Veiculos::setModelo(string n_modelo) { this->modelo = n_modelo; };
void Veiculos::setCor(string n_cor) { this->cor = n_cor; };
void Veiculos::setValorBase(int n_valor_base) { this->valor_base = n_valor_base; };
void Veiculos::setAno(int n_ano) { this->ano = n_ano; };
void Veiculos::setFilial(string n_filial) { this->filial = n_filial; };

Moto::Moto(string n_modelo, int n_ano, float n_valor_base, string n_cor, string n_filial) : Veiculos(n_modelo, n_ano, n_valor_base, n_cor, n_filial) {};

string Moto::motoOuCarro() { return "Moto"; }

Carro::Carro(string n_modelo, int n_ano, float n_valor_base, string n_cor, string n_filial) : Veiculos(n_modelo, n_ano, n_valor_base, n_cor, n_filial) {};

string Carro::motoOuCarro() { return "Carro"; };
