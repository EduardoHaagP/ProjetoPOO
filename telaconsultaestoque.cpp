#include "telaconsultaestoque.h"
#include "ui_telaconsultaestoque.h"
#include "gerenciadores.h" 
#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QDebug> 


TelaConsultaEstoque::TelaConsultaEstoque(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaConsultaEstoque)
{
    ui->setupUi(this);

    configurarTabela();

    // conecta bot ao slot
    connect(ui->botCadastrar, &QPushButton::clicked, this, &TelaConsultaEstoque::on_botCadastrar_clicked);

    // faz uma consulta inicial p/ carregar todos os veiculos
    on_botCadastrar_clicked();

}

TelaConsultaEstoque::~TelaConsultaEstoque()
{
    delete ui;
}

void TelaConsultaEstoque::configurarTabela()
{
    // 5 colunas
    ui->tabelaWidget->setColumnCount(5);

    // header
    QStringList headers = {"Tipo", "Modelo", "Ano", "Valor (R$)", "Cor"};
    ui->tabelaWidget->setHorizontalHeaderLabels(headers);

    // desativa edicao
    ui->tabelaWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // pode selecionar a linha inteira
    ui->tabelaWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}


void TelaConsultaEstoque::popularTabela(const std::vector<Veiculos*>& veiculos)
{
    // limpa tabela antiga
    ui->tabelaWidget->setRowCount(0);

    // adiciona cada um como nova linha
    for (Veiculos* veiculo : veiculos) {
        int linhaAtual = ui->tabelaWidget->rowCount();
        ui->tabelaWidget->insertRow(linhaAtual);

        // --- Cria os Itens para a Tabela ---
        QTableWidgetItem *itemTipo = new QTableWidgetItem(QString::fromStdString(veiculo->motoOuCarro()));
        QTableWidgetItem *itemModelo = new QTableWidgetItem(QString::fromStdString(veiculo->getModelo()));
        QTableWidgetItem *itemAno = new QTableWidgetItem(QString::number(veiculo->getAno()));

        QString valorFormatado = QString("R$ %1").arg(veiculo->getValorBase(), 0, 'f', 2); // formato R$ 90000.00

        QTableWidgetItem *itemValor = new QTableWidgetItem(valorFormatado);
        QTableWidgetItem *itemCor = new QTableWidgetItem(QString::fromStdString(veiculo->getCor()));

        // --- Adiciona os Itens na Linha Atual ---
        ui->tabelaWidget->setItem(linhaAtual, 0, itemTipo);
        ui->tabelaWidget->setItem(linhaAtual, 1, itemModelo);
        ui->tabelaWidget->setItem(linhaAtual, 2, itemAno);
        ui->tabelaWidget->setItem(linhaAtual, 3, itemValor);
        ui->tabelaWidget->setItem(linhaAtual, 4, itemCor);
    }
  }






void TelaConsultaEstoque::on_botVoltar_clicked()
{
    this->close();
}


void TelaConsultaEstoque::on_botCadastrar_clicked()
{
    //define os filtros
    QString filialFiltro = ui->inpFilial->currentText();
    QString tipoFiltro = ui->inpTipo->currentText();
    QString modeloFiltro = ui->inpModelo->text();

    qDebug() << "Consulta iniciada. Filtro Filial (ignorado):" << filialFiltro;

    // lista para todos veiculos
    std::vector<Veiculos*> todosVeiculos = GerenciadorDeVeiculos::getInstance().listar();

    // lista para os veiculos filtrados
    std::vector<Veiculos*> veiculosFiltrados;

    // aplica os filtros
    for (Veiculos* veiculo : todosVeiculos) {
        // verifica se é todos
        bool verificaFilial = (filialFiltro == "Todas" ||
                            QString::fromStdString(veiculo->getFilial()) == filialFiltro);

        bool verificaTipo = (tipoFiltro == "Todos" ||
                          QString::fromStdString(veiculo->motoOuCarro()) == tipoFiltro);

        // filtro de modelo sem diferencial maiuscula e minuscula
        bool verificaModelo = (modeloFiltro.isEmpty() ||
                            QString::fromStdString(veiculo->getModelo()).contains(modeloFiltro, Qt::CaseInsensitive));

        // se der bom, coloca na lista
        if (verificaTipo && verificaModelo && verificaFilial) {
            veiculosFiltrados.push_back(veiculo);
        }
    }

    // coloca na tabela os filtrados
    popularTabela(veiculosFiltrados);



}
















