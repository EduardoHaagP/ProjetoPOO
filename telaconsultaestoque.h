#ifndef TELACONSULTAESTOQUE_H
#define TELACONSULTAESTOQUE_H

#include <QDialog>
#include <vector> // Para usar std::vector
#include "veiculos.h" // Para saber o que é a classe Veiculos
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

private slots:
    void on_botVoltar_clicked();

    void on_botCadastrar_clicked();

private:
    Ui::TelaConsultaEstoque *ui;

    //metodos
    void configurarTabela();
    void popularTabela(const std::vector<Veiculos*>& veiculos);

};

#endif // TELACONSULTAESTOQUE_H
