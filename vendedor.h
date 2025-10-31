#ifndef VENDEDOR_H
#define VENDEDOR_H

#include <string>
using namespace std;

class Vendedor
{
private:
    string nome;
    string cpf;
    string telefone;
    string endereco;
    string email;
    string senha;

public:
    Vendedor();
    Vendedor(string n_nome, string n_cpf, string n_telefone, string n_endereco, string n_email, string n_senha);
    
    // Getters
    string getNome();
    string getCpf();
    string getTelefone();
    string getEndereco();
    string getEmail();
    string getSenha();
    
    // Setters
    void setNome(string n_nome);
    void setCpf(string n_cpf);
    void setTelefone(string n_telefone);
    void setEndereco(string n_endereco);
    void setEmail(string n_email);
    void setSenha(string n_senha);
};

#endif // VENDEDOR_H
