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

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_botEntrar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::on_botOk_clicked()
{
    std::string inpEmail = ui->inpUsuario->text().toStdString();
    std::string inpSenha = ui->inpSenha->text().toStdString();

    Vendedor *vendedorLogado = nullptr;

    bool verifEmail = GerenciadorDeVendedores::getInstance().verificarEmailDisponivel(inpEmail);
    bool verifLogin = false;

    if (verifEmail){
        ui->txtErro->setText("");
        verifLogin = GerenciadorDeVendedores::getInstance().autenticar(inpEmail, inpSenha, &vendedorLogado);

        if(verifLogin){
            ui->txtErro->setText("");
            ui->stackedWidget->setCurrentIndex(2);
            return;

        }else{
            ui->txtErro->setText("Senha incorreta");
        }
    }else{
        ui->txtErro->setText("Email não encontrado");
    };


}

void MainWindow::on_botSair_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_botCadastroCliente_clicked()
{
    if (telaCadastroCliente == nullptr) {
        telaCadastroCliente = new TelaCadastroCliente(this);
        telaCadastroCliente->setWindowTitle("Cadastro de Clientes");

        connect(telaCadastroCliente, &TelaCadastroCliente::finished,
                this, [this](){
                    this->telaCadastroCliente = nullptr;
                });
    }
    telaCadastroCliente->show();
    telaCadastroCliente->activateWindow();
}

void MainWindow::on_botVenda_clicked()
{
    if (telaResgistroVendas == nullptr) {
        telaResgistroVendas = new TelaResgistroVendas(this);
        telaResgistroVendas->setWindowTitle("Registro de Vendas");

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

        connect(telaConsultaEstoque, &TelaConsultaEstoque::finished,
                this, [this](){
                    this->telaConsultaEstoque = nullptr;
                });
    }
    telaConsultaEstoque->show();
    telaConsultaEstoque->activateWindow();
}
