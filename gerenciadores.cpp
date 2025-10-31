#include "gerenciadores.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm> // ADICIONAR ESTE INCLUDE
#include <cctype>    // ADICIONAR PARA ::tolower
#include <cmath>

// Definição do construtor
GerenciadorDeVeiculos::GerenciadorDeVeiculos(const string &arquivo)
{
    this->nome_arquivo = arquivo;
}

// Definição do método estático getInstance
GerenciadorDeVeiculos &GerenciadorDeVeiculos::getInstance(const string &arquivo)
{
    static GerenciadorDeVeiculos instancia(arquivo);
    return instancia;
}

void GerenciadorDeVeiculos::adicionar(Veiculos *novo_veiculo)
{
    this->veiculos.push_back(novo_veiculo);
}

std::string trim(const std::string &str)
{
    size_t inicio = str.find_first_not_of(" \t\r\n");
    size_t fim = str.find_last_not_of(" \t\r\n");

    if (inicio == std::string::npos)
    {
        return "";
    }

    return str.substr(inicio, fim - inicio + 1);
}

void GerenciadorDeVeiculos::carregar_do_csv()
{
    std::ifstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }

    std::string linha;
    int linha_num = 0;

    // Pular cabeçalho
    if (std::getline(arquivo, linha))
    {
        linha_num++;
    }

    while (std::getline(arquivo, linha))
    {
        linha_num++;

        // Ignorar linhas vazias
        if (trim(linha).empty())
        {
            continue;
        }

        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> dados;

        while (std::getline(ss, campo, ','))
        {
            dados.push_back(trim(campo));
        }

        // Validar dados - agora com tratamento adequado
        if (dados.size() >= 5)
        {
            try
            {
                Veiculos *novoVeiculo = nullptr;
                std::string tipo = dados[0];

                if (tipo == "Moto")
                {
                    novoVeiculo = new Moto(dados[1], std::stoi(dados[2]),
                                           std::stof(dados[3]), dados[4]);
                }
                else if (tipo == "Carro")
                {
                    novoVeiculo = new Carro(dados[1], std::stoi(dados[2]),
                                            std::stof(dados[3]), dados[4]);
                }
                else
                {
                    std::cerr << "Tipo de veículo desconhecido: " << tipo
                              << " na linha " << linha_num << std::endl;
                    continue;
                }

                if (novoVeiculo != nullptr)
                {
                    veiculos.push_back(novoVeiculo);
                    std::cout << "Veículo carregado: " << tipo << ", "
                              << dados[1] << ", " << dados[2] << std::endl;
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Erro ao converter dados numéricos na linha "
                          << linha_num << ": " << e.what() << std::endl;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Número fora do intervalo válido na linha "
                          << linha_num << ": " << e.what() << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Erro ao criar veículo na linha "
                          << linha_num << ": " << e.what() << std::endl;
            }
        }
        else
        {
            std::cerr << "Linha " << linha_num
                      << " com número incorreto de campos: " << dados.size() << std::endl;
        }
    }
    arquivo.close();
    std::cout << "Carregamento concluído. Total de veículos: "
              << veiculos.size() << std::endl;
}

void GerenciadorDeVeiculos::salvar_no_csv()
{
    std::ofstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }

    // Escreve cabeçalho
    arquivo << "Tipo,Modelo,Ano,Valor (R$),Cor\n";

    // Escreve cada veículo
    for (const auto &veiculo : this->veiculos)
    {
        // CORREÇÃO: Usar "Moto" e "Carro" em vez de "moto" e "carro"
        std::string tipo = veiculo->motoOuCarro();
        if (tipo == "moto")
            tipo = "Moto";
        else if (tipo == "carro")
            tipo = "Carro";

        arquivo << tipo << ","
                << veiculo->getModelo() << ","
                << veiculo->getAno() << ","
                << std::fixed << std::setprecision(2) << veiculo->getValorBase() << ","
                << veiculo->getCor() << "\n";
    }

    arquivo.close();
    std::cout << "Arquivo salvo com sucesso: " << this->nome_arquivo
              << ". Total de veículos: " << veiculos.size() << std::endl;
}

vector<Veiculos *> GerenciadorDeVeiculos::listar()
{
    return this->veiculos;
}

