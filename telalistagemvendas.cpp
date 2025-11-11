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

    ui->tabelaVendas->clearContents();
    ui->tabelaVendas->setRowCount(0);

    // Ordena por data (mais recente primeiro)
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

    // --- CORREÇÃO AQUI ---
    // Em vez de 'ResizeToContents', usamos 'Stretch' para todas as colunas
    // e 'Interactive' para o cabeçalho horizontal
    ui->tabelaVendas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // --- FIM DA CORREÇÃO ---


    for (int i = 0; i < (int)listaVendas.size(); ++i) {
        Vendas* venda = listaVendas[i];

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << venda->getValorFinal();
        std::string valorFinalStr = "R$ " + ss.str();

        ui->tabelaVendas->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(venda->getDataVendaString())));

        QTableWidgetItem *itemStatus = new QTableWidgetItem(QString::fromStdString(venda->getStatusVenda()));
        if (venda->getStatusVenda() == "Concluída") {
            itemStatus->setBackground(QBrush(QColor(144, 238, 144))); // Verde
        } else if (venda->getStatusVenda() == "Pendente") {
            itemStatus->setBackground(QBrush(QColor(255, 255, 128))); // Amarelo
        } else {
            itemStatus->setBackground(QBrush(QColor(255, 180, 180))); // Vermelho
        }
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
