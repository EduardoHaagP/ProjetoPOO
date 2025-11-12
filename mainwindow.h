#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent> 

#include "telacadastrocliente.h"
#include "telaconsultaestoque.h"
#include "telalistagemvendas.h"
#include "telaresgistrovendas.h"
#include "vendedor.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override; 
private slots:
    // tela inicial botoes
    void on_botEntrar_clicked();

    // tela login
    void on_botOk_clicked();



    //tela principal botoes
    void on_botSair_clicked();
    void on_botCadastroCliente_clicked();

    void on_botVenda_clicked();

    void on_botListaVendas_clicked();

    void on_botEstoque_clicked();

private:
    Ui::MainWindow *ui;

    TelaCadastroCliente *telaCadastroCliente;
    TelaConsultaEstoque *telaConsultaEstoque;
    TelaListagemVendas *telaListagemVendas;
    TelaResgistroVendas *telaResgistroVendas;

    Vendedor* vendedorAutenticado;

};
#endif // MAINWINDOW_H
