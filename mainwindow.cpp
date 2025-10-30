#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gerenciadores.h"
#include "veiculos.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GerenciadorDeVeiculos& gerenciador = GerenciadorDeVeiculos::getInstance();
    gerenciador.carregar_do_csv();
    vector<Veiculos*> teste = gerenciador.listar();
    for (int i = 0; i < teste.size(); i++)
    {
        std::cout<<teste[0]->getModelo()<<endl;
    }
    
}

MainWindow::~MainWindow()
{
    delete ui;
}

