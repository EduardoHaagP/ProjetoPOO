#include "gerenciadores.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>


// Definição do construtor: NÃO repetir o default aqui
GerenciadorDeVeiculos::GerenciadorDeVeiculos(const string &arquivo) { this->nome_arquivo = arquivo; }

// Definição do método estático getInstance QUALIFICADO como membro da classe
GerenciadorDeVeiculos& GerenciadorDeVeiculos::getInstance(const string& arquivo) { static GerenciadorDeVeiculos instancia(arquivo); return instancia; }


void GerenciadorDeVeiculos::adicionar(Veiculos *novo_veiculo) {
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
            dados.push_back(trim(campo)); // Remove espaços
        }

        // Validar dados
        if (dados.size() == 5)
        {
            try
            {
                Veiculos *novoVeiculo = nullptr; 

                if (dados[0] == "Moto")
                {
                    novoVeiculo = new Moto(dados[1], std::stoi(dados[2]),
                                           std::stof(dados[3]), dados[4]);
                }
                else if (dados[0] == "Carro")
                {
                    novoVeiculo = new Carro(dados[1], std::stoi(dados[2]),
                                            std::stof(dados[3]), dados[4]);
                }

                if (novoVeiculo != nullptr)
                {
                    veiculos.push_back(novoVeiculo);
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Erro ao converter dados numéricos: " << e.what() << std::endl;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Número fora do intervalo válido: " << e.what() << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Erro ao criar veículo: " << e.what() << std::endl;
            }
        }try
        {
            std::cout << "Veículo carregado: " << dados[0] << ", "
                      << dados[1] << ", " << dados[2] << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erro na linha " << linha_num << ": " << e.what() << std::endl;
        }
    }
    arquivo.close();
}

void GerenciadorDeVeiculos::salvar_no_csv() {
    std::ofstream arquivo(this->nome_arquivo);

    if (!arquivo.is_open()) {
        std::cerr << "Erro: não foi possível abrir " << this->nome_arquivo << std::endl;
        return;
    }
    
    // Escreve cabeçalho
    arquivo << "Tipo,Modelo,Ano,Valor (R$),Cor\n";
    
    // Escreve cada veículo diretamente no arquivo
    for (const auto& veiculo : this->veiculos) {
        arquivo << veiculo->motoOuCarro() << ","
                << veiculo->getModelo() << ","
                << veiculo->getAno() << ","
                << std::fixed << std::setprecision(2) << veiculo->getValorBase() << ","
                << veiculo->getCor() << "\n";
    }
    
    arquivo.close();
    std::cout << "Arquivo salvo com sucesso: " << this->nome_arquivo << std::endl;
}

vector<Veiculos*> GerenciadorDeVeiculos::listar(){
    return  this->veiculos;
};

vector<Veiculos*> GerenciadorDeVeiculos::buscar(string nome_modelo) {
    vector<Veiculos*> resposta;
    for (size_t i = 0; i < this->veiculos.size(); i++)
    {
        // compare retorna 0 quando igual
        if (this->veiculos[i]->getModelo().compare(0, nome_modelo.size(), nome_modelo) == 0)
        {
            resposta.push_back(this->veiculos[i]);
        }
    }

    return resposta;
}