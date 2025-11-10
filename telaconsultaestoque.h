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


public slots:
    void on_botCadastrar_clicked(); // Esta função agora é pública

private slots:
    void on_botVoltar_clicked();
    // (A linha 'on_botCadastrar_clicked()' foi removida daqui)

private:
    Ui::TelaConsultaEstoque *ui;

    //metodos
    void configurarTabela();
    void popularTabela(const std::vector<Veiculos*>& veiculos);

};

#endif // TELACONSULTAESTOQUE_H
