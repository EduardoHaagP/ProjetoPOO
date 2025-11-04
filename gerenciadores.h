#ifndef GERENCIADORES_H
#define GERENCIADORES_H
#include <iostream>

#include <vector>
#include "veiculos.h"
#include "clientes.h"
#include "vendedor.h"
#include "vendas.h"
using namespace std;

// adiciona a definição para pegar o caminho do CMake
// PROJECT_SOURCE_DIR é definido em CMakeLists.txt
#ifndef PROJECT_SOURCE_DIR
#define PROJECT_SOURCE_DIR "." // fallback caso não seja definido
#endif

// func auxiliar pra construir o caminho completo
inline std::string getCaminhoDoProjeto(const std::string& relativePath) {
    return std::string(PROJECT_SOURCE_DIR) + "/" + relativePath;
}



class Gerenciadores
{
protected:
    string nome_arquivo;

public:
    virtual void carregar_do_csv() = 0;
    virtual void salvar_no_csv() = 0;
};

class GerenciadorDeVeiculos : public Gerenciadores
{
private:
    GerenciadorDeVeiculos(const string &arquivo = getCaminhoDoProjeto("Trabalho POO - Carros.csv"));
    GerenciadorDeVeiculos(const GerenciadorDeVeiculos &) = delete;
    GerenciadorDeVeiculos &operator=(const GerenciadorDeVeiculos &) = delete;
    std::vector<Veiculos *> veiculos;

public:
    static GerenciadorDeVeiculos &getInstance(const string &arquivo = getCaminhoDoProjeto("Trabalho POO - Carros.csv"));
    void carregar_do_csv() override;
    void salvar_no_csv() override;
    vector<Veiculos *> listar();
    vector<Veiculos *> buscar(string modelo_carro);
    void adicionar(Veiculos *novo_veiculo);
    bool remover(int indice);
    bool remover_por_modelo(string modelo);
    int getTotalVeiculos();
};

class GerenciadorDeClientes : public Gerenciadores
{
private:
    GerenciadorDeClientes(const string &arquivo = getCaminhoDoProjeto("Trabalho POO - Clientes.csv"));
    GerenciadorDeClientes(const GerenciadorDeClientes &) = delete;
    GerenciadorDeClientes &operator=(const GerenciadorDeClientes &) = delete;
    std::vector<Clientes *> clientes;

public:
    static GerenciadorDeClientes &getInstance(const string &arquivo = getCaminhoDoProjeto("Trabalho POO - Clientes.csv"));
    void carregar_do_csv() override;
    void salvar_no_csv() override;
    vector<Clientes *> listar();
    vector<Clientes *> buscar(string termo_busca);
    Clientes* buscarPorDocumento(string doc_busca);
    void adicionar(Clientes *novo_cliente); // recebe ponteiro
    bool remover(int indice);
    bool remover_por_documento(string doc_procurado);
    int getTotalCliente();
};

class GerenciadorDeVendedores : public Gerenciadores
{
private:
    GerenciadorDeVendedores(const string &arquivo = getCaminhoDoProjeto("Trabalho POO - Vendedores.csv"));
    GerenciadorDeVendedores(const GerenciadorDeVendedores &) = delete;
    GerenciadorDeVendedores &operator=(const GerenciadorDeVendedores &) = delete;
    std::vector<Vendedor *> vendedores;

public:
    static GerenciadorDeVendedores &getInstance(const string &arquivo = getCaminhoDoProjeto("Trabalho POO - Vendedores.csv"));
    void carregar_do_csv() override;
    void salvar_no_csv() override;
    vector<Vendedor *> listar();
    vector<Vendedor *> buscar(string termo_busca);
    void adicionar(Vendedor *novo_vendedor);
    bool remover(int indice);
    bool remover_por_cpf(string cpf_procurado);
    int getTotalVendedor();
    Vendedor* buscarPorCpf(string cpf_busca);

    bool autenticar(string email, string senha, Vendedor** vendedor_autenticado);
    bool verificarEmailDisponivel(string email);
};

class GereniciadorDeVendas : public Gerenciadores
{
private:
    GereniciadorDeVendas(const string &arquivo = getCaminhoDoProjeto("Trabalho POO - Vendas.csv"));
    GereniciadorDeVendas(const GereniciadorDeVendas &) = delete;
    GereniciadorDeVendas &operator=(const GereniciadorDeVendas &) = delete;
    std::vector<Vendas *> vendas;

public:
    static GereniciadorDeVendas &getInstance(const string &arquivo = getCaminhoDoProjeto("Trabalho POO - Vendas.csv"));
    void carregar_do_csv() override;
    void salvar_no_csv() override;
    vector<Vendas *> listar();
    vector<Vendas *> buscarPorVendedor(string termo_busca);
    vector<Vendas *> buscarPorVeiculo(string termo_busca);
    void adicionar(Vendas *nova_venda);
    int getTotalVendas();
    bool verificarConsistenciaDados();
};
#endif // GERENCIADORES_H
