#include "gerenciadores.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <QDebug>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>

GerenciadorDeVeiculos::GerenciadorDeVeiculos(const string &arquivo)
{
    this->nome_arquivo = arquivo;
}

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
        qWarning() << "Erro: não foi possível abrir " << this->nome_arquivo.c_str();
        return;
    }

    std::string linha;
    int linha_num = 0;

    if (std::getline(arquivo, linha))
    {
        linha_num++;
    }

    while (std::getline(arquivo, linha))
    {
        linha_num++;

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
                    qWarning() << "Tipo de veículo desconhecido: " << tipo.c_str()
                               << " na linha " << linha_num;
                    continue;
                }

                if (novoVeiculo != nullptr)
                {
                    veiculos.push_back(novoVeiculo);
                    qDebug() << "Veículo carregado: " << tipo.c_str() << ", "
                             << dados[1].c_str() << ", " << dados[2].c_str();
                }
            }
            catch (const std::invalid_argument &e)
            {
                qWarning() << "Erro ao converter dados numéricos na linha "
                           << linha_num << ": " << e.what();
            }
            catch (const std::out_of_range &e)
            {
                qWarning() << "Número fora do intervalo válido na linha "
                           << linha_num << ": " << e.what();
            }
            catch (const std::exception &e)
            {
                qWarning() << "Erro ao criar veículo na linha "
                           << linha_num << ": " << e.what();
            }
        }
        else
        {
            qWarning() << "Linha " << linha_num
                       << " com número incorreto de campos: " << dados.size();
        }
    }
    arquivo.close();
    qDebug() << "Carregamento concluído. Total de veículos: "
             << veiculos.size();
}

void GerenciadorDeVeiculos::salvar_no_csv()
{
    std::ofstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        qWarning() << "Erro: não foi possível abrir " << this->nome_arquivo.c_str();
        return;
    }

    arquivo << "Tipo,Modelo,Ano,Valor (R$),Cor\n";

    for (const auto &veiculo : this->veiculos)
    {
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
    qDebug() << "Arquivo salvo com sucesso: " << this->nome_arquivo.c_str()
             << ". Total de veículos: " << veiculos.size();
}

vector<Veiculos *> GerenciadorDeVeiculos::listar()
{
    return this->veiculos;
}

vector<Veiculos *> GerenciadorDeVeiculos::buscar(string termo_busca)
{
    vector<Veiculos *> resposta;

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

        if (modelo_lower.find(termo_lower) != std::string::npos)
        {
            resposta.push_back(this->veiculos[i]);
        }
    }

    return resposta;
}

bool GerenciadorDeVeiculos::remover(int indice)
{
    if (indice < 0 || indice >= (int)this->veiculos.size())
    {
        qWarning() << "Erro: Índice " << indice << " inválido!";
        return false;
    }

    delete this->veiculos[indice];
    this->veiculos.erase(this->veiculos.begin() + indice);

    qDebug() << "Veículo removido com sucesso! Índice: " << indice;
    return true;
}

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
            delete this->veiculos[i];
            this->veiculos.erase(this->veiculos.begin() + i);

            qDebug() << "Veículo removido com sucesso! Modelo: " << modelo.c_str();
            return true;
        }
    }

    qWarning() << "Erro: Veículo com modelo '" << modelo.c_str() << "' não encontrado!";
    return false;
}

int GerenciadorDeVeiculos::getTotalVeiculos()
{
    return this->veiculos.size();
}

GerenciadorDeClientes::GerenciadorDeClientes(const string &arquivo)
{
    this->nome_arquivo = arquivo;
}

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
        qWarning() << "Erro: não foi possível abrir " << this->nome_arquivo.c_str();
        return;
    }

    std::string linha;
    int linha_num = 0;

    if (std::getline(arquivo, linha))
    {
        linha_num++;
    }

    while (std::getline(arquivo, linha))
    {
        linha_num++;

        if (linha.empty() || linha.find_first_not_of(" \t\r\n") == std::string::npos)
        {
            continue;
        }

        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> dados;

        while (std::getline(ss, campo, ','))
        {
            campo.erase(0, campo.find_first_not_of(" \t\r\n"));
            campo.erase(campo.find_last_not_of(" \t\r\n") + 1);
            dados.push_back(campo);
        }

        if (dados.size() >= 4)
        {
            try
            {
                Clientes *novoCliente = new Clientes(dados[0], dados[1], dados[2], dados[3]);
                this->clientes.push_back(novoCliente);

                qDebug() << "Cliente carregado: " << dados[0].c_str() << ", CPF: " << dados[1].c_str();
            }
            catch (const std::exception &e)
            {
                qWarning() << "Erro ao criar cliente na linha " << linha_num << ": " << e.what();
            }
        }
        else
        {
            qWarning() << "Linha " << linha_num << " com número incorreto de campos: " << dados.size();
        }
    }
    arquivo.close();
    qDebug() << "Carregamento de clientes concluído. Total: " << clientes.size() << " clientes";
}