vector<Veiculos *> GerenciadorDeVeiculos::buscar(string termo_busca)
{
    vector<Veiculos *> resposta;

    // Função helper para converter para minúsculas manualmente
    auto to_lower = [](const std::string &str)
    {
        std::string result = str;
        for (char &c : result)
        {
            c = std::tolower(c);
        }
        return result;
    };

    std::string termo_lower = to_lower(termo_busca);

    for (size_t i = 0; i < this->veiculos.size(); i++)
    {
        std::string modelo = this->veiculos[i]->getModelo();
        std::string modelo_lower = to_lower(modelo);

        // Buscar por substring
        if (modelo_lower.find(termo_lower) != std::string::npos)
        {
            resposta.push_back(this->veiculos[i]);
        }
    }

    return resposta;
}

// NOVO: Remover veículo por índice
bool GerenciadorDeVeiculos::remover(int indice)
{
    if (indice < 0 || indice >= (int)this->veiculos.size())
    {
        std::cerr << "Erro: Índice " << indice << " inválido!" << std::endl;
        return false;
    }

    // Deletar o objeto da memória
    delete this->veiculos[indice];

    // Remover do vetor
    this->veiculos.erase(this->veiculos.begin() + indice);

    std::cout << "Veículo removido com sucesso! Índice: " << indice << std::endl;
    return true;
}

// NOVO: Remover veículo por modelo (remove o primeiro encontrado)
bool GerenciadorDeVeiculos::remover_por_modelo(string modelo)
{
    auto to_lower = [](const std::string &str)
    {
        std::string result = str;
        for (char &c : result)
        {
            c = std::tolower(c);
        }
        return result;
    };

    std::string modelo_lower = to_lower(modelo);

    for (size_t i = 0; i < this->veiculos.size(); i++)
    {
        std::string veiculo_modelo = to_lower(this->veiculos[i]->getModelo());

        if (veiculo_modelo == modelo_lower)
        {
            // Encontrou, remover
            delete this->veiculos[i];
            this->veiculos.erase(this->veiculos.begin() + i);

            std::cout << "Veículo removido com sucesso! Modelo: " << modelo << std::endl;
            return true;
        }
    }

    std::cerr << "Erro: Veículo com modelo '" << modelo << "' não encontrado!" << std::endl;
    return false;
}

// NOVO: Obter total de veículos
int GerenciadorDeVeiculos::getTotalVeiculos()
{
    return this->veiculos.size();
}

GerenciadorDeClientes::GerenciadorDeClientes(const string &arquivo)
{
    this->nome_arquivo = arquivo;
}

// Método Singleton
GerenciadorDeClientes &GerenciadorDeClientes::getInstance(const string &arquivo)
{
    static GerenciadorDeClientes instancia(arquivo);
    return instancia;
}

void GerenciadorDeClientes::carregar_do_csv()
{
    std::ifstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }

    std::string linha;
    int linha_num = 0;

    // Pular cabeçalho
    if (std::getline(arquivo, linha))
    {
        linha_num++;
    }

    while (std::getline(arquivo, linha))
    {
        linha_num++;

        // Ignorar linhas vazias
        if (linha.empty() || linha.find_first_not_of(" \t\r\n") == std::string::npos)
        {
            continue;
        }

        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> dados;

        while (std::getline(ss, campo, ','))
        {
            // Remover espaços em branco
            campo.erase(0, campo.find_first_not_of(" \t\r\n"));
            campo.erase(campo.find_last_not_of(" \t\r\n") + 1);
            dados.push_back(campo);
        }

        // Validar dados (esperamos 4 campos: Nome, CPF, Telefone, Endereço)
        if (dados.size() >= 4)
        {
            try
            {
                Clientes *novoCliente = new Clientes(dados[0], dados[1], dados[2], dados[3]);

                // CORREÇÃO: push_back do PONTEIRO, não do objeto
                this->clientes.push_back(novoCliente); // SEM asterisco aqui!

                std::cout << "Cliente carregado: " << dados[0] << ", CPF: " << dados[1] << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Erro ao criar cliente na linha " << linha_num << ": " << e.what() << std::endl;
            }
        }
        else
        {
            std::cerr << "Linha " << linha_num << " com número incorreto de campos: " << dados.size() << std::endl;
        }
    }
    arquivo.close();
    std::cout << "Carregamento de clientes concluído. Total: " << clientes.size() << " clientes" << std::endl;
}

