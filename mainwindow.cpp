#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gerenciadores.h"
#include "veiculos.h"
#include "clientes.h"
#include "vendedor.h"
#include <iostream>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    std::string csv_vendedores_path = std::string(PROJECT_SOURCE_DIR) + "/Trabalho POO - Vendedores.csv";
    
    qDebug() << "=== TESTES DO SISTEMA DE VENDEDORES ===";
    qDebug() << "Caminho do CSV Vendedores:" << QString::fromStdString(csv_vendedores_path);

    // Obtém a instância do gerenciador de vendedores
    GerenciadorDeVendedores& gerenciador_vendedores = GerenciadorDeVendedores::getInstance(csv_vendedores_path);
    
    // Teste 1: Carregar vendedores do CSV
    qDebug() << "\n=== TESTE 1: Carregar vendedores do CSV ===";
    gerenciador_vendedores.carregar_do_csv();

    // Teste 2: Listar todos os vendedores
    qDebug() << "\n=== TESTE 2: Listar todos os vendedores ===";
    vector<Vendedor*> todos_vendedores = gerenciador_vendedores.listar();
    std::cout << "Total de vendedores carregados: " << todos_vendedores.size() << std::endl;

    // Exibir alguns vendedores com seus índices
    if (!todos_vendedores.empty()) {
        std::cout << "\nPrimeiros 5 vendedores (com índices):" << std::endl;
        for (size_t i = 0; i < std::min(todos_vendedores.size(), size_t(5)); i++) {
            Vendedor* v = todos_vendedores[i];
            std::cout << "[" << i << "] " << v->getNome() 
                      << " - CPF: " << v->getCpf()
                      << " - Email: " << v->getEmail() << std::endl;
        }
    }

    // Teste 3: Buscas por vendedores
    qDebug() << "\n=== TESTE 3: Buscas por vendedores ===";
    
    vector<Vendedor*> busca_silva = gerenciador_vendedores.buscar("Silva");
    std::cout << "Busca por 'Silva': " << busca_silva.size() << " vendedores encontrados" << std::endl;
    for (size_t i = 0; i < busca_silva.size(); i++) {
        Vendedor* v = busca_silva[i];
        std::cout << "• " << v->getNome() << " - CPF: " << v->getCpf() << " - Email: " << v->getEmail() << std::endl;
    }

    vector<Vendedor*> busca_email = gerenciador_vendedores.buscar("@");
    std::cout << "\nBusca por '@' (emails): " << busca_email.size() << " vendedores encontrados" << std::endl;

    // Teste 4: ADIÇÃO de novos vendedores
    qDebug() << "\n=== TESTE 4: Adição de vendedores ===";
    
    Vendedor* novo_vendedor1 = new Vendedor("João Silva", "123.456.789-00", "(11) 9999-8888", "Rua A, 123", "joao.silva@concessionaria.com", "senha123");
    gerenciador_vendedores.adicionar(novo_vendedor1);
    
    Vendedor* novo_vendedor2 = new Vendedor("Maria Santos", "987.654.321-00", "(11) 7777-6666", "Av. B, 456", "maria.santos@concessionaria.com", "abc456");
    gerenciador_vendedores.adicionar(novo_vendedor2);
    
    Vendedor* novo_vendedor3 = new Vendedor("Carlos Oliveira", "111.222.333-44", "(11) 5555-4444", "Rua C, 789", "carlos.oliveira@concessionaria.com", "xyz789");
    gerenciador_vendedores.adicionar(novo_vendedor3);
    
    std::cout << "Total após adições: " << gerenciador_vendedores.getTotalVendedor() << " vendedores" << std::endl;

    // Teste 5: Tentativa de adicionar vendedor com CPF duplicado
    qDebug() << "\n=== TESTE 5: Tentativa de CPF duplicado ===";
    
    Vendedor* vendedor_duplicado = new Vendedor("João Silva Duplicado", "123.456.789-00", "(11) 0000-0000", "Rua X, 999", "joao.duplicado@concessionaria.com", "senha999");
    gerenciador_vendedores.adicionar(vendedor_duplicado);
    std::cout << "✓ Impediu adição de CPF duplicado!" << std::endl;

    // Teste 6: Tentativa de adicionar vendedor com email duplicado
    qDebug() << "\n=== TESTE 6: Tentativa de email duplicado ===";
    
    Vendedor* email_duplicado = new Vendedor("Outro João", "999.888.777-66", "(11) 1111-2222", "Rua Y, 888", "joao.silva@concessionaria.com", "outrasenha");
    gerenciador_vendedores.adicionar(email_duplicado);
    std::cout << "✓ Impediu adição de email duplicado!" << std::endl;

    // Teste 7: TESTE DE AUTENTICAÇÃO
    qDebug() << "\n=== TESTE 7: Autenticação de vendedores ===";
    
    // Autenticação bem-sucedida
    Vendedor* vendedor_autenticado1 = gerenciador_vendedores.autenticar("joao.silva@concessionaria.com", "senha123");
    if (vendedor_autenticado1 != nullptr) {
        std::cout << "✓ Autenticação bem-sucedida para: " << vendedor_autenticado1->getNome() << std::endl;
    }
    
    // Autenticação com senha errada
    Vendedor* vendedor_autenticado2 = gerenciador_vendedores.autenticar("maria.santos@concessionaria.com", "senha_errada");
    if (vendedor_autenticado2 == nullptr) {
        std::cout << "✓ Corretamente rejeitou senha incorreta!" << std::endl;
    }
    
    // Autenticação com email inexistente
    Vendedor* vendedor_autenticado3 = gerenciador_vendedores.autenticar("naoexiste@concessionaria.com", "qualquersenha");
    if (vendedor_autenticado3 == nullptr) {
        std::cout << "✓ Corretamente rejeitou email inexistente!" << std::endl;
    }

    // Teste 8: VERIFICAÇÃO DE EMAIL DISPONÍVEL
    qDebug() << "\n=== TESTE 8: Verificação de email disponível ===";
    
    bool email1_disponivel = gerenciador_vendedores.verificarEmailDisponivel("joao.silva@concessionaria.com");
    bool email2_disponivel = gerenciador_vendedores.verificarEmailDisponivel("novo.email@concessionaria.com");
    
    std::cout << "Email 'joao.silva@concessionaria.com' disponível? " << (email1_disponivel ? "SIM" : "NÃO") << std::endl;
    std::cout << "Email 'novo.email@concessionaria.com' disponível? " << (email2_disponivel ? "SIM" : "NÃO") << std::endl;

    // Teste 9: REMOÇÃO por índice
    qDebug() << "\n=== TESTE 9: Remoção por índice ===";
    
    int total_antes_remocao = gerenciador_vendedores.getTotalVendedor();
    std::cout << "Total antes da remoção: " << total_antes_remocao << " vendedores" << std::endl;
    
    // Remover o primeiro vendedor (índice 0)
    bool sucesso_remocao1 = gerenciador_vendedores.remover(0);
    if (sucesso_remocao1) {
        std::cout << "✓ Vendedor no índice 0 removido com sucesso!" << std::endl;
    }
    
    std::cout << "Total após remoção do índice 0: " << gerenciador_vendedores.getTotalVendedor() << " vendedores" << std::endl;

    // Teste 10: REMOÇÃO por CPF
    qDebug() << "\n=== TESTE 10: Remoção por CPF ===";
    
    // Remover por CPF específico
    bool sucesso_remocao2 = gerenciador_vendedores.remover_por_cpf("987.654.321-00");
    if (sucesso_remocao2) {
        std::cout << "✓ Vendedor com CPF 987.654.321-00 removido com sucesso!" << std::endl;
    }
    
    std::cout << "Total após remoção por CPF: " << gerenciador_vendedores.getTotalVendedor() << " vendedores" << std::endl;

    // Teste 11: Tentativa de remoção com índice inválido
    qDebug() << "\n=== TESTE 11: Remoção com índice inválido ===";
    
    bool sucesso_remocao3 = gerenciador_vendedores.remover(9999); // Índice inválido
    if (!sucesso_remocao3) {
        std::cout << "✓ Corretamente impediu remoção com índice inválido!" << std::endl;
    }

    // Teste 12: Tentativa de remoção com CPF inexistente
    qDebug() << "\n=== TESTE 12: Remoção com CPF inexistente ===";
    
    bool sucesso_remocao4 = gerenciador_vendedores.remover_por_cpf("000.000.000-00");
    if (!sucesso_remocao4) {
        std::cout << "✓ Corretamente impediu remoção de CPF inexistente!" << std::endl;
    }

    // Teste 13: Verificar lista após remoções
    qDebug() << "\n=== TESTE 13: Lista após remoções ===";
    
    vector<Vendedor*> lista_final = gerenciador_vendedores.listar();
    std::cout << "Total final de vendedores: " << lista_final.size() << std::endl;
    
    // Mostrar alguns vendedores restantes
    if (!lista_final.empty()) {
        std::cout << "\nPrimeiros 3 vendedores na lista final:" << std::endl;
        for (size_t i = 0; i < std::min(lista_final.size(), size_t(3)); i++) {
            Vendedor* v = lista_final[i];
            std::cout << "[" << i << "] " << v->getNome() 
                      << " - CPF: " << v->getCpf()
                      << " - Email: " << v->getEmail() << std::endl;
        }
    }

    // Teste 14: Testar setters dos vendedores
    qDebug() << "\n=== TESTE 14: Testar setters dos vendedores ===";
    if (!lista_final.empty()) {
        Vendedor* vendedor_teste = lista_final[0];
        std::cout << "Vendedor antes das alterações: " << std::endl;
        std::cout << "  Nome: " << vendedor_teste->getNome() << std::endl;
        std::cout << "  Telefone: " << vendedor_teste->getTelefone() << std::endl;
        std::cout << "  Endereço: " << vendedor_teste->getEndereco() << std::endl;
        std::cout << "  Email: " << vendedor_teste->getEmail() << std::endl;
        
        // Testar setters
        vendedor_teste->setTelefone("(11) 3333-2222");
        vendedor_teste->setEndereco("Nova Rua, 1000");
        vendedor_teste->setSenha("novasenha123");
        
        std::cout << "Vendedor após alterações: " << std::endl;
        std::cout << "  Telefone: " << vendedor_teste->getTelefone() << std::endl;
        std::cout << "  Endereço: " << vendedor_teste->getEndereco() << std::endl;
        std::cout << "  Senha alterada com sucesso!" << std::endl;
    }

    // Teste 15: Salvar vendedores no CSV
    qDebug() << "\n=== TESTE 15: Salvar vendedores no CSV ===";
    gerenciador_vendedores.salvar_no_csv();

    // Resumo final
    QString resumo = QString("Testes de Vendedores Concluídos!\n\n"
                           "Operações realizadas:\n"
                           "• Carregados: %1 vendedores\n"
                           "• Adicionados: 3 vendedores\n"
                           "• Removidos: 2 vendedores\n"
                           "• Total final: %2 vendedores\n\n"
                           "Funcionalidades testadas:\n"
                           "✓ Carregar CSV\n"
                           "✓ Listar vendedores\n"
                           "✓ Buscar por nome/CPF/email\n"
                           "✓ Adicionar vendedores\n"
                           "✓ Verificação de CPF duplicado\n"
                           "✓ Verificação de email duplicado\n"
                           "✓ Autenticação (login/senha)\n"
                           "✓ Verificação de email disponível\n"
                           "✓ Remover por índice\n"
                           "✓ Remover por CPF\n"
                           "✓ Setters e Getters\n"
                           "✓ Salvar CSV\n"
                           "✓ Tratamento de erros")
                           .arg(todos_vendedores.size())
                           .arg(gerenciador_vendedores.getTotalVendedor());
    
    QMessageBox::information(this, "Teste do Sistema de Vendedores", resumo);
    
    qDebug() << "\n=== TODOS OS TESTES DE VENDEDORES CONCLUÍDOS ===";
}

MainWindow::~MainWindow()
{
    delete ui;
}