#ifndef CLIENTES_H
#define CLIENTES_H
#include <iostream>
#include <string>
using namespace std;

//classe abistrata - classe mae
class Clientes
{
protected:
    string nome;
    string telefone;
    string endereco;
public:
    // construtor
    Clientes(string nome, string telefone, string endereco);
    virtual ~Clientes() = default;

    // getters
    string getNome();
    string getTelefone();
    string getEndereco();

    // setters
    void setNome(string nome);
    void setTelefone(string telefone);
    void setEndereco(string endereco);

    virtual string getDocumento() = 0;
    virtual string getTipoCliente() = 0;
};


//pessoa fisica clase filha
class PessoaFisica : public Clientes
{
private:
    string cpf;
public:
    PessoaFisica(string nome, string cpf, string telefone, string endereco);

    string getDocumento() override;
    string getTipoCliente() override;

    void setCpf(string cpf);
};


//pessoa juridica - classe filhaa
class PessoaJuridica : public Clientes
{
private:
    string cnpj;
public:
    PessoaJuridica(string nome, string cnpj, string telefone, string endereco);

    string getDocumento() override;
    string getTipoCliente() override;

    void setCnpj(string cnpj);
};

#endif // CLIENTES_H
