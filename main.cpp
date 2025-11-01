#include "mainwindow.h"
#include "telaprincipal.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    TelaPrincipal telaPrincipal;
    w.show();
    return a.exec();
}
