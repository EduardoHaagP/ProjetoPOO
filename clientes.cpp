#include "clientes.h"

Clientes::Clientes(string n_nome, string n_cpf, string n_telefone, string n_endereco) : nome(n_nome), cpf(n_cpf), telefone(n_telefone), endereco(n_endereco) {};

string Clientes::getNome(){return this->nome;};
string Clientes::getCpf(){return this->cpf;};
string Clientes::getTelefone(){return this->telefone;};
string Clientes::getEndereco(){return this->endereco;};
void Clientes::setNome(string n_nome){this->nome = nome;return;};
void Clientes::setCpf(string n_cpf){this->cpf = cpf;return;};
void Clientes::setTelefone(string n_telefone){this->telefone = n_telefone;return;};
void Clientes::setEndereco(string n_endereco){this->endereco = n_endereco;return;};