void GerenciadorDeClientes::salvar_no_csv()
{
    std::ofstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        qWarning() << "Erro: não foi possível abrir " << this->nome_arquivo.c_str();
        return;
    }

    arquivo << "Nome,Cpf,Telefone,Endereco\n";

    for (size_t i = 0; i < this->clientes.size(); i++)
    {
        arquivo << clientes[i]->getNome() << ","
                << clientes[i]->getCpf() << ","
                << clientes[i]->getTelefone() << ","
                << clientes[i]->getEndereco() << "\n";
    }

    arquivo.close();
    qDebug() << "Arquivo salvo com sucesso: " << this->nome_arquivo.c_str()
             << ". Total de clientes: " << clientes.size();
}

vector<Clientes *> GerenciadorDeClientes::listar()
{
    return this->clientes;
}

vector<Clientes *> GerenciadorDeClientes::buscar(string termo_busca)
{
    vector<Clientes *> resposta;

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
    for (size_t i = 0; i < this->clientes.size(); i++)
    {
        if (this->clientes[i]->getCpf() == novo_cliente->getCpf())
        {
            qWarning() << "Erro: Cliente com CPF " << novo_cliente->getCpf().c_str()
            << " já existe!";
            return;
        }
    }

    this->clientes.push_back(novo_cliente);
    qDebug() << "Cliente adicionado com sucesso: " << novo_cliente->getNome().c_str();
}

bool GerenciadorDeClientes::remover(int indice)
{
    if (indice < 0 || indice >= (int)this->clientes.size())
    {
        qWarning() << "Erro: Índice " << indice << " inválido!";
        return false;
    }

    std::string nome_removido = this->clientes[indice]->getNome();
    delete this->clientes[indice];
    this->clientes.erase(this->clientes.begin() + indice);

    qDebug() << "Cliente removido com sucesso! Nome: " << nome_removido.c_str();
    return true;
}

bool GerenciadorDeClientes::remover_por_cpf(string cpf_procurado)
{
    for (size_t i = 0; i < this->clientes.size(); i++)
    {
        if (this->clientes[i]->getCpf() == cpf_procurado)
        {
            std::string nome_removido = this->clientes[i]->getNome();
            delete this->clientes[i];
            this->clientes.erase(this->clientes.begin() + i);

            qDebug() << "Cliente removido com sucesso! CPF: " << cpf_procurado.c_str()
                     << " - Nome: " << nome_removido.c_str();
            return true;
        }
    }

    qWarning() << "Erro: Cliente com CPF '" << cpf_procurado.c_str() << "' não encontrado!";
    return false;
}

