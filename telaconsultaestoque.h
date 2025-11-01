#ifndef TELACONSULTAESTOQUE_H
#define TELACONSULTAESTOQUE_H

#include <QDialog>

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

private:
    Ui::TelaConsultaEstoque *ui;
};

#endif // TELACONSULTAESTOQUE_H
