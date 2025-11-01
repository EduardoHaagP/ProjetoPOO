#include "telalistagemvendas.h"
#include "ui_telalistagemvendas.h"

#include "telaprincipal.h"

TelaListagemVendas::TelaListagemVendas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaListagemVendas)
{
    ui->setupUi(this);
}

TelaListagemVendas::~TelaListagemVendas()
{
    delete ui;
}

void TelaListagemVendas::on_botVoltar_clicked()
{
    TelaPrincipal *telaPrincipal = qobject_cast<TelaPrincipal*>(parent());
    // verifica
    if (telaPrincipal) {
        telaPrincipal->show();
    }
    this->close();
}