Clientes *GerenciadorDeClientes::buscarPorCpf(string cpf_busca)
{
    for (size_t i = 0; i < this->clientes.size(); i++)
    {
        if (this->clientes[i]->getCpf() == cpf_busca)
        {
            return this->clientes[i];
        }
    }
    return nullptr;
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

void GerenciadorDeVendedores::carregar_do_csv()
{
    std::ifstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        qWarning() << "Erro: não foi possível abrir " << this->nome_arquivo.c_str();
        return;
    }

    std::string linha;
    int linha_num = 0;

    if (std::getline(arquivo, linha))
    {
        linha_num++;
    }

    while (std::getline(arquivo, linha))
    {
        linha_num++;

        if (linha.empty() || linha.find_first_not_of(" \t\r\n") == std::string::npos)
        {
            continue;
        }

        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> dados;

        while (std::getline(ss, campo, ','))
        {
            campo.erase(0, campo.find_first_not_of(" \t\r\n"));
            campo.erase(campo.find_last_not_of(" \t\r\n") + 1);
            dados.push_back(campo);
        }

        if (dados.size() >= 6)
        {
            try
            {
                Vendedor *novoVendedor = new Vendedor(dados[0], dados[1], dados[2], dados[3], dados[4], dados[5]);
                this->vendedores.push_back(novoVendedor);

                qDebug() << "Vendedor carregado: " << dados[0].c_str() << ", CPF: " << dados[1].c_str()
                         << ", Email: " << dados[4].c_str();
            }
            catch (const std::exception &e)
            {
                qWarning() << "Erro ao criar vendedor na linha " << linha_num << ": " << e.what();
            }
        }
        else
        {
            qWarning() << "Linha " << linha_num << " com número incorreto de campos: " << dados.size();
        }
    }
    arquivo.close();
    qDebug() << "Carregamento de vendedores concluído. Total: " << vendedores.size() << " vendedores";
}

void GerenciadorDeVendedores::salvar_no_csv()
{
    std::ofstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open())
    {
        qWarning() << "Erro: não foi possível abrir " << this->nome_arquivo.c_str();
        return;
    }

    arquivo << "Nome,Cpf,Telefone,Endereco,Email,Senha\n";

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
    qDebug() << "Arquivo salvo com sucesso: " << this->nome_arquivo.c_str()
             << ". Total de vendedores: " << vendedores.size();
}

vector<Vendedor *> GerenciadorDeVendedores::listar()
{
    return this->vendedores;
}

vector<Vendedor *> GerenciadorDeVendedores::buscar(string termo_busca)
{
    vector<Vendedor *> resposta;

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
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getCpf() == novo_vendedor->getCpf())
        {
            qWarning() << "Erro: Vendedor com CPF " << novo_vendedor->getCpf().c_str()
            << " já existe!";
            return;
        }
    }

    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getEmail() == novo_vendedor->getEmail())
        {
            qWarning() << "Erro: Email " << novo_vendedor->getEmail().c_str()
            << " já está em uso!";
            return;
        }
    }

    this->vendedores.push_back(novo_vendedor);
    qDebug() << "Vendedor adicionado com sucesso: " << novo_vendedor->getNome().c_str()
             << " - Email: " << novo_vendedor->getEmail().c_str();
}

bool GerenciadorDeVendedores::remover(int indice)
{
    if (indice < 0 || indice >= (int)this->vendedores.size())
    {
        qWarning() << "Erro: Índice " << indice << " inválido!";
        return false;
    }

    std::string nome_removido = this->vendedores[indice]->getNome();
    delete this->vendedores[indice];
    this->vendedores.erase(this->vendedores.begin() + indice);

    qDebug() << "Vendedor removido com sucesso! Nome: " << nome_removido.c_str();
    return true;
}

bool GerenciadorDeVendedores::remover_por_cpf(string cpf_procurado)
{
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getCpf() == cpf_procurado)
        {
            std::string nome_removido = this->vendedores[i]->getNome();
            delete this->vendedores[i];
            this->vendedores.erase(this->vendedores.begin() + i);

            qDebug() << "Vendedor removido com sucesso! CPF: " << cpf_procurado.c_str()
                     << " - Nome: " << nome_removido.c_str();
            return true;
        }
    }

    qWarning() << "Erro: Vendedor com CPF '" << cpf_procurado.c_str() << "' não encontrado!";
    return false;
}

int GerenciadorDeVendedores::getTotalVendedor()
{
    return this->vendedores.size();
}

bool GerenciadorDeVendedores::autenticar(string email, string senha, Vendedor **vendedor_autenticado)
{
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getEmail() == email &&
            this->vendedores[i]->getSenha() == senha)
        {
            *vendedor_autenticado = this->vendedores[i];
            qDebug() << "Autenticação bem-sucedida! Vendedor: "
                     << this->vendedores[i]->getNome().c_str();
            return true;
        }
    }

    *vendedor_autenticado = nullptr;
    qWarning() << "Falha na autenticação! Email ou senha incorretos.";
    return false;
}

Vendedor *GerenciadorDeVendedores::buscarPorCpf(string cpf_busca)
{
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getCpf() == cpf_busca)
        {
            return this->vendedores[i];
        }
    }
    return nullptr;
}

