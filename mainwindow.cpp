#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gerenciadores.h"
#include "veiculos.h"
#include "clientes.h"
#include "vendedor.h"
#include <iostream>
#include <QMessageBox>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Drive Tech");
    
    
}


MainWindow::~MainWindow()
{
    delete ui;
}

// Tela inicial -> Tela Login
void MainWindow::on_botLogin_clicked()
{
    telaLogin = new TelaLogin(this);

    telaLogin->setWindowTitle("Login...");
    telaLogin->show();
    this->hide();
}

