#include "telacadastrocliente.h"
#include "ui_telacadastrocliente.h"

#include "telaprincipal.h"

TelaCadastroCliente::TelaCadastroCliente(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaCadastroCliente)
{
    ui->setupUi(this);
}

TelaCadastroCliente::~TelaCadastroCliente()
{
    delete ui;
}

void TelaCadastroCliente::on_botVoltar_clicked()
{
    TelaPrincipal *telaPrincipal = qobject_cast<TelaPrincipal*>(parent());
    // verifica
    if (telaPrincipal) {
        telaPrincipal->show();
    }
    this->close();
}

