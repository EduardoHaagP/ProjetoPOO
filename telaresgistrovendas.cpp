#include "telaresgistrovendas.h"
#include "ui_telaresgistrovendas.h"

#include "telaprincipal.h"

TelaResgistroVendas::TelaResgistroVendas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaResgistroVendas)
{
    ui->setupUi(this);
}

TelaResgistroVendas::~TelaResgistroVendas()
{
    delete ui;
}

void TelaResgistroVendas::on_botVoltar_clicked()
{
    TelaPrincipal *telaPrincipal = qobject_cast<TelaPrincipal*>(parent());
    // verifica
    if (telaPrincipal) {
        telaPrincipal->show();
    }
    this->close();
}

