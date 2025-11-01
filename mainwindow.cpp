#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gerenciadores.h"
#include "veiculos.h"
#include "clientes.h"
#include <iostream>
#include <QMessageBox>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Drive Tech");
    /*
    std::string csv_clientes_path = std::string(PROJECT_SOURCE_DIR) + "/clientes.csv";
    
    qDebug() << "=== TESTES DO SISTEMA DE CLIENTES ===";
    qDebug() << "Caminho do CSV Clientes:" << QString::fromStdString(csv_clientes_path);

    // Obtém a instância do gerenciador de clientes
    GerenciadorDeClientes& gerenciador_clientes = GerenciadorDeClientes::getInstance(csv_clientes_path);
    
    // Teste 1: Carregar clientes do CSV
    qDebug() << "\n=== TESTE 1: Carregar clientes do CSV ===";
    gerenciador_clientes.carregar_do_csv();

    // Teste 2: Listar todos os clientes
    qDebug() << "\n=== TESTE 2: Listar todos os clientes ===";
    vector<Clientes*> todos_clientes = gerenciador_clientes.listar();
    std::cout << "Total de clientes carregados: " << todos_clientes.size() << std::endl;

    // Exibir alguns clientes com seus índices
    if (!todos_clientes.empty()) {
        std::cout << "\nPrimeiros 5 clientes (com índices):" << std::endl;
        for (size_t i = 0; i < std::min(todos_clientes.size(), size_t(5)); i++) {
            Clientes* c = todos_clientes[i];
            std::cout << "[" << i << "] " << c->getNome() 
                      << " - CPF: " << c->getCpf()
                      << " - Tel: " << c->getTelefone() << std::endl;
        }
    }

    // Teste 3: Buscas por clientes
    qDebug() << "\n=== TESTE 3: Buscas por clientes ===";
    
    vector<Clientes*> busca_silva = gerenciador_clientes.buscar("Silva");
    std::cout << "Busca por 'Silva': " << busca_silva.size() << " clientes encontrados" << std::endl;
    for (size_t i = 0; i < busca_silva.size(); i++) {
        Clientes* c = busca_silva[i];
        std::cout << "• " << c->getNome() << " - CPF: " << c->getCpf() << std::endl;
    }

    vector<Clientes*> busca_cpf = gerenciador_clientes.buscar("123");
    std::cout << "\nBusca por '123' (no CPF): " << busca_cpf.size() << " clientes encontrados" << std::endl;

    // Teste 4: ADIÇÃO de novos clientes
    qDebug() << "\n=== TESTE 4: Adição de clientes ===";
    
    Clientes* novo_cliente1 = new Clientes("João Silva", "123.456.789-00", "(11) 9999-8888", "Rua A, 123");
    gerenciador_clientes.adicionar(novo_cliente1);
    
    Clientes* novo_cliente2 = new Clientes("Maria Santos", "987.654.321-00", "(11) 7777-6666", "Av. B, 456");
    gerenciador_clientes.adicionar(novo_cliente2);
    
    Clientes* novo_cliente3 = new Clientes("Carlos Oliveira", "111.222.333-44", "(11) 5555-4444", "Rua C, 789");
    gerenciador_clientes.adicionar(novo_cliente3);
    
    std::cout << "Total após adições: " << gerenciador_clientes.getTotalCliente() << " clientes" << std::endl;

    // Teste 5: Tentativa de adicionar cliente com CPF duplicado
    qDebug() << "\n=== TESTE 5: Tentativa de CPF duplicado ===";
    
    Clientes* cliente_duplicado = new Clientes("João Silva Duplicado", "123.456.789-00", "(11) 0000-0000", "Rua X, 999");
    gerenciador_clientes.adicionar(cliente_duplicado);
    std::cout << "✓ Impediu adição de CPF duplicado!" << std::endl;

    // Teste 6: REMOÇÃO por índice
    qDebug() << "\n=== TESTE 6: Remoção por índice ===";
    
    int total_antes_remocao = gerenciador_clientes.getTotalCliente();
    std::cout << "Total antes da remoção: " << total_antes_remocao << " clientes" << std::endl;
    
    // Remover o primeiro cliente (índice 0)
    bool sucesso_remocao1 = gerenciador_clientes.remover(0);
    if (sucesso_remocao1) {
        std::cout << "✓ Cliente no índice 0 removido com sucesso!" << std::endl;
    }
    
    std::cout << "Total após remoção do índice 0: " << gerenciador_clientes.getTotalCliente() << " clientes" << std::endl;

    // Teste 7: REMOÇÃO por CPF
    qDebug() << "\n=== TESTE 7: Remoção por CPF ===";
    
    // Remover por CPF específico
    bool sucesso_remocao2 = gerenciador_clientes.remover_por_cpf("987.654.321-00");
    if (sucesso_remocao2) {
        std::cout << "✓ Cliente com CPF 987.654.321-00 removido com sucesso!" << std::endl;
    }
    
    std::cout << "Total após remoção por CPF: " << gerenciador_clientes.getTotalCliente() << " clientes" << std::endl;

    // Teste 8: Tentativa de remoção com índice inválido
    qDebug() << "\n=== TESTE 8: Remoção com índice inválido ===";
    
    bool sucesso_remocao3 = gerenciador_clientes.remover(9999); // Índice inválido
    if (!sucesso_remocao3) {
        std::cout << "✓ Corretamente impediu remoção com índice inválido!" << std::endl;
    }

    // Teste 9: Tentativa de remoção com CPF inexistente
    qDebug() << "\n=== TESTE 9: Remoção com CPF inexistente ===";
    
    bool sucesso_remocao4 = gerenciador_clientes.remover_por_cpf("000.000.000-00");
    if (!sucesso_remocao4) {
        std::cout << "✓ Corretamente impediu remoção de CPF inexistente!" << std::endl;
    }

    // Teste 10: Verificar lista após remoções
    qDebug() << "\n=== TESTE 10: Lista após remoções ===";
    
    vector<Clientes*> lista_final = gerenciador_clientes.listar();
    std::cout << "Total final de clientes: " << lista_final.size() << std::endl;
    
    // Mostrar alguns clientes restantes
    if (!lista_final.empty()) {
        std::cout << "\nPrimeiros 3 clientes na lista final:" << std::endl;
        for (size_t i = 0; i < std::min(lista_final.size(), size_t(3)); i++) {
            Clientes* c = lista_final[i];
            std::cout << "[" << i << "] " << c->getNome() 
                      << " - CPF: " << c->getCpf()
                      << " - Tel: " << c->getTelefone() << std::endl;
        }
    }

    // Teste 11: Salvar clientes no CSV
    qDebug() << "\n=== TESTE 11: Salvar clientes no CSV ===";
    gerenciador_clientes.salvar_no_csv();

    // Teste 12: Testar setters dos clientes
    qDebug() << "\n=== TESTE 12: Testar setters dos clientes ===";
    if (!lista_final.empty()) {
        Clientes* cliente_teste = lista_final[0];
        std::cout << "Cliente antes das alterações: " << std::endl;
        std::cout << "  Nome: " << cliente_teste->getNome() << std::endl;
        std::cout << "  Telefone: " << cliente_teste->getTelefone() << std::endl;
        std::cout << "  Endereço: " << cliente_teste->getEndereco() << std::endl;
        
        // Testar setters
        cliente_teste->setTelefone("(11) 3333-2222");
        cliente_teste->setEndereco("Nova Rua, 1000");
        
        std::cout << "Cliente após alterações: " << std::endl;
        std::cout << "  Telefone: " << cliente_teste->getTelefone() << std::endl;
        std::cout << "  Endereço: " << cliente_teste->getEndereco() << std::endl;
    }

    // Resumo final
    QString resumo = QString("Testes de Clientes Concluídos!\n\n"
                           "Operações realizadas:\n"
                           "• Carregados: %1 clientes\n"
                           "• Adicionados: 3 clientes\n"
                           "• Removidos: 2 clientes\n"
                           "• Total final: %2 clientes\n\n"
                           "Funcionalidades testadas:\n"
                           "✓ Carregar CSV\n"
                           "✓ Listar clientes\n"
                           "✓ Buscar por nome/CPF\n"
                           "✓ Adicionar clientes\n"
                           "✓ Verificação de CPF duplicado\n"
                           "✓ Remover por índice\n"
                           "✓ Remover por CPF\n"
                           "✓ Setters e Getters\n"
                           "✓ Salvar CSV\n"
                           "✓ Tratamento de erros")
                           .arg(todos_clientes.size())
                           .arg(gerenciador_clientes.getTotalCliente());
    
    QMessageBox::information(this, "Teste do Sistema de Clientes", resumo);
    
    qDebug() << "\n=== TODOS OS TESTES DE CLIENTES CONCLUÍDOS ===";
    */
}


MainWindow::~MainWindow()
{
    delete ui;
}

// Tela inicial -> Tela Login
void MainWindow::on_botLogin_clicked()
{
    telaLogin = new TelaLogin(this);

    telaLogin->setWindowTitle("Login...");
    telaLogin->show();
    this->hide();
}

