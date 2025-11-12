#include "telaresgistrovendas.h"
#include "ui_telaresgistrovendas.h"

#include "gerenciadores.h"
#include "vendedor.h"
#include "clientes.h"
#include "veiculos.h"
#include "descontos.h"
#include "vendas.h"

#include <QDebug>
#include <QCompleter>
#include <QStringListModel>
#include <QDate>
#include <QMessageBox>
#include <iomanip>
#include <sstream>

TelaResgistroVendas::TelaResgistroVendas(Vendedor* vendedor, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaResgistroVendas)
    , vendedorLogado(vendedor)
    , clienteSelecionado(nullptr)
    , veiculoSelecionado(nullptr)
    , politicaSelecionada(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Registro de Venda");

    connect(this, &QDialog::finished, [this](){
        delete politicaSelecionada;
    });

    configurarTabela();
    estadoInicial();

    connect(ui->inpFilial, &QComboBox::currentTextChanged, this, &TelaResgistroVendas::atualizarTabelaVeiculos);
    connect(ui->inpTipo, &QComboBox::currentTextChanged, this, &TelaResgistroVendas::atualizarTabelaVeiculos);
    connect(ui->inpModelo, &QLineEdit::textChanged, this, &TelaResgistroVendas::atualizarTabelaVeiculos);
    connect(ui->inpDesconto, &QComboBox::currentTextChanged, this, &TelaResgistroVendas::atualizarCalculoPagamento);
    connect(ui->inpEntrada, &QLineEdit::textChanged, this, &TelaResgistroVendas::atualizarCalculoPagamento);

    connect(ui->comboBox_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TelaResgistroVendas::on_comboBox_2_currentIndexChanged);
    connect(ui->inpPag, &QComboBox::currentTextChanged, this, &TelaResgistroVendas::on_inpPag_currentTextChanged);
    connect(ui->inpEntrada, &QLineEdit::textChanged, this, &TelaResgistroVendas::atualizarCalculoParcelas);
    connect(ui->inpParcelas, &QComboBox::currentTextChanged, this, &TelaResgistroVendas::atualizarCalculoParcelas);
}

TelaResgistroVendas::~TelaResgistroVendas()
{
    delete ui;
}

