#include "mainwindow.h"

#include <QApplication>
#include <iostream>

#include "gerenciadores.h" // inicializar os gerenciadores


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* gerenciadores usam o padrao singleton e precisam ser carregados antes de tudo*/
    std::cout << "--- Carregando Dados ---" << std::endl;

    // carrega vendedores
    GerenciadorDeVendedores::getInstance().carregar_do_csv();

    // carrega clientes
    GerenciadorDeClientes::getInstance().carregar_do_csv();

    // carrega estoque
    GerenciadorDeVeiculos::getInstance().carregar_do_csv();

    // carrega vendas
    GereniciadorDeVendas::getInstance().carregar_do_csv();

    std::cout << "--- Dados Carregados ---" << std::endl;

    MainWindow w;
    w.show();
    return a.exec();
}


