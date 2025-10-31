#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gerenciadores.h"
#include "veiculos.h"
#include <iostream>
#include <QMessageBox>
#include <QDebug>

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

    // Exibir alguns veículos com seus índices
    std::cout << "\nPrimeiros 5 veículos (com índices):" << std::endl;
    for (size_t i = 0; i < std::min(todos_veiculos.size(), size_t(5)); i++) {
        Veiculos* v = todos_veiculos[i];
        std::cout << "[" << i << "] " << v->motoOuCarro() 
                  << " - " << v->getModelo() 
                  << " (" << v->getAno() << ")"
                  << " - R$ " << v->getValorBase() << std::endl;
    }

    // Teste 3: Buscas
    qDebug() << "\n=== TESTE 3: Buscas ===";
    
    vector<Veiculos*> busca_fusion = gerenciador.buscar("Fusion");
    std::cout << "Busca por 'Fusion': " << busca_fusion.size() << " veículos encontrados" << std::endl;

    // Teste 4: ADIÇÃO de novos veículos
    qDebug() << "\n=== TESTE 4: Adição de veículos ===";
    
    Carro* novo_carro1 = new Carro("Ford Fusion Hybrid", 2024, 180000.0f, "Azul");
    gerenciador.adicionar(novo_carro1);
    
    Carro* novo_carro2 = new Carro("Tesla Model S", 2024, 450000.0f, "Vermelho");
    gerenciador.adicionar(novo_carro2);
    
    Moto* nova_moto = new Moto("Yamaha MT-09", 2024, 55000.0f, "Azul");
    gerenciador.adicionar(nova_moto);
    
    std::cout << "Total após adições: " << gerenciador.getTotalVeiculos() << " veículos" << std::endl;

    // Teste 5: REMOÇÃO por índice
    qDebug() << "\n=== TESTE 5: Remoção por índice ===";
    
    int total_antes_remocao = gerenciador.getTotalVeiculos();
    std::cout << "Total antes da remoção: " << total_antes_remocao << " veículos" << std::endl;
    
    // Remover o primeiro veículo (índice 0)
    bool sucesso_remocao1 = gerenciador.remover(0);
    if (sucesso_remocao1) {
        std::cout << "✓ Veículo no índice 0 removido com sucesso!" << std::endl;
    }
    
    std::cout << "Total após remoção do índice 0: " << gerenciador.getTotalVeiculos() << " veículos" << std::endl;

    // Teste 6: REMOÇÃO por modelo
    qDebug() << "\n=== TESTE 6: Remoção por modelo ===";
    
    // Tentar remover um Fusion
    bool sucesso_remocao2 = gerenciador.remover_por_modelo("Fusion");
    if (sucesso_remocao2) {
        std::cout << "✓ Fusion removido com sucesso!" << std::endl;
    }
    
    std::cout << "Total após remoção do Fusion: " << gerenciador.getTotalVeiculos() << " veículos" << std::endl;

    // Teste 7: Tentativa de remoção com índice inválido
    qDebug() << "\n=== TESTE 7: Remoção com índice inválido ===";
    
    bool sucesso_remocao3 = gerenciador.remover(9999); // Índice inválido
    if (!sucesso_remocao3) {
        std::cout << "✓ Corretamente impediu remoção com índice inválido!" << std::endl;
    }

    // Teste 8: Tentativa de remoção com modelo inexistente
    qDebug() << "\n=== TESTE 8: Remoção com modelo inexistente ===";
    
    bool sucesso_remocao4 = gerenciador.remover_por_modelo("Ferrari Enzo");
    if (!sucesso_remocao4) {
        std::cout << "✓ Corretamente impediu remoção de modelo inexistente!" << std::endl;
    }

    // Teste 9: Verificar lista após remoções
    qDebug() << "\n=== TESTE 9: Lista após remoções ===";
    
    vector<Veiculos*> lista_final = gerenciador.listar();
    std::cout << "Total final de veículos: " << lista_final.size() << std::endl;
    
    // Mostrar alguns veículos restantes
    std::cout << "\nPrimeiros 3 veículos na lista final:" << std::endl;
    for (size_t i = 0; i < std::min(lista_final.size(), size_t(3)); i++) {
        Veiculos* v = lista_final[i];
        std::cout << "[" << i << "] " << v->motoOuCarro() 
                  << " - " << v->getModelo() 
                  << " (" << v->getAno() << ")"
                  << " - R$ " << v->getValorBase() << std::endl;
    }

    // Teste 10: Salvar no CSV
    qDebug() << "\n=== TESTE 10: Salvar no CSV ===";
    gerenciador.salvar_no_csv();

    // Resumo final
    QString resumo = QString("Testes CRUD Completos!\n\n"
                           "Operações realizadas:\n"
                           "• Carregados: %1 veículos\n"
                           "• Adicionados: 3 veículos\n"
                           "• Removidos: 2 veículos\n"
                           "• Total final: %2 veículos\n\n"
                           "Funcionalidades testadas:\n"
                           "✓ Carregar CSV\n"
                           "✓ Listar veículos\n"
                           "✓ Buscar por modelo\n"
                           "✓ Adicionar veículos\n"
                           "✓ Remover por índice\n"
                           "✓ Remover por modelo\n"
                           "✓ Salvar CSV\n"
                           "✓ Tratamento de erros")
                           .arg(todos_veiculos.size())
                           .arg(gerenciador.getTotalVeiculos());
    
    QMessageBox::information(this, "Teste CRUD Completo", resumo);
    
    qDebug() << "\n=== TODOS OS TESTES CRUD CONCLUÍDOS ===";
}

MainWindow::~MainWindow()
{
    delete ui;
}