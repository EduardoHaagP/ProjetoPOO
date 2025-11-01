#ifndef TELARESGISTROVENDAS_H
#define TELARESGISTROVENDAS_H

#include <QDialog>

namespace Ui {
class TelaResgistroVendas;
}

class TelaResgistroVendas : public QDialog
{
    Q_OBJECT

public:
    explicit TelaResgistroVendas(QWidget *parent = nullptr);
    ~TelaResgistroVendas();

private slots:
    void on_botVoltar_clicked();

private:
    Ui::TelaResgistroVendas *ui;
};

#endif // TELARESGISTROVENDAS_H