void GerenciadorDeClientes::salvar_no_csv()
{
    std::ofstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }

    // Escreve cabeçalho
    arquivo << "Nome,Cpf,Telefone,Endereco\n";

    // Escreve cada cliente
    for (size_t i = 0; i < this->clientes.size(); i++)
    {
        arquivo << clientes[i]->getNome() << ","
                << clientes[i]->getCpf() << ","
                << clientes[i]->getTelefone() << ","
                << clientes[i]->getEndereco() << "\n";
    }

    arquivo.close();
    std::cout << "Arquivo salvo com sucesso: " << this->nome_arquivo
              << ". Total de clientes: " << clientes.size() << std::endl;
}

vector<Clientes *> GerenciadorDeClientes::listar()
{
    return this->clientes;
}

vector<Clientes *> GerenciadorDeClientes::buscar(string termo_busca)
{
    vector<Clientes *> resposta;

    // Função helper para converter para minúsculas
    auto to_lower = [](const std::string &str)
    {
        std::string result = str;
        for (char &c : result)
        {
            c = std::tolower(c);
        }
        return result;
    };

    std::string termo_lower = to_lower(termo_busca);

    for (size_t i = 0; i < this->clientes.size(); i++)
    {
        std::string nome = to_lower(this->clientes[i]->getNome());
        std::string cpf = to_lower(this->clientes[i]->getCpf());

        // Buscar por substring no nome ou CPF
        if (nome.find(termo_lower) != std::string::npos ||
            cpf.find(termo_lower) != std::string::npos)
        {
            resposta.push_back(this->clientes[i]);
        }
    }

    return resposta;
}

void GerenciadorDeClientes::adicionar(Clientes *novo_cliente)
{
    // Verificar se CPF já existe
    for (size_t i = 0; i < this->clientes.size(); i++)
    {
        if (this->clientes[i]->getCpf() == novo_cliente->getCpf())
        {
            std::cerr << "Erro: Cliente com CPF " << novo_cliente->getCpf()
                      << " já existe!" << std::endl;
            return;
        }
    }

    // CORREÇÃO: push_back do PONTEIRO
    this->clientes.push_back(novo_cliente); // SEM asterisco aqui!
    std::cout << "Cliente adicionado com sucesso: " << novo_cliente->getNome() << std::endl;
}

bool GerenciadorDeClientes::remover(int indice)
{
    if (indice < 0 || indice >= (int)this->clientes.size())
    {
        std::cerr << "Erro: Índice " << indice << " inválido!" << std::endl;
        return false;
    }

    std::string nome_removido = this->clientes[indice]->getNome();

    // Deletar o objeto da memória
    delete this->clientes[indice];

    // Remover do vetor
    this->clientes.erase(this->clientes.begin() + indice);

    std::cout << "Cliente removido com sucesso! Nome: " << nome_removido << std::endl;
    return true;
}

bool GerenciadorDeClientes::remover_por_cpf(string cpf_procurado)
{
    for (size_t i = 0; i < this->clientes.size(); i++)
    {
        if (this->clientes[i]->getCpf() == cpf_procurado)
        {
            std::string nome_removido = this->clientes[i]->getNome();

            // Encontrou, remover
            delete this->clientes[i];
            this->clientes.erase(this->clientes.begin() + i);

            std::cout << "Cliente removido com sucesso! CPF: " << cpf_procurado
                      << " - Nome: " << nome_removido << std::endl;
            return true;
        }
    }

    std::cerr << "Erro: Cliente com CPF '" << cpf_procurado << "' não encontrado!" << std::endl;
    return false;
}

Clientes *GerenciadorDeClientes::buscarPorCpf(string cpf_busca)
{
    for (size_t i = 0; i < this->clientes.size(); i++)
    {
        if (this->clientes[i]->getCpf() == cpf_busca)
        {
            return this->clientes[i]; // Retorna o cliente encontrado
        }
    }
    return nullptr; // Retorna nullptr se não encontrar
}
int GerenciadorDeClientes::getTotalCliente()
{
    return this->clientes.size();
}

GerenciadorDeVendedores &GerenciadorDeVendedores::getInstance(const string &arquivo)
{
    static GerenciadorDeVendedores instancia(arquivo);
    return instancia;
}
GerenciadorDeVendedores::GerenciadorDeVendedores(const string &arquivo)
{
    this->nome_arquivo = arquivo;
}

