#ifndef TELALOGIN_H
#define TELALOGIN_H

#include <QDialog>
#include "telaprincipal.h"

namespace Ui {
class TelaLogin;
}

class TelaLogin : public QDialog
{
    Q_OBJECT

public:
    explicit TelaLogin(QWidget *parent = nullptr);
    ~TelaLogin();

private slots:

    void on_botOk_clicked();

    void on_botVoltar_clicked();

private:
    Ui::TelaLogin *ui;

    // declarando classe tela principal
    TelaPrincipal *telaPrincipal;
};

#endif // TELALOGIN_H
