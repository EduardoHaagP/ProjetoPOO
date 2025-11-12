#ifndef TELACONSULTAESTOQUE_H
#define TELACONSULTAESTOQUE_H

#include <QDialog>
#include <vector> 
#include "veiculos.h" 
class QTableWidget;


namespace Ui {
class TelaConsultaEstoque;
}

class TelaConsultaEstoque : public QDialog
{
    Q_OBJECT

public:
    explicit TelaConsultaEstoque(QWidget *parent = nullptr);
    ~TelaConsultaEstoque();


public slots:
    void on_botCadastrar_clicked();

private slots:
    void on_botVoltar_clicked();

private:
    Ui::TelaConsultaEstoque *ui;

    //metodos
    void configurarTabela();
    void popularTabela(const std::vector<Veiculos*>& veiculos);

};

#endif // TELACONSULTAESTOQUE_H
