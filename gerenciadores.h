#ifndef GERENCIADORES_H
#define GERENCIADORES_H
#include <iostream>
#include <vector>
#include "veiculos.h"
#include "clientes.h"
using namespace std;

class Gerenciadores
{
protected:
    string nome_arquivo;
public:
    virtual void carregar_do_csv() = 0;
    virtual void salvar_no_csv() = 0;
};

class GerenciadorDeVeiculos: public Gerenciadores{
    private:
        GerenciadorDeVeiculos(const string &arquivo = "Trabalho POO - Carros.csv");
        GerenciadorDeVeiculos(const GerenciadorDeVeiculos&) = delete;
        GerenciadorDeVeiculos& operator=(const GerenciadorDeVeiculos&) = delete;
        std::vector<Veiculos*> veiculos;
    public: 
        static GerenciadorDeVeiculos& getInstance(const string& arquivo = "Trabalho POO - Carros.csv");
        void carregar_do_csv() override;
        void salvar_no_csv() override;
        vector<Veiculos*> listar();
        vector<Veiculos*> buscar(string modelo_carro);
        void adicionar(Veiculos* novo_veiculo);
        bool remover(int indice);
        bool remover_por_modelo(string modelo);
        int getTotalVeiculos();
};

class GerenciadorDeClientes: public Gerenciadores{
    private:
        // Implementação como Singleton também
        GerenciadorDeClientes(const string &arquivo = "clientes.csv");
        GerenciadorDeClientes(const GerenciadorDeClientes&) = delete;
        GerenciadorDeClientes& operator=(const GerenciadorDeClientes&) = delete;
        std::vector<Clientes*> clientes;
    
    public:
        static GerenciadorDeClientes& getInstance(const string& arquivo = "clientes.csv");
        void carregar_do_csv() override;
        void salvar_no_csv() override;
        vector<Clientes*> listar();
        vector<Clientes*> buscar(string termo_busca);
        void adicionar(Clientes* novo_cliente); // CORRIGIDO: recebe ponteiro
        bool remover(int indice);
        bool remover_por_cpf(string cpf_procurado);
        int getTotalCliente();
};

#endif // GERENCIADORES_H