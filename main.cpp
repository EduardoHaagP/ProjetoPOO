#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "gerenciadores.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATAFORM","xcb");
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    qDebug() << "--- Carregando Dados ---";

    GerenciadorDeVendedores::getInstance().carregar_do_csv();
    GerenciadorDeClientes::getInstance().carregar_do_csv();
    GerenciadorDeVeiculos::getInstance().carregar_do_csv();
    GerenciadorDeVendas::getInstance().carregar_do_csv();

    qDebug() << "--- Dados Carregados ---";

    MainWindow w;
    w.show();
    return a.exec();
}
