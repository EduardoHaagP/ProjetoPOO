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

    // Teste 3: ADIÇÃO de novos vendedores
    qDebug() << "\n=== TESTE 3: Adição de vendedores ===";
    
    Vendedor* novo_vendedor1 = new Vendedor("João Silva", "123.456.789-00", "(11) 9999-8888", "Rua A, 123", "joao.silva@concessionaria.com", "senha123");
    gerenciador_vendedores.adicionar(novo_vendedor1);
    
    Vendedor* novo_vendedor2 = new Vendedor("Maria Santos", "987.654.321-00", "(11) 7777-6666", "Av. B, 456", "maria.santos@concessionaria.com", "abc456");
    gerenciador_vendedores.adicionar(novo_vendedor2);

    // Teste 4: TESTE DE AUTENTICAÇÃO ATUALIZADO
    qDebug() << "\n=== TESTE 4: Autenticação de vendedores (nova implementação) ===";
    
    Vendedor* vendedor_logado = nullptr;
    
    // Autenticação bem-sucedida
    bool autenticacao1 = gerenciador_vendedores.autenticar("joao.silva@concessionaria.com", "senha123", &vendedor_logado);
    if (autenticacao1 && vendedor_logado != nullptr) {
        std::cout << "✓ Autenticação bem-sucedida!" << std::endl;
        std::cout << "  Vendedor: " << vendedor_logado->getNome() << std::endl;
        std::cout << "  CPF: " << vendedor_logado->getCpf() << std::endl;
        std::cout << "  Email: " << vendedor_logado->getEmail() << std::endl;
    }
    
    // Autenticação com senha errada
    Vendedor* vendedor_temporario = nullptr;
    bool autenticacao2 = gerenciador_vendedores.autenticar("maria.santos@concessionaria.com", "senha_errada", &vendedor_temporario);
    if (!autenticacao2 && vendedor_temporario == nullptr) {
        std::cout << "✓ Corretamente rejeitou senha incorreta!" << std::endl;
        std::cout << "  Booleano: " << (autenticacao2 ? "true" : "false") << std::endl;
        std::cout << "  Ponteiro: " << (vendedor_temporario == nullptr ? "nullptr" : "válido") << std::endl;
    }
    
    // Autenticação com email inexistente
    Vendedor* vendedor_temporario2 = nullptr;
    bool autenticacao3 = gerenciador_vendedores.autenticar("naoexiste@concessionaria.com", "qualquersenha", &vendedor_temporario2);
    if (!autenticacao3 && vendedor_temporario2 == nullptr) {
        std::cout << "✓ Corretamente rejeitou email inexistente!" << std::endl;
        std::cout << "  Booleano: " << (autenticacao3 ? "true" : "false") << std::endl;
        std::cout << "  Ponteiro: " << (vendedor_temporario2 == nullptr ? "nullptr" : "válido") << std::endl;
    }

    // Teste 5: USO PRÁTICO DO VENDEDOR AUTENTICADO
    qDebug() << "\n=== TESTE 5: Uso prático do vendedor autenticado ===";
    
    Vendedor* vendedor_atual = nullptr;
    if (gerenciador_vendedores.autenticar("joao.silva@concessionaria.com", "senha123", &vendedor_atual)) {
        std::cout << "✓ Vendedor autenticado com sucesso!" << std::endl;
        std::cout << "  Bem-vindo, " << vendedor_atual->getNome() << "!" << std::endl;
        std::cout << "  Seu telefone: " << vendedor_atual->getTelefone() << std::endl;
        std::cout << "  Seu endereço: " << vendedor_atual->getEndereco() << std::endl;
        
        // Podemos usar o vendedor autenticado para outras operações
        // Exemplo: alterar dados do vendedor logado
        vendedor_atual->setTelefone("(11) 3333-2222");
        std::cout << "  Telefone atualizado para: " << vendedor_atual->getTelefone() << std::endl;
    } else {
        std::cout << "✗ Falha na autenticação!" << std::endl;
    }

    // Teste 6: VERIFICAÇÃO DE EMAIL DISPONÍVEL
    qDebug() << "\n=== TESTE 6: Verificação de email disponível ===";
    
    bool email1_disponivel = gerenciador_vendedores.verificarEmailDisponivel("joao.silva@concessionaria.com");
    bool email2_disponivel = gerenciador_vendedores.verificarEmailDisponivel("novo.email@concessionaria.com");
    
    std::cout << "Email 'joao.silva@concessionaria.com' disponível? " << (email1_disponivel ? "SIM" : "NÃO") << std::endl;
    std::cout << "Email 'novo.email@concessionaria.com' disponível? " << (email2_disponivel ? "SIM" : "NÃO") << std::endl;

    // Teste 7: Salvar vendedores no CSV
    qDebug() << "\n=== TESTE 7: Salvar vendedores no CSV ===";
    gerenciador_vendedores.salvar_no_csv();

    // Resumo final
    QString resumo = QString("Testes de Vendedores Concluídos!\n\n"
                           "Nova implementação de autenticação:\n"
                           "• Retorna booleano (sucesso/falha)\n"
                           "• Retorna vendedor por ponteiro\n"
                           "• Uso prático do vendedor autenticado\n\n"
                           "Funcionalidades testadas:\n"
                           "✓ Carregar CSV\n"
                           "✓ Listar vendedores\n"
                           "✓ Adicionar vendedores\n"
                           "✓ Autenticação (nova implementação)\n"
                           "✓ Verificação de email disponível\n"
                           "✓ Uso do vendedor autenticado\n"
                           "✓ Salvar CSV")
                           .arg(todos_vendedores.size());
    
    QMessageBox::information(this, "Teste do Sistema de Vendedores", resumo);
    
    qDebug() << "\n=== TODOS OS TESTES DE VENDEDORES CONCLUÍDOS ===";
}

MainWindow::~MainWindow()
{
    delete ui;
}