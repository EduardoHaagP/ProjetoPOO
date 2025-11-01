#include "telaconsultaestoque.h"
#include "ui_telaconsultaestoque.h"

#include "telaprincipal.h"

TelaConsultaEstoque::TelaConsultaEstoque(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaConsultaEstoque)
{
    ui->setupUi(this);
}

TelaConsultaEstoque::~TelaConsultaEstoque()
{
    delete ui;
}

void TelaConsultaEstoque::on_botVoltar_clicked()
{
    TelaPrincipal *telaPrincipal = qobject_cast<TelaPrincipal*>(parent());
    // verifica
    if (telaPrincipal) {
        telaPrincipal->show();
    }
    this->close();
}

