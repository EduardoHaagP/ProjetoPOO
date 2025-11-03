#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "gerenciadores.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "--- Carregando Dados ---";

    GerenciadorDeVendedores::getInstance().carregar_do_csv();
    GerenciadorDeClientes::getInstance().carregar_do_csv();
    GerenciadorDeVeiculos::getInstance().carregar_do_csv();
    GereniciadorDeVendas::getInstance().carregar_do_csv();

    qDebug() << "--- Dados Carregados ---";

    MainWindow w;
    w.show();
    return a.exec();
}
