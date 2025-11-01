#ifndef TELAPRINCIPAL_H
#define TELAPRINCIPAL_H

#include <QDialog>
#include "telacadastrocliente.h"
#include "telaconsultaestoque.h"
#include "telalistagemvendas.h"
#include "telaresgistrovendas.h"

namespace Ui {
class TelaPrincipal;
}

class TelaPrincipal : public QDialog
{
    Q_OBJECT

public:
    explicit TelaPrincipal(QWidget *parent = nullptr);
    ~TelaPrincipal();

private slots:
    void on_botSair_clicked();

    void on_botCadastroCliente_clicked();

    void on_botVenda_clicked();

    void on_botListaVendas_clicked();

    void on_botEstoque_clicked();

private:
    Ui::TelaPrincipal *ui;

    TelaCadastroCliente *telaCadastroCliente;
    TelaConsultaEstoque *telaConsultaEstoque;
    TelaListagemVendas  *telaListagemVendas;
    TelaResgistroVendas *telaResgistroVendas;
};

#endif // TELAPRINCIPAL_H
