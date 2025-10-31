#include "gerenciadores.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm> // ADICIONAR ESTE INCLUDE
#include <cctype>    // ADICIONAR PARA ::tolower

// Definição do construtor
GerenciadorDeVeiculos::GerenciadorDeVeiculos(const string &arquivo) { 
    this->nome_arquivo = arquivo; 
}

// Definição do método estático getInstance
GerenciadorDeVeiculos& GerenciadorDeVeiculos::getInstance(const string& arquivo) { 
    static GerenciadorDeVeiculos instancia(arquivo); 
    return instancia; 
}

void GerenciadorDeVeiculos::adicionar(Veiculos *novo_veiculo) {
    this->veiculos.push_back(novo_veiculo);
}

std::string trim(const std::string &str) {
    size_t inicio = str.find_first_not_of(" \t\r\n");
    size_t fim = str.find_last_not_of(" \t\r\n");

    if (inicio == std::string::npos) {
        return "";
    }

    return str.substr(inicio, fim - inicio + 1);
}

void GerenciadorDeVeiculos::carregar_do_csv() {
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

        // Ignorar linhas vazias
        if (trim(linha).empty()) {
            continue;
        }

        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> dados;

        while (std::getline(ss, campo, ',')) {
            dados.push_back(trim(campo));
        }

        // Validar dados - agora com tratamento adequado
        if (dados.size() >= 5) {
            try {
                Veiculos *novoVeiculo = nullptr; 
                std::string tipo = dados[0];

                if (tipo == "Moto") {
                    novoVeiculo = new Moto(dados[1], std::stoi(dados[2]),
                                           std::stof(dados[3]), dados[4]);
                }
                else if (tipo == "Carro") {
                    novoVeiculo = new Carro(dados[1], std::stoi(dados[2]),
                                            std::stof(dados[3]), dados[4]);
                }
                else {
                    std::cerr << "Tipo de veículo desconhecido: " << tipo 
                              << " na linha " << linha_num << std::endl;
                    continue;
                }

                if (novoVeiculo != nullptr) {
                    veiculos.push_back(novoVeiculo);
                    std::cout << "Veículo carregado: " << tipo << ", "
                              << dados[1] << ", " << dados[2] << std::endl;
                }
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Erro ao converter dados numéricos na linha " 
                          << linha_num << ": " << e.what() << std::endl;
            }
            catch (const std::out_of_range &e) {
                std::cerr << "Número fora do intervalo válido na linha " 
                          << linha_num << ": " << e.what() << std::endl;
            }
            catch (const std::exception &e) {
                std::cerr << "Erro ao criar veículo na linha " 
                          << linha_num << ": " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Linha " << linha_num 
                      << " com número incorreto de campos: " << dados.size() << std::endl;
        }
    }
    arquivo.close();
    std::cout << "Carregamento concluído. Total de veículos: " 
              << veiculos.size() << std::endl;
}

void GerenciadorDeVeiculos::salvar_no_csv() {
    std::ofstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open()) {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }
    
    // Escreve cabeçalho
    arquivo << "Tipo,Modelo,Ano,Valor (R$),Cor\n";
    
    // Escreve cada veículo
    for (const auto& veiculo : this->veiculos) {
        // CORREÇÃO: Usar "Moto" e "Carro" em vez de "moto" e "carro"
        std::string tipo = veiculo->motoOuCarro();
        if (tipo == "moto") tipo = "Moto";
        else if (tipo == "carro") tipo = "Carro";
        
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

vector<Veiculos*> GerenciadorDeVeiculos::listar() {
    return this->veiculos;
}

vector<Veiculos*> GerenciadorDeVeiculos::buscar(string termo_busca) {
    vector<Veiculos*> resposta;
    
    // Converter termo de busca para minúsculas para busca case-insensitive
    std::string termo_lower = termo_busca;
    std::transform(termo_lower.begin(), termo_lower.end(), termo_lower.begin(), ::tolower);
    
    for (size_t i = 0; i < this->veiculos.size(); i++) {
        std::string modelo = this->veiculos[i]->getModelo();
        
        // Converter modelo para minúsculas para busca case-insensitive
        std::string modelo_lower = modelo;
        std::transform(modelo_lower.begin(), modelo_lower.end(), modelo_lower.begin(), ::tolower);
        
        // Buscar por substring (agora encontra "Fusion" em "Ford Fusion")
        if (modelo_lower.find(termo_lower) != std::string::npos) {
            resposta.push_back(this->veiculos[i]);
        }
    }
    
    return resposta;
}