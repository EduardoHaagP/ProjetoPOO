#ifndef VEICULO_H
#define VEICULO_H

#include <QObject>

class veiculo : public QObject
{
    Q_OBJECT
public:
    explicit veiculo(QObject *parent = nullptr);

signals:

};

#endif // VEICULO_H
