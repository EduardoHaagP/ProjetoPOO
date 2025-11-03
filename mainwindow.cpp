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
    , telaCadastroCliente(nullptr)
    , telaConsultaEstoque(nullptr)
    , telaListagemVendas(nullptr)
    , telaResgistroVendas(nullptr)
{
    ui->setupUi(this);

    setWindowTitle("Drive Tech - Concessionaria");

    ui->stackedWidget->setCurrentIndex(0);

    // carrega csv vendedores qnd abre o app
    GerenciadorDeVendedores::getInstance().carregar_do_csv();
    
}


MainWindow::~MainWindow()
{
    delete ui;
}

// Tela inicial -> Tela Login
void MainWindow::on_botEntrar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

// Tela login -> Tela principal
void MainWindow::on_botOk_clicked()
{
    std::string inpEmail = ui->inpUsuario->text().toStdString();
    std::string inpSenha = ui->inpSenha->text().toStdString(); // funcao do gerenciado

    Vendedor *vendedorLogado = nullptr;

    bool verifEmail = GerenciadorDeVendedores::getInstance().verificarEmailDisponivel(inpEmail);
    bool verifLogin = false;

    if (verifEmail){
        // deixar bordas quadrado verde

        ui->txtErro->setText("");  //limpa erro
        verifLogin = GerenciadorDeVendedores::getInstance().autenticar(inpEmail, inpSenha, &vendedorLogado);

        if(verifLogin){
            ui->txtErro->setText("");  //limpa erro
            ui->stackedWidget->setCurrentIndex(2);
            return;     // Sai da função após o login bem-sucedido

            // deixar bordas quadrado verde


        }else{
            // borda senha vermelha

            // txt erro
            ui->txtErro->setText("Senha incorreta");
        }
    }else{
        // borda email vermelha

        // txt erro
        ui->txtErro->setText("Email não encontrado");
    };


}

// Tela principal -> Tela inicial ========================== lembrar de resetar o login qnd sair
void MainWindow::on_botSair_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



// ========== botoes para telas secundarias ==========

void MainWindow::on_botCadastroCliente_clicked()
{
    if (telaCadastroCliente == nullptr) {
        telaCadastroCliente = new TelaCadastroCliente(this);
        telaCadastroCliente->setWindowTitle("Cadastro de Clientes");

        // resetar o ponteiro
        connect(telaCadastroCliente, &TelaCadastroCliente::finished,
                this, [this](){
                    this->telaCadastroCliente = nullptr;
                });
    }
    telaCadastroCliente->show();
    telaCadastroCliente->activateWindow(); // se já estiver aberta, tras pra frente
}

void MainWindow::on_botVenda_clicked()
{
    if (telaResgistroVendas == nullptr) {
        telaResgistroVendas = new TelaResgistroVendas(this);
        telaResgistroVendas->setWindowTitle("Registro de Vendas");

        // resetar o ponteiro
        connect(telaResgistroVendas, &TelaResgistroVendas::finished,
                this, [this](){
                    this->telaResgistroVendas = nullptr;
                });
    }
    telaResgistroVendas->show();
    telaResgistroVendas->activateWindow();
}

void MainWindow::on_botListaVendas_clicked()
{
    if (telaListagemVendas == nullptr) {
        telaListagemVendas = new TelaListagemVendas(this);
        telaListagemVendas->setWindowTitle("Listagem de Vendas");

        // resetar o ponteiro
        connect(telaListagemVendas, &TelaListagemVendas::finished,
                this, [this](){
                    this->telaListagemVendas = nullptr;
                });
    }
    telaListagemVendas->show();
    telaListagemVendas->activateWindow();
}

void MainWindow::on_botEstoque_clicked()
{
    if (telaConsultaEstoque == nullptr) {
        telaConsultaEstoque = new TelaConsultaEstoque(this);
        telaConsultaEstoque->setWindowTitle("Consulta de Estoque");

        // resetar o ponteiro
        connect(telaConsultaEstoque, &TelaConsultaEstoque::finished,
                this, [this](){
                    this->telaConsultaEstoque = nullptr;
                });
    }
    telaConsultaEstoque->show();
    telaConsultaEstoque->activateWindow();
}