void TelaResgistroVendas::configurarTabela()
{
    ui->tableWidget->setColumnCount(5);
    QStringList headers = {"Tipo", "Modelo", "Ano", "Valor (R$)", "Cor"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TelaResgistroVendas::estadoInicial()
{
    clienteSelecionado = nullptr;
    veiculoSelecionado = nullptr;
    delete politicaSelecionada;
    politicaSelecionada = new SemDesconto(); 

    ui->cliente->setEnabled(true);
    ui->venda->setEnabled(false);
    ui->pagamento->setEnabled(false);
    ui->resumo->setEnabled(false);

    ui->erroCliente->setText("* Selecione o cliente");
    ui->erroCliente->setStyleSheet("color: #AAAAAA;"); 
    ui->erroVeiculo->setText("* Aguardando cliente...");
    ui->erroVeiculo->setStyleSheet("color: #AAAAAA;"); 

    ui->erroPagamento->setText("* Selecione a forma de pagamento e valores");
    ui->erroPagamento->setStyleSheet("color: #AAAAAA;"); 

    carregarClientes();
    carregarFiltrosVeiculo();
    carregarOpcoesPagamento();

    on_inpPag_currentTextChanged(ui->inpPag->currentText());
}

void TelaResgistroVendas::carregarClientes()
{
    QComboBox* comboClientes = ui->comboBox_2;
    comboClientes->clear();
    comboClientes->addItem("Selecione um cliente...");

    QStringListModel* model = new QStringListModel(this);
    QStringList listaNomes;

    std::vector<Clientes*> clientes = GerenciadorDeClientes::getInstance().listar();
    for (Clientes* c : clientes) {
        std::string textoItem = c->getNome() + " (" + c->getDocumento() + ")";
        comboClientes->addItem(QString::fromStdString(textoItem));
        listaNomes << QString::fromStdString(textoItem);
    }

    model->setStringList(listaNomes);
    QCompleter* completer = new QCompleter(this);
    completer->setModel(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);

    comboClientes->setEditable(true);
    comboClientes->setCompleter(completer);

    comboClientes->lineEdit()->clear();
}

void TelaResgistroVendas::carregarFiltrosVeiculo()
{
    ui->inpFilial->setCurrentIndex(0);
    ui->inpTipo->setCurrentIndex(0);
    ui->inpModelo->clear();
}

void TelaResgistroVendas::carregarOpcoesPagamento()
{
    ui->inpPag->clear();
    ui->inpPag->addItem("À vista");
    ui->inpPag->addItem("Parcelado");

    ui->inpPag->setCurrentIndex(-1);
    ui->inpPag->setPlaceholderText("Selecione...");

    ui->inpParcelas->clear();
    ui->inpParcelas->addItem("1x");
    for (int i = 2; i <= 24; ++i) {
        ui->inpParcelas->addItem(QString("%1x").arg(i));
    }

    ui->inpDesconto->clear();
    ui->inpDesconto->addItem("Sem desconto");

    ui->erroPagamento->setText("");
    ui->erroPagamento->setStyleSheet("color: #AAAAAA;");
}

std::string TelaResgistroVendas::determinarMelhorDesconto(Clientes* cliente)
{
    if (!cliente) {
        return "Sem desconto";
    }
    const auto& vendas = GerenciadorDeVendas::getInstance().listar();
    for (Vendas* v : vendas) {
        if (v->getCliente() && v->getCliente()->getDocumento() == cliente->getDocumento()) {
            return "Cliente Fidelidade";
        }
    }
    return "Sem desconto";
}



void TelaResgistroVendas::on_botVoltar_clicked()
{
    this->close();
}

void TelaResgistroVendas::on_botCancelar_clicked()
{
    estadoInicial();
}

void TelaResgistroVendas::on_confirmCliente_clicked()
{
    int indice = ui->comboBox_2->currentIndex();

    if (indice <= 0) {
        ui->erroCliente->setText("* ERRO: Cliente inválido!");
        ui->erroCliente->setStyleSheet("color: #FF5050;"); 
        return;
    }

    clienteSelecionado = GerenciadorDeClientes::getInstance().listar()[indice - 1];

    if (clienteSelecionado) {
        ui->comboBox_2->setCurrentText(QString::fromStdString(clienteSelecionado->getNome()));

        ui->erroCliente->setText("* Cliente Confirmado!");
        ui->erroCliente->setStyleSheet("color: #28A745;"); 

        ui->cliente->setEnabled(false);
        ui->venda->setEnabled(true);

        ui->erroVeiculo->setText("* Selecione o veículo");
        ui->erroVeiculo->setStyleSheet("color: #AAAAAA;"); 

        ui->inpDesconto->clear();
        ui->inpDesconto->addItem("Sem desconto");
        ui->inpDesconto->addItem("Promoção Especial");

        if (clienteSelecionado->getTipoCliente() == "PF") {
            ui->inpDesconto->addItem("Cliente Fidelidade");
        } else { // "PJ"
            ui->inpDesconto->addItem("Cliente Fidelidade");
            ui->inpDesconto->addItem("Vendas Corporativas");
        }

        std::string descontoSugerido = determinarMelhorDesconto(clienteSelecionado);
        ui->inpDesconto->setCurrentText(QString::fromStdString(descontoSugerido));

        atualizarTabelaVeiculos();
    }
}

void TelaResgistroVendas::on_confirmVeiculo_clicked()
{
    int linha = ui->tableWidget->currentRow();

    if (linha < 0 || linha >= (int)veiculosFiltrados.size()) {
        
        ui->erroVeiculo->setText("* ERRO: Selecione um veículo na tabela!");
        ui->erroVeiculo->setStyleSheet("color: #FF5050;"); 
        return;
    }

    veiculoSelecionado = veiculosFiltrados[linha];

    if (veiculoSelecionado) {

        ui->erroVeiculo->setText("* Veículo Confirmado!");
        ui->erroVeiculo->setStyleSheet("color: #28A745;"); 

        ui->venda->setEnabled(false);
        ui->pagamento->setEnabled(true);

        ui->erroPagamento->setText("* Verifique os valores e confirme.");
        ui->erroPagamento->setStyleSheet("color: #AAAAAA;");

        atualizarCalculoPagamento();
    }
}

void TelaResgistroVendas::on_confirmPagamento_clicked()
{
    ui->erroPagamento->setText("* Verificando...");
    ui->erroPagamento->setStyleSheet("color: #AAAAAA;");

    if (ui->inpPag->currentIndex() == -1) {
        ui->erroPagamento->setText("* ERRO: Selecione uma forma de pagamento.");
        ui->erroPagamento->setStyleSheet("color: #FF5050;"); 
        return;
    }

    if (ui->inpPag->currentText() == "Parcelado") {
        bool ok;
        float entrada = ui->inpEntrada->text().toFloat(&ok);
        float valorTotal = 0.0;
        if (politicaSelecionada && veiculoSelecionado) {
            valorTotal = politicaSelecionada->calcularDesconto(veiculoSelecionado->getValorBase());
        }

        if (!ok || entrada < 0) {
            ui->erroPagamento->setText("* ERRO: Valor de entrada inválido.");
            ui->erroPagamento->setStyleSheet("color: #FF5050;"); 
            return;
        }
        if (entrada > valorTotal) {
            ui->erroPagamento->setText("* ERRO: Entrada não pode ser maior que o total.");
            ui->erroPagamento->setStyleSheet("color: #FF5050;"); 
            return;
        }
    }

    ui->erroPagamento->setText("* Pagamento Confirmado!");
    ui->erroPagamento->setStyleSheet("color: #28A745;"); 

    ui->pagamento->setEnabled(false);
    ui->resumo->setEnabled(true);
    atualizarResumo();
}

void TelaResgistroVendas::on_confirmResumo_clicked() 
{
    if (!vendedorLogado || !clienteSelecionado || !veiculoSelecionado || !politicaSelecionada) {
        QMessageBox::critical(this, "Erro", "Dados da venda incompletos. Não foi possível registrar.");
        return;
    }

    float valorBase = veiculoSelecionado->getValorBase();
    float valorFinal = politicaSelecionada->calcularDesconto(valorBase);
    float valorEntrada = 0.0;
    string formaPagamento = ui->inpPag->currentText().toStdString();

    if (formaPagamento == "Parcelado") {
        valorEntrada = ui->inpEntrada->text().toFloat();
    } else {
        valorEntrada = valorFinal;
    }

    string statusVenda = (valorEntrada >= valorFinal) ? "Concluída" : "Pendente";
    Data dataVenda(QDate::currentDate().day(), QDate::currentDate().month(), QDate::currentDate().year());
    string filialVenda = veiculoSelecionado->getFilial();

    Veiculos* veiculoParaVenda = nullptr;
    string tipoVeiculo = veiculoSelecionado->motoOuCarro();

    if (tipoVeiculo == "Moto") {
        veiculoParaVenda = new Moto(
            veiculoSelecionado->getModelo(),
            veiculoSelecionado->getAno(),
            veiculoSelecionado->getValorBase(), 
            veiculoSelecionado->getCor(),
            veiculoSelecionado->getFilial()
            );
    } else { 
        veiculoParaVenda = new Carro(
            veiculoSelecionado->getModelo(),
            veiculoSelecionado->getAno(),
            veiculoSelecionado->getValorBase(), 
            veiculoSelecionado->getCor(),
            veiculoSelecionado->getFilial()
            );
    }


    Vendas* novaVenda = new Vendas(
        vendedorLogado,
        clienteSelecionado,
        veiculoParaVenda,
        valorBase,
        politicaSelecionada,
        valorEntrada,
        formaPagamento,
        statusVenda,
        dataVenda,
        filialVenda
        );

    novaVenda->setValorFinal(valorFinal);
    GerenciadorDeVendas::getInstance().adicionar(novaVenda);

    bool removido = GerenciadorDeVeiculos::getInstance().remover_por_modelo(veiculoSelecionado->getModelo());
    if (!removido) {
        qWarning() << "ATENÇÃO: Venda registrada, mas não foi possível remover o veículo do estoque.";
    }

    QMessageBox::information(this, "Sucesso", "Venda registrada com sucesso!");

    politicaSelecionada = new SemDesconto(); 
    this->close();
}



void TelaResgistroVendas::atualizarTabelaVeiculos()
{
    QString filialFiltro = ui->inpFilial->currentText();
    QString tipoFiltro = ui->inpTipo->currentText();
    QString modeloFiltro = ui->inpModelo->text();
    std::vector<Veiculos*> todosVeiculos = GerenciadorDeVeiculos::getInstance().listar();

    veiculosFiltrados.clear();
    ui->tableWidget->setRowCount(0);
    for (Veiculos* veiculo : todosVeiculos) {
        bool verificaFilial = (filialFiltro == "Todas" ||
                               QString::fromStdString(veiculo->getFilial()) == filialFiltro);
        bool verificaTipo = (tipoFiltro == "Todos" ||
                             QString::fromStdString(veiculo->motoOuCarro()) == tipoFiltro);
        bool verificaModelo = (modeloFiltro.isEmpty() ||
                               QString::fromStdString(veiculo->getModelo()).contains(modeloFiltro, Qt::CaseInsensitive));
        if (verificaFilial && verificaTipo && verificaModelo) {
            veiculosFiltrados.push_back(veiculo);
        }
    }
    for (Veiculos* veiculo : veiculosFiltrados) {
        int linhaAtual = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(linhaAtual);
        ui->tableWidget->setItem(linhaAtual, 0, new QTableWidgetItem(QString::fromStdString(veiculo->motoOuCarro())));
        ui->tableWidget->setItem(linhaAtual, 1, new QTableWidgetItem(QString::fromStdString(veiculo->getModelo())));
        ui->tableWidget->setItem(linhaAtual, 2, new QTableWidgetItem(QString::number(veiculo->getAno())));
        QString valorFormatado = QString("R$ %1").arg(veiculo->getValorBase(), 0, 'f', 2);
        ui->tableWidget->setItem(linhaAtual, 3, new QTableWidgetItem(valorFormatado));
        ui->tableWidget->setItem(linhaAtual, 4, new QTableWidgetItem(QString::fromStdString(veiculo->getCor())));
    }

    ui->erroVeiculo->setText(QString("%1 veículo(s) encontrado(s)").arg(veiculosFiltrados.size()));
    ui->erroVeiculo->setStyleSheet("color: #F0F0F0;"); 
}


void TelaResgistroVendas::atualizarCalculoPagamento()
{
    if (!veiculoSelecionado) return;

    float valorBase = veiculoSelecionado->getValorBase();
    ui->txtValor->setText(QString("R$ %1").arg(valorBase, 0, 'f', 2));

    std::string tipoDesconto = ui->inpDesconto->currentText().toStdString();

    delete politicaSelecionada;
    politicaSelecionada = FabricaPoliticasDesconto::criarPolitica(tipoDesconto);

    float valorFinal = politicaSelecionada->calcularDesconto(valorBase);

    ui->txtDesconto->setText(QString::number(politicaSelecionada->getPercentual(), 'f', 1) + "%");
    ui->txtTotalPag->setText(QString("R$ %1").arg(valorFinal, 0, 'f', 2));

    if (ui->pagamento->isEnabled()) {
        ui->erroPagamento->setText("* Verifique os valores e confirme.");
        ui->erroPagamento->setStyleSheet("color: #AAAAAA;");
    }

    atualizarCalculoParcelas();
}

void TelaResgistroVendas::atualizarResumo()
{
    if (!clienteSelecionado || !veiculoSelecionado) return;
    ui->txtNomeResumo->setText(QString::fromStdString(clienteSelecionado->getNome()));
    ui->txtModeloResumo->setText(QString::fromStdString(veiculoSelecionado->getModelo()));
    ui->txtTotalResumo->setText(ui->txtTotalPag->text());
}


void TelaResgistroVendas::on_inpPag_currentTextChanged(const QString &text)
{
    bool visible = (text == "Parcelado");
    ui->label_35->setVisible(visible);
    ui->inpEntrada->setVisible(visible);
    ui->label_31->setVisible(visible);
    ui->inpParcelas->setVisible(visible);
    ui->txtValorParcela->setVisible(visible); 

    ui->erroPagamento->setText("* Verifique os valores e confirme.");
    ui->erroPagamento->setStyleSheet("color: #AAAAAA;");
}

void TelaResgistroVendas::on_comboBox_2_currentIndexChanged(int index)
{
    if (index <= 0) {
        ui->txtTipoCliente->clear();
        ui->txtTipoCliente->setText("Tipo cliente");
    } else {
        Clientes* c = GerenciadorDeClientes::getInstance().listar()[index - 1];
        if (c) {
            ui->txtTipoCliente->setText(QString::fromStdString(c->getTipoCliente()));
        }
    }
}


void TelaResgistroVendas::atualizarCalculoParcelas()
{
    if (!veiculoSelecionado || !politicaSelecionada) {
        ui->txtValorParcela->setText("R$ 0.00"); 
        return;
    }

    float valorFinal = politicaSelecionada->calcularDesconto(veiculoSelecionado->getValorBase());

    float entrada = 0.0;
    if (ui->inpPag->currentText() == "Parcelado") {
        entrada = ui->inpEntrada->text().toFloat();
    }

    QString textoParcela = ui->inpParcelas->currentText();
    textoParcela.remove("x"); 
    int numParcelas = textoParcela.toInt();
    if (numParcelas == 0) numParcelas = 1; 

    float valorAPrazo = valorFinal - entrada;
    if (valorAPrazo < 0) valorAPrazo = 0; 

    float valorParcela = valorAPrazo / numParcelas;

    ui->txtValorParcela->setText(QString("R$ %1").arg(valorParcela, 0, 'f', 2));
}