// Método Singleton

void GerenciadorDeVendedores::carregar_do_csv()
{
    std::ifstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }

    std::string linha;
    int linha_num = 0;

    // Pular cabeçalho
    if (std::getline(arquivo, linha))
    {
        linha_num++;
    }

    while (std::getline(arquivo, linha))
    {
        linha_num++;

        // Ignorar linhas vazias
        if (linha.empty() || linha.find_first_not_of(" \t\r\n") == std::string::npos)
        {
            continue;
        }

        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> dados;

        while (std::getline(ss, campo, ','))
        {
            // Remover espaços em branco
            campo.erase(0, campo.find_first_not_of(" \t\r\n"));
            campo.erase(campo.find_last_not_of(" \t\r\n") + 1);
            dados.push_back(campo);
        }

        // Validar dados (esperamos 6 campos: Nome, CPF, Telefone, Endereço, Email, Senha)
        if (dados.size() >= 6)
        {
            try
            {
                Vendedor *novoVendedor = new Vendedor(dados[0], dados[1], dados[2], dados[3], dados[4], dados[5]);

                this->vendedores.push_back(novoVendedor);

                std::cout << "Vendedor carregado: " << dados[0] << ", CPF: " << dados[1]
                          << ", Email: " << dados[4] << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Erro ao criar vendedor na linha " << linha_num << ": " << e.what() << std::endl;
            }
        }
        else
        {
            std::cerr << "Linha " << linha_num << " com número incorreto de campos: " << dados.size() << std::endl;
        }
    }
    arquivo.close();
    std::cout << "Carregamento de vendedores concluído. Total: " << vendedores.size() << " vendedores" << std::endl;
}

void GerenciadorDeVendedores::salvar_no_csv()
{
    std::ofstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }

    // Escreve cabeçalho
    arquivo << "Nome,Cpf,Telefone,Endereco,Email,Senha\n";

    // Escreve cada vendedor
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        arquivo << vendedores[i]->getNome() << ","
                << vendedores[i]->getCpf() << ","
                << vendedores[i]->getTelefone() << ","
                << vendedores[i]->getEndereco() << ","
                << vendedores[i]->getEmail() << ","
                << vendedores[i]->getSenha() << "\n";
    }

    arquivo.close();
    std::cout << "Arquivo salvo com sucesso: " << this->nome_arquivo
              << ". Total de vendedores: " << vendedores.size() << std::endl;
}

vector<Vendedor *> GerenciadorDeVendedores::listar()
{
    return this->vendedores;
}

vector<Vendedor *> GerenciadorDeVendedores::buscar(string termo_busca)
{
    vector<Vendedor *> resposta;

    // Função helper para converter para minúsculas
    auto to_lower = [](const std::string &str)
    {
        std::string result = str;
        for (char &c : result)
        {
            c = std::tolower(c);
        }
        return result;
    };

    std::string termo_lower = to_lower(termo_busca);

    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        std::string nome = to_lower(this->vendedores[i]->getNome());
        std::string cpf = to_lower(this->vendedores[i]->getCpf());
        std::string email = to_lower(this->vendedores[i]->getEmail());

        // Buscar por substring no nome, CPF ou email
        if (nome.find(termo_lower) != std::string::npos ||
            cpf.find(termo_lower) != std::string::npos ||
            email.find(termo_lower) != std::string::npos)
        {
            resposta.push_back(this->vendedores[i]);
        }
    }

    return resposta;
}

void GerenciadorDeVendedores::adicionar(Vendedor *novo_vendedor)
{
    // Verificar se CPF já existe
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getCpf() == novo_vendedor->getCpf())
        {
            std::cerr << "Erro: Vendedor com CPF " << novo_vendedor->getCpf()
                      << " já existe!" << std::endl;
            return;
        }
    }

    // Verificar se email já existe
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getEmail() == novo_vendedor->getEmail())
        {
            std::cerr << "Erro: Email " << novo_vendedor->getEmail()
                      << " já está em uso!" << std::endl;
            return;
        }
    }

    this->vendedores.push_back(novo_vendedor);
    std::cout << "Vendedor adicionado com sucesso: " << novo_vendedor->getNome()
              << " - Email: " << novo_vendedor->getEmail() << std::endl;
}

