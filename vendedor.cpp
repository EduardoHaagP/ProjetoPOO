#include "vendedor.h"

Vendedor::Vendedor() : nome(""), cpf(""), telefone(""), endereco(""), email(""), senha("") {}

Vendedor::Vendedor(string n_nome, string n_cpf, string n_telefone, string n_endereco, string n_email, string n_senha)
    : nome(n_nome), cpf(n_cpf), telefone(n_telefone), endereco(n_endereco), email(n_email), senha(n_senha) {}

string Vendedor::getNome() { return this->nome; }
string Vendedor::getCpf() { return this->cpf; }
string Vendedor::getTelefone() { return this->telefone; }
string Vendedor::getEndereco() { return this->endereco; }
string Vendedor::getEmail() { return this->email; }
string Vendedor::getSenha() { return this->senha; }

void Vendedor::setNome(string n_nome) { this->nome = n_nome; }
void Vendedor::setCpf(string n_cpf) { this->cpf = n_cpf; }
void Vendedor::setTelefone(string n_telefone) { this->telefone = n_telefone; }
void Vendedor::setEndereco(string n_endereco) { this->endereco = n_endereco; }
void Vendedor::setEmail(string n_email) { this->email = n_email; }
void Vendedor::setSenha(string n_senha) { this->senha = n_senha; }
