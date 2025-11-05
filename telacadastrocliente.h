#ifndef TELACADASTROCLIENTE_H
#define TELACADASTROCLIENTE_H

#include <QDialog>
#include <QMessageBox>
#include "clientes.h"
#include "gerenciadores.h"

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

    void on_checkFisica_stateChanged(int estadoCheck);

    void on_checkJuridica_stateChanged(int estadoCheck);

    void on_botCadastro_clicked();

    void on_botCadastro_2_clicked();

private:
    Ui::TelaCadastroCliente *ui;

    void resetarCamposErro();
    std::string limparFormatacao(std::string str);
};

#endif // TELACADASTROCLIENTE_H
