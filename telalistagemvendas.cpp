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

    // CHAMA A FUNÇÃO DE CARREGAMENTO NO CONSTRUTOR
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

    // limpa tabela
    ui->tabelaVendas->clearContents();
    ui->tabelaVendas->setRowCount(0);

    // ordena por data
    std::sort(listaVendas.begin(), listaVendas.end(), [](Vendas* a, Vendas* b) {
        if (a->getDataVenda().getAno() != b->getDataVenda().getAno()) {
            return a->getDataVenda().getAno() > b->getDataVenda().getAno();
        }
        if (a->getDataVenda().getMes() != b->getDataVenda().getMes()) {
            return a->getDataVenda().getMes() > b->getDataVenda().getMes();
        }
        return a->getDataVenda().getDia() > b->getDataVenda().getDia();
    });

    // config da tabela
    int numColunas = 8;
    ui->tabelaVendas->setColumnCount(numColunas);
    ui->tabelaVendas->setRowCount(listaVendas.size());

    // cabecalho
    QStringList headers = {"Data", "Status", "Filial", "Vendedor (CPF)", "Cliente (Doc)", "Veículo", "Valor Final", "Desconto (%)"};
    ui->tabelaVendas->setHorizontalHeaderLabels(headers);

    // ajusta largura
    ui->tabelaVendas->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tabelaVendas->horizontalHeader()->setStretchLastSection(true);

    // preenche
    for (int i = 0; i < (int)listaVendas.size(); ++i) {
        Vendas* venda = listaVendas[i];

        // formatacao
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << venda->getValorFinal();
        std::string valorFinalStr = "R$ " + ss.str();

        // data
        ui->tabelaVendas->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(venda->getDataVendaString())));

        // status item
        QTableWidgetItem *itemStatus = new QTableWidgetItem(QString::fromStdString(venda->getStatusVenda()));

        // cor de fundo para status
        if (venda->getStatusVenda() == "Concluída") {
            itemStatus->setBackground(QBrush(QColor(144, 238, 144))); // Verde claro
        } else if (venda->getStatusVenda() == "Pendente") {
            itemStatus->setBackground(QBrush(QColor(255, 255, 128))); // Amarelo
        } else {
            itemStatus->setBackground(QBrush(QColor(255, 180, 180))); // Vermelho claro
        }
        ui->tabelaVendas->setItem(i, 1, itemStatus);

        // filial
        ui->tabelaVendas->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(venda->getFilial())));

        //vendedor
        QString vendedorCpf = venda->getVendedor() ? QString::fromStdString(venda->getVendedor()->getCpf()) : "N/A";
        ui->tabelaVendas->setItem(i, 3, new QTableWidgetItem(vendedorCpf));

        // cliente
        QString clienteDoc = venda->getCliente() ? QString::fromStdString(venda->getCliente()->getDocumento()) : "N/A";
        ui->tabelaVendas->setItem(i, 4, new QTableWidgetItem(clienteDoc));

        // veiculo
        QString veiculoModelo = venda->getVeiculo() ? QString::fromStdString(venda->getVeiculo()->getModelo()) : "N/A";
        ui->tabelaVendas->setItem(i, 5, new QTableWidgetItem(veiculoModelo));

        // valor final
        ui->tabelaVendas->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(valorFinalStr)));

        // desconto
        QString desconto = QString::number(venda->getPoliticaDesconto()->getPercentual(), 'f', 1) + "%";
        ui->tabelaVendas->setItem(i, 7, new QTableWidgetItem(desconto));
    }
}