bool GerenciadorDeVendedores::remover(int indice)
{
    if (indice < 0 || indice >= (int)this->vendedores.size())
    {
        std::cerr << "Erro: Índice " << indice << " inválido!" << std::endl;
        return false;
    }

    std::string nome_removido = this->vendedores[indice]->getNome();

    // Deletar o objeto da memória
    delete this->vendedores[indice];

    // Remover do vetor
    this->vendedores.erase(this->vendedores.begin() + indice);

    std::cout << "Vendedor removido com sucesso! Nome: " << nome_removido << std::endl;
    return true;
}

bool GerenciadorDeVendedores::remover_por_cpf(string cpf_procurado)
{
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getCpf() == cpf_procurado)
        {
            std::string nome_removido = this->vendedores[i]->getNome();

            // Encontrou, remover
            delete this->vendedores[i];
            this->vendedores.erase(this->vendedores.begin() + i);

            std::cout << "Vendedor removido com sucesso! CPF: " << cpf_procurado
                      << " - Nome: " << nome_removido << std::endl;
            return true;
        }
    }

    std::cerr << "Erro: Vendedor com CPF '" << cpf_procurado << "' não encontrado!" << std::endl;
    return false;
}

int GerenciadorDeVendedores::getTotalVendedor()
{
    return this->vendedores.size();
}

// MÉTODO DE AUTENTICAÇÃO (agora no gerenciador)
bool GerenciadorDeVendedores::autenticar(string email, string senha, Vendedor **vendedor_autenticado)
{
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getEmail() == email &&
            this->vendedores[i]->getSenha() == senha)
        {

            // Autenticação bem-sucedida
            *vendedor_autenticado = this->vendedores[i]; // Atribui o vendedor encontrado
            std::cout << "Autenticação bem-sucedida! Vendedor: "
                      << this->vendedores[i]->getNome() << std::endl;
            return true;
        }
    }

    // Autenticação falhou
    *vendedor_autenticado = nullptr; // Garante que o ponteiro seja nulo
    std::cerr << "Falha na autenticação! Email ou senha incorretos." << std::endl;
    return false;
}

Vendedor *GerenciadorDeVendedores::buscarPorCpf(string cpf_busca)
{
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getCpf() == cpf_busca)
        {
            return this->vendedores[i]; // Retorna o vendedor encontrado
        }
    }
    return nullptr; // Retorna nullptr se não encontrar
}

// VERIFICAR SE EMAIL ESTÁ DISPONÍVEL (agora no gerenciador)
bool GerenciadorDeVendedores::verificarEmailDisponivel(string email)
{
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getEmail() == email)
        {
            return false; // Email já existe
        }
    }
    return true; // Email disponível
}

GereniciadorDeVendas::GereniciadorDeVendas(const string &arquivo)
{
    this->nome_arquivo = arquivo;
}
// Método Singleton
GereniciadorDeVendas &GereniciadorDeVendas::getInstance(const string &arquivo)
{
    static GereniciadorDeVendas instancia(arquivo);
    return instancia;
}

