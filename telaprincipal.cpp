#include "telaprincipal.h"
#include "ui_telaprincipal.h"

#include "telalogin.h"


TelaPrincipal::TelaPrincipal(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaPrincipal)
{
    ui->setupUi(this);
}

TelaPrincipal::~TelaPrincipal()
{
    delete ui;
}

// sair tela principal -> voltar tela login
void TelaPrincipal::on_botSair_clicked()
{
    TelaLogin *telaLogin = qobject_cast<TelaLogin*>(parent());
    // verifica
    if (telaLogin) {
        telaLogin->show();
    }
    this->close();
}

// tela principal -> TelaCadastro de Clientes
void TelaPrincipal::on_botCadastroCliente_clicked()
{
    telaCadastroCliente = new TelaCadastroCliente(this);
    telaCadastroCliente->setWindowTitle("Cadastro de Clientes");
    telaCadastroCliente->show();
}


void TelaPrincipal::on_botVenda_clicked()
{
    telaResgistroVendas = new TelaResgistroVendas(this);
    telaResgistroVendas->setWindowTitle("Registro de Vendas");
    telaResgistroVendas->show();
}


void TelaPrincipal::on_botListaVendas_clicked()
{
    telaListagemVendas = new TelaListagemVendas(this);
    telaListagemVendas->setWindowTitle("Listagem de Vendas");
    telaListagemVendas->show();
}


void TelaPrincipal::on_botEstoque_clicked()
{
    telaConsultaEstoque = new TelaConsultaEstoque(this);
    telaConsultaEstoque->setWindowTitle("Consulta de Estoque");
    telaConsultaEstoque->show();
}

