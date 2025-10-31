#ifndef GERENCIADORES_H
#define GERENCIADORES_H
#include <iostream>
#include <vector>
#include "veiculos.h"
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
        // construtor privado para implementar singleton
        GerenciadorDeVeiculos(const string &arquivo = "Trabalho POO - Carros.csv");

        // Deletar construtor de cópia e operador de atribuição
        //nao pode existir mais de um objeto, somente um não uma copia nem um clone
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
        bool remover(int indice); // NOVO: Remover por índice
        bool remover_por_modelo(string modelo); // NOVO: Remover por modelo
        int getTotalVeiculos(); // NOVO: Obter total de veículos
        
};

#endif // GERENCIADORES_H
