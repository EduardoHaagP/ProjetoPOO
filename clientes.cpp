#include "clientes.h"

//classe base
Clientes::Clientes(string n_nome, string n_telefone, string n_endereco)
    : nome(n_nome), telefone(n_telefone), endereco(n_endereco) {}

string Clientes::getNome(){return this->nome;}
string Clientes::getTelefone(){return this->telefone;}
string Clientes::getEndereco(){return this->endereco;}
void Clientes::setNome(string n_nome){this->nome = n_nome;}
void Clientes::setTelefone(string n_telefone){this->telefone = n_telefone;}
void Clientes::setEndereco(string n_endereco){this->endereco = n_endereco;}


//pessoa juridica
PessoaFisica::PessoaFisica(string n_nome, string n_cpf, string n_telefone, string n_endereco)
    : Clientes(n_nome, n_telefone, n_endereco), cpf(n_cpf) {}

string PessoaFisica::getDocumento() { return this->cpf; }
string PessoaFisica::getTipoCliente() { return "PF"; }
void PessoaFisica::setCpf(string n_cpf) { this->cpf = n_cpf; }


//pessoa fisica
PessoaJuridica::PessoaJuridica(string n_nome, string n_cnpj, string n_telefone, string n_endereco)
    : Clientes(n_nome, n_telefone, n_endereco), cnpj(n_cnpj) {}

string PessoaJuridica::getDocumento() { return this->cnpj; }
string PessoaJuridica::getTipoCliente() { return "PJ"; }
void PessoaJuridica::setCnpj(string n_cnpj) { this->cnpj = n_cnpj; }