bool GerenciadorDeVendedores::verificarEmailDisponivel(string email)
{
    for (size_t i = 0; i < this->vendedores.size(); i++)
    {
        if (this->vendedores[i]->getEmail() == email)
        {
            return false;
        }
    }
    return true;
}

GereniciadorDeVendas::GereniciadorDeVendas(const string &arquivo)
{
    this->nome_arquivo = arquivo;
}

GereniciadorDeVendas &GereniciadorDeVendas::getInstance(const string &arquivo)
{
    static GereniciadorDeVendas instancia(arquivo);
    return instancia;
}

void GereniciadorDeVendas::carregar_do_csv() {
    std::ifstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open()) {
        qWarning() << "Erro: não foi possível abrir " << this->nome_arquivo.c_str();
        return;
    }

    std::string linha;
    int linha_num = 0;

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

        if (dados.size() >= 14) {
            try {
                GerenciadorDeClientes &gerenciadorClientes = GerenciadorDeClientes::getInstance();
                GerenciadorDeVendedores &gerenciadorVendedores = GerenciadorDeVendedores::getInstance();

                Clientes* Cliente_venda = gerenciadorClientes.buscarPorCpf(dados[9]);
                Vendedor* Vendedor_venda = gerenciadorVendedores.buscarPorCpf(dados[2]);
                PoliticaDesconto* desconto_venda = FabricaPoliticasDesconto::criarPolitica(dados[10]);

                float valor_final = std::stof(dados[5]);
                float valor_entrada = std::stof(dados[6]);
                string forma_de_pagamento = dados[11];
                string status_vendas = dados[12];

                Data data_venda = DataUtils::fromString(dados[13]);

                Veiculos* Veiculo_venda = nullptr;
                if (dados[0] == "Moto") {
                    Veiculo_venda = new Moto(dados[3], std::stoi(dados[4]), valor_final, dados[7]);
                } else if (dados[0] == "Carro") {
                    Veiculo_venda = new Carro(dados[3], std::stoi(dados[4]), valor_final, dados[7]);
                }

                Vendas *nova_Venda = new Vendas(Vendedor_venda, Cliente_venda, Veiculo_venda,
                                                valor_final, desconto_venda, valor_entrada,
                                                forma_de_pagamento, status_vendas, data_venda);
                this->vendas.push_back(nova_Venda);

            } catch (const std::exception &e) {
                qWarning() << "Erro ao criar venda na linha " << linha_num << ": " << e.what();
            }
        } else {
            qWarning() << "Linha " << linha_num << " com número incorreto de campos: " << dados.size();
        }
    }
    arquivo.close();
    qDebug() << "Carregamento de vendas concluído. Total: " << vendas.size() << " vendas";
}

void GereniciadorDeVendas::salvar_no_csv()
{
    std::ofstream arquivo(this->nome_arquivo);
    if (!arquivo.is_open())
    {
        qWarning() << "Erro: não foi possível abrir " << this->nome_arquivo.c_str();
        return;
    }

    arquivo << "Tipo,Vendedor,CPF Vendedor,Modelo,Ano,Valor Pago,Valor Entrada,Cor,Cliente,CPF Cliente,Politica Desconto,Forma Pagamento,Status Venda\n";

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
    qDebug() << "Vendas salvas com sucesso: " << this->nome_arquivo.c_str();
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
        Clientes* cliente = gerenciadorClientes.buscarPorCpf(venda->getCliente()->getCpf());
        if (cliente == nullptr) {
            qWarning() << "ERRO: Cliente não encontrado - CPF: " << venda->getCliente()->getCpf().c_str();
            consistente = false;
        }

        Vendedor* vendedor = gerenciadorVendedores.buscarPorCpf(venda->getVendedor()->getCpf());
        if (vendedor == nullptr) {
            qWarning() << "ERRO: Vendedor não encontrado - CPF: " << venda->getVendedor()->getCpf().c_str();
            consistente = false;
        }
    }

    if (consistente) {
        qDebug() << "✓ Todos os dados estão consistentes!";
    } else {
        qDebug() << "✗ Foram encontrados erros de consistência nos dados!";
    }

    return consistente;
}


int GereniciadorDeVendas::getTotalVendas()
{
    return this->vendas.size();
}
