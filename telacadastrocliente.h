#ifndef TELACADASTROCLIENTE_H
#define TELACADASTROCLIENTE_H

#include <QDialog>

namespace Ui {
class TelaCadastroCliente;
}

class TelaCadastroCliente : public QDialog
{
    Q_OBJECT

public:
    explicit TelaCadastroCliente(QWidget *parent = nullptr);
    ~TelaCadastroCliente();

private slots:
    void on_botVoltar_clicked();

private:
    Ui::TelaCadastroCliente *ui;
};

#endif // TELACADASTROCLIENTE_H