void GereniciadorDeVendas::carregar_do_csv() {
    std::ifstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open()) {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }

    std::string linha;
    int linha_num = 0;

    // Pular cabeçalho
    if (std::getline(arquivo, linha)) {
        linha_num++;
    }

    while (std::getline(arquivo, linha)) {
        linha_num++;

        if (linha.empty() || linha.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> dados;

        while (std::getline(ss, campo, ',')) {
            campo.erase(0, campo.find_first_not_of(" \t\r\n"));
            campo.erase(campo.find_last_not_of(" \t\r\n") + 1);
            dados.push_back(campo);
        }

        // ATUALIZADO: Agora espera 13 campos (com Data)
        if (dados.size() >= 13) {
            try {
                GerenciadorDeClientes &gerenciadorClientes = GerenciadorDeClientes::getInstance();
                GerenciadorDeVendedores &gerenciadorVendedores = GerenciadorDeVendedores::getInstance();
                
                Clientes* Cliente_venda = gerenciadorClientes.buscarPorCpf(dados[9]); // CPF Cliente
                Vendedor* Vendedor_venda = gerenciadorVendedores.buscarPorCpf(dados[2]); // CPF Vendedor
                PoliticaDesconto* desconto_venda = FabricaPoliticasDesconto::criarPolitica(dados[10]);
                
                float valor_final = std::stof(dados[5]);
                float valor_entrada = std::stof(dados[6]);
                string forma_de_pagamento = dados[11];
                string status_vendas = dados[12];
                
                // NOVO: Carregar data usando DataUtils
                Data data_venda = DataUtils::fromString(dados[13]);
                
                Veiculos* Veiculo_venda = nullptr;
                if (dados[0] == "Moto") {
                    Veiculo_venda = new Moto(dados[3], std::stoi(dados[4]), valor_final, dados[7]);
                } else if (dados[0] == "Carro") {
                    Veiculo_venda = new Carro(dados[3], std::stoi(dados[4]), valor_final, dados[7]);
                }
                
                // NOVO: Passar data para o construtor
                Vendas *nova_Venda = new Vendas(Vendedor_venda, Cliente_venda, Veiculo_venda, 
                                               valor_final, desconto_venda, valor_entrada, 
                                               forma_de_pagamento, status_vendas, data_venda);
                this->vendas.push_back(nova_Venda);
                
            } catch (const std::exception &e) {
                std::cerr << "Erro ao criar venda na linha " << linha_num << ": " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Linha " << linha_num << " com número incorreto de campos: " << dados.size() << std::endl;
        }
    }
    arquivo.close();
    std::cout << "Carregamento de vendas concluído. Total: " << vendas.size() << " vendas" << std::endl;
}

void GereniciadorDeVendas::salvar_no_csv()
{
    std::ofstream arquivo(this->nome_arquivo);
    if (!arquivo.is_open())
    {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }

    // Escreve cabeçalho
    arquivo << "Tipo,Vendedor,CPF Vendedor,Modelo,Ano,Valor Pago,Valor Entrada,Cor,Cliente,CPF Cliente,Politica Desconto,Forma Pagamento,Status Venda\n";

    // Escreve cada venda
    for (const auto &venda : this->vendas)
    {
        arquivo << venda->getVeiculo()->motoOuCarro() << ","
                << venda->getVendedor()->getNome() << ","
                << venda->getVendedor()->getCpf() << ","
                << venda->getVeiculo()->getModelo() << ","
                << venda->getVeiculo()->getAno() << ","
                << venda->getValorFinal() << ","
                << venda->getValorEntrada() << ","
                << venda->getVeiculo()->getCor() << ","
                << venda->getCliente()->getNome() << ","
                << venda->getCliente()->getCpf() << ","
                << venda->getPoliticaDesconto()->getNome() << ","
                << venda->getFormaPagamento() << ","
                << venda->getStatusVenda() << "\n";
    }

    arquivo.close();
    std::cout << "Vendas salvas com sucesso: " << this->nome_arquivo << std::endl;
}

vector<Vendas *> GereniciadorDeVendas::listar()
{
    return this->vendas;
}

void GereniciadorDeVendas::adicionar(Vendas *nova_venda)
{
    this->vendas.push_back(nova_venda);
}

bool GereniciadorDeVendas::verificarConsistenciaDados() {
    GerenciadorDeClientes &gerenciadorClientes = GerenciadorDeClientes::getInstance();
    GerenciadorDeVendedores &gerenciadorVendedores = GerenciadorDeVendedores::getInstance();
    
    bool consistente = true;
    
    for (const auto &venda : this->vendas) {
        // Verificar se cliente existe
        Clientes* cliente = gerenciadorClientes.buscarPorCpf(venda->getCliente()->getCpf());
        if (cliente == nullptr) {
            std::cerr << "ERRO: Cliente não encontrado - CPF: " << venda->getCliente()->getCpf() << std::endl;
            consistente = false;
        }
        
        // Verificar se vendedor existe
        Vendedor* vendedor = gerenciadorVendedores.buscarPorCpf(venda->getVendedor()->getCpf());
        if (vendedor == nullptr) {
            std::cerr << "ERRO: Vendedor não encontrado - CPF: " << venda->getVendedor()->getCpf() << std::endl;
            consistente = false;
        }
    }
    
    if (consistente) {
        std::cout << "✓ Todos os dados estão consistentes!" << std::endl;
    } else {
        std::cout << "✗ Foram encontrados erros de consistência nos dados!" << std::endl;
    }
    
    return consistente;
}


int GereniciadorDeVendas::getTotalVendas()
{
    return this->vendas.size();
}
