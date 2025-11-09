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
    this->close();
}

