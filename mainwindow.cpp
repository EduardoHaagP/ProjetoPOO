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
    std::string csv_path = std::string(PROJECT_SOURCE_DIR) + "/Trabalho POO - Carros.csv";

    // Agora, passa o caminho completo para o gerenciador
    GerenciadorDeVeiculos& gerenciador = GerenciadorDeVeiculos::getInstance(csv_path);
    gerenciador.carregar_do_csv();

    vector<Veiculos*> teste = gerenciador.listar();
    for (size_t i = 0; i < teste.size(); i++)
    {
        // Corrigido: Estava imprimindo sempre o índice 0, alterado para i
        std::cout << teste[i]->getModelo() << std::endl;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

