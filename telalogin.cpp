#include "telalogin.h"
#include "ui_telalogin.h"

#include "mainwindow.h"

TelaLogin::TelaLogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaLogin)
{
    ui->setupUi(this);
}

TelaLogin::~TelaLogin()
{
    delete ui;
}

void TelaLogin::on_botOk_clicked()
{

    // fazer o autenticador do login antes dessa parte

    telaPrincipal = new TelaPrincipal(this);

    telaPrincipal->setWindowTitle("Tela Principal");
    telaPrincipal->show();

    // cast seguro para MainWindow*
    MainWindow *mainWindow = qobject_cast<MainWindow*>(parent());
    // verifica
    if (mainWindow) {
        mainWindow->hide();
    }

}


void TelaLogin::on_botVoltar_clicked()
{
    MainWindow *mainWindow = qobject_cast<MainWindow*>(parent());
    // verifica
    if (mainWindow) {
        mainWindow->show();
    }
    this->close();
}

