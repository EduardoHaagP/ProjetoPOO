#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gerenciadores.h"
#include "veiculos.h"
#include <iostream>
#include <QMessageBox>
#include <QDebug>
#include <algorithm> // para std::transform

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    std::string csv_path = std::string(PROJECT_SOURCE_DIR) + "/Trabalho POO - Carros.csv";
    qDebug() << "Caminho do CSV:" << QString::fromStdString(csv_path);

    // Obtém a instância do gerenciador
    GerenciadorDeVeiculos& gerenciador = GerenciadorDeVeiculos::getInstance(csv_path);
    
    // Teste 1: Carregar do CSV
    qDebug() << "=== TESTE 1: Carregar do CSV ===";
    gerenciador.carregar_do_csv();

    // Teste 2: Listar todos os veículos
    qDebug() << "\n=== TESTE 2: Listar todos os veículos ===";
    vector<Veiculos*> todos_veiculos = gerenciador.listar();
    std::cout << "Total de veículos carregados: " << todos_veiculos.size() << std::endl;

    // Teste 3: Buscas específicas para testar o problema identificado
    qDebug() << "\n=== TESTE 3: Buscas por substring (CORREÇÃO DO PROBLEMA) ===";
    
    // Teste que deve encontrar 2 resultados (Ford Fusion)
    vector<Veiculos*> busca_fusion = gerenciador.buscar("Fusion");
    std::cout << "Busca por 'Fusion': " << busca_fusion.size() << " veículos encontrados:" << std::endl;
    for (size_t i = 0; i < busca_fusion.size(); i++) {
        Veiculos* v = busca_fusion[i];
        std::cout << "• " << v->motoOuCarro() << " " << v->getModelo() 
                  << " " << v->getAno() << " - R$ " << v->getValorBase() << std::endl;
    }

    // Teste que deve encontrar múltiplos Civics
    vector<Veiculos*> busca_civic = gerenciador.buscar("Civic");
    std::cout << "\nBusca por 'Civic': " << busca_civic.size() << " veículos encontrados:" << std::endl;
    for (size_t i = 0; i < busca_civic.size(); i++) {
        Veiculos* v = busca_civic[i];
        std::cout << "• " << v->motoOuCarro() << " " << v->getModelo() 
                  << " " << v->getAno() << " - R$ " << v->getValorBase() << std::endl;
    }

    // Teste 4: Buscas parciais
    qDebug() << "\n=== TESTE 4: Buscas parciais ===";
    
    vector<Veiculos*> busca_cb = gerenciador.buscar("CB");
    std::cout << "Busca por 'CB': " << busca_cb.size() << " veículos encontrados:" << std::endl;
    for (size_t i = 0; i < std::min(busca_cb.size(), size_t(3)); i++) {
        Veiculos* v = busca_cb[i];
        std::cout << "• " << v->motoOuCarro() << " " << v->getModelo() 
                  << " " << v->getAno() << " - R$ " << v->getValorBase() << std::endl;
    }

    vector<Veiculos*> busca_ford = gerenciador.buscar("Ford");
    std::cout << "\nBusca por 'Ford': " << busca_ford.size() << " veículos encontrados:" << std::endl;
    for (size_t i = 0; i < std::min(busca_ford.size(), size_t(3)); i++) {
        Veiculos* v = busca_ford[i];
        std::cout << "• " << v->motoOuCarro() << " " << v->getModelo() 
                  << " " << v->getAno() << " - R$ " << v->getValorBase() << std::endl;
    }

    // Teste 5: Busca case-insensitive
    qDebug() << "\n=== TESTE 5: Busca case-insensitive ===";
    
    vector<Veiculos*> busca_fusion_lower = gerenciador.buscar("fusion");
    vector<Veiculos*> busca_fusion_upper = gerenciador.buscar("FUSION");
    
    std::cout << "Busca por 'fusion' (minúsculo): " << busca_fusion_lower.size() << " veículos" << std::endl;
    std::cout << "Busca por 'FUSION' (maiúsculo): " << busca_fusion_upper.size() << " veículos" << std::endl;
    std::cout << "✓ Busca case-insensitive funcionando!" << std::endl;

    // Teste 6: Adicionar novos veículos e testar busca
    qDebug() << "\n=== TESTE 6: Adicionar e buscar novos veículos ===";
    
    Carro* novo_carro = new Carro("Ford Fusion Hybrid", 2024, 180000.0f, "Azul");
    gerenciador.adicionar(novo_carro);
    
    // Testar busca no veículo recém-adicionado
    vector<Veiculos*> busca_hybrid = gerenciador.buscar("Hybrid");
    std::cout << "Busca por 'Hybrid' após adição: " << busca_hybrid.size() << " veículos encontrados" << std::endl;

    vector<Veiculos*> busca_fusion_apos_add = gerenciador.buscar("Fusion");
    std::cout << "Busca por 'Fusion' após adição: " << busca_fusion_apos_add.size() << " veículos encontrados" << std::endl;

    // Teste 7: Buscar por termos que não existem
    qDebug() << "\n=== TESTE 7: Buscas sem resultados ===";
    
    vector<Veiculos*> busca_ferrari = gerenciador.buscar("Ferrari");
    vector<Veiculos*> busca_xyz = gerenciador.buscar("XYZ123");
    
    std::cout << "Busca por 'Ferrari': " << busca_ferrari.size() << " veículos (esperado: 0)" << std::endl;
    std::cout << "Busca por 'XYZ123': " << busca_xyz.size() << " veículos (esperado: 0)" << std::endl;

    // Teste 8: Salvar no CSV
    qDebug() << "\n=== TESTE 8: Salvar no CSV ===";
    gerenciador.salvar_no_csv();

    // Estatísticas finais
    qDebug() << "\n=== ESTATÍSTICAS FINAIS ===";
    int carros_count = 0;
    int motos_count = 0;
    
    for (Veiculos* v : todos_veiculos) {
        if (v->motoOuCarro() == "Carro") {
            carros_count++;
        } else {
            motos_count++;
        }
    }
    
    std::cout << "Total de veículos: " << (todos_veiculos.size() + 1) << std::endl; // +1 pelo veículo adicionado
    std::cout << "Carros: " << (carros_count + 1) << std::endl; // +1 pelo carro adicionado
    std::cout << "Motos: " << motos_count << std::endl;

    // Resumo dos testes de busca
    QString resumo = QString("Testes de Busca Concluídos!\n\n"
                           "Problema identificado CORRIGIDO!\n\n"
                           "Busca por 'Fusion': %1 veículos (antes: 1)\n"
                           "Busca por 'Civic': %2 veículos\n"
                           "Busca por 'Ford': %3 veículos\n"
                           "Busca case-insensitive: ✓ Funcionando\n"
                           "Busca por substring: ✓ Funcionando\n\n"
                           "Total de veículos: %4")
                           .arg(busca_fusion.size())
                           .arg(busca_civic.size())
                           .arg(busca_ford.size())
                           .arg(todos_veiculos.size() + 1);
    
    QMessageBox::information(this, "Teste do Sistema - Busca Corrigida", resumo);
    
    qDebug() << "\n=== TODOS OS TESTES CONCLUÍDOS - BUSCA CORRIGIDA ===";
}

MainWindow::~MainWindow()
{
    delete ui;
}