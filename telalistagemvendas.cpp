#include "telalistagemvendas.h"
#include "ui_telalistagemvendas.h"
#include "gerenciadores.h"
#include "vendas.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QBrush>
#include <QColor>
#include <algorithm>
#include <sstream>
#include <iomanip>

TelaListagemVendas::TelaListagemVendas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaListagemVendas)
{
    ui->setupUi(this);

    carregarVendasNaTabela();
}

TelaListagemVendas::~TelaListagemVendas()
{
    delete ui;
}

void TelaListagemVendas::on_botVoltar_clicked()
{
    this->close();
}

void TelaListagemVendas::carregarVendasNaTabela()
{
    GerenciadorDeVendas &manager = GerenciadorDeVendas::getInstance();
    std::vector<Vendas *> listaVendas = manager.listar();

    ui->tabelaVendas->clearContents();
    ui->tabelaVendas->setRowCount(0);

    std::sort(listaVendas.begin(), listaVendas.end(), [](Vendas* a, Vendas* b) {
        if (a->getDataVenda().getAno() != b->getDataVenda().getAno()) {
            return a->getDataVenda().getAno() > b->getDataVenda().getAno();
        }
        if (a->getDataVenda().getMes() != b->getDataVenda().getMes()) {
            return a->getDataVenda().getMes() > b->getDataVenda().getMes();
        }
        return a->getDataVenda().getDia() > b->getDataVenda().getDia();
    });

    int numColunas = 8;
    ui->tabelaVendas->setColumnCount(numColunas);
    ui->tabelaVendas->setRowCount(listaVendas.size());

    QStringList headers = {"Data", "Status", "Filial", "Vendedor (CPF)", "Cliente (Doc)", "Veículo", "Valor Final", "Desconto (%)"};
    ui->tabelaVendas->setHorizontalHeaderLabels(headers);

    ui->tabelaVendas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    for (int i = 0; i < (int)listaVendas.size(); ++i) {
        Vendas* venda = listaVendas[i];

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << venda->getValorFinal();
        std::string valorFinalStr = "R$ " + ss.str();

        ui->tabelaVendas->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(venda->getDataVendaString())));

        QTableWidgetItem *itemStatus = new QTableWidgetItem(QString::fromStdString(venda->getStatusVenda()));

        // --- CORES ESCURAS PARA O TEMA ---
        if (venda->getStatusVenda() == "Concluída") {
            itemStatus->setBackground(QBrush(QColor("#38761D"))); // Verde Escuro (Sucesso)
        } else if (venda->getStatusVenda() == "Pendente") {
            itemStatus->setBackground(QBrush(QColor("#B4A000"))); // Amarelo Escuro/Âmbar (Atenção)
        } else {
            itemStatus->setBackground(QBrush(QColor("#800000"))); // Vermelho Escuro/Marrom (Cancelado/Erro)
        }
        // --- FIM CORES ESCURAS ---

        ui->tabelaVendas->setItem(i, 1, itemStatus);

        ui->tabelaVendas->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(venda->getFilial())));

        // Vendedor (Nome + CPF)
        QString vendedorInfo = "N/A";
        if (venda->getVendedor()) {
            vendedorInfo = QString::fromStdString(venda->getVendedor()->getNome()) +
                           " (" + QString::fromStdString(venda->getVendedor()->getCpf()) + ")";
        }
        ui->tabelaVendas->setItem(i, 3, new QTableWidgetItem(vendedorInfo));

        // Cliente (Nome + Documento)
        QString clienteInfo = "N/A";
        if (venda->getCliente()) {
            clienteInfo = QString::fromStdString(venda->getCliente()->getNome()) +
                          " (" + QString::fromStdString(venda->getCliente()->getDocumento()) + ")";
        }
        ui->tabelaVendas->setItem(i, 4, new QTableWidgetItem(clienteInfo));

        // Veículo
        QString veiculoModelo = venda->getVeiculo() ? QString::fromStdString(venda->getVeiculo()->getModelo()) : "N/A";
        ui->tabelaVendas->setItem(i, 5, new QTableWidgetItem(veiculoModelo));

        // Valor Final
        ui->tabelaVendas->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(valorFinalStr)));

        // Desconto
        QString desconto = QString::number(venda->getPoliticaDesconto()->getPercentual(), 'f', 1) + "%";
        ui->tabelaVendas->setItem(i, 7, new QTableWidgetItem(desconto));
    }
}
