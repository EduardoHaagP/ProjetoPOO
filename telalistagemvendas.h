#ifndef TELALISTAGEMVENDAS_H
#define TELALISTAGEMVENDAS_H

#include <QDialog>

namespace Ui {
class TelaListagemVendas;
}

class TelaListagemVendas : public QDialog
{
    Q_OBJECT

public:
    explicit TelaListagemVendas(QWidget *parent = nullptr);
    ~TelaListagemVendas();

private slots:
    void on_botVoltar_clicked();

private:
    Ui::TelaListagemVendas *ui;
};

#endif // TELALISTAGEMVENDAS_H
