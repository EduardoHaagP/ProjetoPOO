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
    
}

MainWindow::~MainWindow()
{
    delete ui;
}