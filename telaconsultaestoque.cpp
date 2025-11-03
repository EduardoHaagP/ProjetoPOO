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
    this->close();
}

