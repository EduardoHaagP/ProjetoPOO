#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gerenciadores.h"
#include "veiculos.h"
#include "clientes.h"
#include "vendedor.h"
#include <iostream>
#include <QMessageBox>
#include <QDebug>
#include <QPixmap>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , telaCadastroCliente(nullptr)
    , telaConsultaEstoque(nullptr)
    , telaListagemVendas(nullptr)
    , telaResgistroVendas(nullptr)
    , vendedorAutenticado(nullptr) 
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/assets/LogoEscura.png"));

    QPixmap BemVindo(":/assets/EscritoBemVindo.png"); 
    ui->imgBemVindo->setPixmap(BemVindo);
    ui->imgBemVindo->setScaledContents(true);

    QPixmap logo(":/assets/LogoEscura.png"); 
    ui->imgLogo->setPixmap(logo);
    ui->imgLogo->setScaledContents(true);


    connect(ui->inpUsuario, &QLineEdit::textChanged, this, [this](){
        ui->txtErro->setText("");
    });
    connect(ui->inpSenha, &QLineEdit::textChanged, this, [this](){
        ui->txtErro->setText("");
    });


    setWindowTitle("Drive Tech - Concessionaria");

    ui->stackedWidget->setCurrentIndex(0);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "--- Salvando dados nos CSVs ---";

    GerenciadorDeClientes::getInstance().salvar_no_csv();
    GerenciadorDeVendedores::getInstance().salvar_no_csv();
    GerenciadorDeVeiculos::getInstance().salvar_no_csv();
    GerenciadorDeVendas::getInstance().salvar_no_csv();

    qDebug() << "--- Dados Salvos. Fechando o app. ---";

    QMainWindow::closeEvent(event);
}

void MainWindow::on_botEntrar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::on_botOk_clicked()
{

    ui->txtErro->setText("");

    std::string inpEmail = ui->inpUsuario->text().toStdString();
    std::string inpSenha = ui->inpSenha->text().toStdString();

    vendedorAutenticado = nullptr;

    bool emailExiste = !GerenciadorDeVendedores::getInstance().verificarEmailDisponivel(inpEmail);
    bool verifLogin = false;

    if (inpEmail.empty() || inpSenha.empty()) {
        ui->txtErro->setText("Email e senha devem ser preenchidos");
        return; 
    }

    if (emailExiste){
        ui->txtErro->setText("");

        verifLogin = GerenciadorDeVendedores::getInstance().autenticar(inpEmail, inpSenha, &vendedorAutenticado);

        if(verifLogin){
            ui->txtErro->setText("");
            ui->stackedWidget->setCurrentIndex(2);

            ui->inpUsuario->clear();
            ui->inpSenha->clear();
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
    vendedorAutenticado = nullptr; 
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
    if (vendedorAutenticado == nullptr) {
        QMessageBox::critical(this, "Erro de Autenticação", "Nenhum vendedor está logado!");
        ui->stackedWidget->setCurrentIndex(1); 
        return;
    }

    if (telaResgistroVendas == nullptr) {
        telaResgistroVendas = new TelaResgistroVendas(vendedorAutenticado, this);
        telaResgistroVendas->setWindowTitle("Registro de Vendas");

        connect(telaResgistroVendas, &TelaResgistroVendas::finished,
                this, [this](){
                    this->telaResgistroVendas = nullptr;
                    if (this->telaConsultaEstoque) {
                       
                        this->telaConsultaEstoque->on_botCadastrar_clicked();
                    }
                });
    }
    telaResgistroVendas->show();

    telaResgistroVendas->activateWindow(); 
}

void MainWindow::on_botListaVendas_clicked()
{
    if (telaListagemVendas != nullptr) {
        telaListagemVendas->close();
        delete telaListagemVendas;
        this->telaListagemVendas = nullptr;
    }

    telaListagemVendas = new TelaListagemVendas(this);
    telaListagemVendas->setWindowTitle("Listagem de Vendas");

    connect(telaListagemVendas, &TelaListagemVendas::finished,
            this, [this](){
                this->telaListagemVendas = nullptr;
            });

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
