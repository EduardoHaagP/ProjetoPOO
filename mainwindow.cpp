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
    , vendedorAutenticado(nullptr) // << Variável da classe inicializada
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/assets/LogoEscura.png"));

    QPixmap BemVindo(":/assets/EscritoBemVindo.png"); // <-- Mude se o nome da sua imagem for outro
    ui->imgBemVindo->setPixmap(BemVindo);
    ui->imgBemVindo->setScaledContents(true);

    QPixmap logo(":/assets/LogoEscura.png"); // <-- Mude se o nome da sua imagem for outro
    ui->imgLogo->setPixmap(logo);
    ui->imgLogo->setScaledContents(true);







    // se o texto mudar, limpa o txtErro
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


// chama qnd clica no X
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "--- Salvando dados nos CSVs ---";

    // salva td
    GerenciadorDeClientes::getInstance().salvar_no_csv();
    GerenciadorDeVendedores::getInstance().salvar_no_csv();
    GerenciadorDeVeiculos::getInstance().salvar_no_csv();
    GerenciadorDeVendas::getInstance().salvar_no_csv();

    qDebug() << "--- Dados Salvos. Fechando o app. ---";

    // continua fechamento normal
    QMainWindow::closeEvent(event);
}

void MainWindow::on_botEntrar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

// --- ESTA É A FUNÇÃO CORRIGIDA ---
void MainWindow::on_botOk_clicked()
{

    ui->txtErro->setText("");

    std::string inpEmail = ui->inpUsuario->text().toStdString();
    std::string inpSenha = ui->inpSenha->text().toStdString();

    // Usamos 'vendedorAutenticado' (o membro da classe)
    vendedorAutenticado = nullptr;

    bool emailExiste = !GerenciadorDeVendedores::getInstance().verificarEmailDisponivel(inpEmail);
    bool verifLogin = false;

    //campos vazios
    if (inpEmail.empty() || inpSenha.empty()) {
        ui->txtErro->setText("Email e senha devem ser preenchidos");
        return; // Para a execução aqui
    }

    //verifica email
    if (emailExiste){
        ui->txtErro->setText("");

        // AQUI ESTÁ A CORREÇÃO:
        // Usamos &vendedorAutenticado (a variável da classe)
        verifLogin = GerenciadorDeVendedores::getInstance().autenticar(inpEmail, inpSenha, &vendedorAutenticado);

        if(verifLogin){
            ui->txtErro->setText("");
            ui->stackedWidget->setCurrentIndex(2);

            //limpa os inputs
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
    vendedorAutenticado = nullptr; // Limpa o vendedor no logout
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
    // Verifica se há um vendedor logado
    if (vendedorAutenticado == nullptr) {
        QMessageBox::critical(this, "Erro de Autenticação", "Nenhum vendedor está logado!");
        ui->stackedWidget->setCurrentIndex(1); // Volta para tela de login
        return;
    }

    // Só abre se não houver outra aberta
    if (telaResgistroVendas == nullptr) {
        // Passa o vendedor logado para a tela de vendas
        telaResgistroVendas = new TelaResgistroVendas(vendedorAutenticado, this);
        telaResgistroVendas->setWindowTitle("Registro de Vendas");

        // Conecta o sinal 'finished' para limpar o ponteiro quando a tela for fechada
        connect(telaResgistroVendas, &TelaResgistroVendas::finished,
                this, [this](){
                    this->telaResgistroVendas = nullptr;
                    // Atualiza a tabela de estoque na tela de consulta, se ela estiver aberta
                    if (this->telaConsultaEstoque) {
                        // (telaconsultaestoque.cpp usa 'on_botCadastrar_clicked' para consultar)
                        this->telaConsultaEstoque->on_botCadastrar_clicked();
                    }
                });
    }
    telaResgistroVendas->show();

    telaResgistroVendas->activateWindow(); // Traz a tela para frente
}

void MainWindow::on_botListaVendas_clicked()
{
    // Se a tela já existe, fecha e destrói ela
    if (telaListagemVendas != nullptr) {
        telaListagemVendas->close();
        delete telaListagemVendas;
        this->telaListagemVendas = nullptr; // <-- ESSA LINHA CORRIGE O CRASH
    }

    // Cria uma instância *nova*
    telaListagemVendas = new TelaListagemVendas(this);
    telaListagemVendas->setWindowTitle("Listagem de Vendas");

    // Conecta o 'finished' para limpar o ponteiro quando o *usuário* fechar
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
