#ifndef CLIENTES_H
#define CLIENTES_H
#include <iostream>
#include <string>
using namespace std;

class Clientes
{
private:
    string nome;
    string cpf;
    string telefone;
    string endereco;
public:
    Clientes(string nome, string cpf, string telefone, string endereco);
    string getNome();
    string getCpf();
    string getTelefone();
    string getEndereco();
    void setNome(string nome);
    void setCpf(string cpf);
    void setTelefone(string telefone);
    void setEndereco(string endereco);
};

#endif // CLIENTES_H
