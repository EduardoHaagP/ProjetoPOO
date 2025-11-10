#include "telaresgistrovendas.h"
#include "ui_telaresgistrovendas.h"

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

    // Limpa a memória da política ao fechar
    connect(this, &QDialog::finished, [this](){
        delete politicaSelecionada;
    });

    configurarTabela();
    estadoInicial();

    // --- CONECTA OS SINAIS AOS SLOTS ---
    connect(ui->inpFilial, &QComboBox::currentTextChanged, this, &TelaResgistroVendas::atualizarTabelaVeiculos);
    connect(ui->inpTipo, &QComboBox::currentTextChanged, this, &TelaResgistroVendas::atualizarTabelaVeiculos);
    connect(ui->inpModelo, &QLineEdit::textChanged, this, &TelaResgistroVendas::atualizarTabelaVeiculos);

    // Conecta os campos de pagamento à atualização de cálculo
    connect(ui->inpDesconto, &QComboBox::currentTextChanged, this, &TelaResgistroVendas::atualizarCalculoPagamento);
    connect(ui->inpEntrada, &QLineEdit::textChanged, this, &TelaResgistroVendas::atualizarCalculoPagamento);
}

TelaResgistroVendas::~TelaResgistroVendas()
{
    delete ui;
}

// --- FUNÇÕES DE SETUP ---

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

    // Habilita/Desabilita os blocos
    ui->cliente->setEnabled(true);
    ui->venda->setEnabled(false);
    ui->pagamento->setEnabled(false);
    ui->resumo->setEnabled(false);

    // Reseta textos de status
    ui->erroCliente->setText("* Selecione o cliente");
    ui->erroVeiculo->setText("* Aguardando cliente...");

    // Carrega dados iniciais
    carregarClientes();
    carregarFiltrosVeiculo();
    carregarOpcoesPagamento();
}

void TelaResgistroVendas::carregarClientes()
{
    QComboBox* comboClientes = ui->comboBox_2; // ComboBox de cliente
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

    // --- LÓGICA DO AUTO-COMPLETE ---
    model->setStringList(listaNomes);
    QCompleter* completer = new QCompleter(this);
    completer->setModel(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);

    comboClientes->setEditable(true);
    comboClientes->setCompleter(completer);
}

void TelaResgistroVendas::carregarFiltrosVeiculo()
{
    ui->inpFilial->setCurrentIndex(0);
    ui->inpTipo->setCurrentIndex(0);
    ui->inpModelo->clear();
}

void TelaResgistroVendas::carregarOpcoesPagamento()
{
    // Forma de Pagamento (inpPag)
    ui->inpPag->clear();
    ui->inpPag->addItem("À vista");
    ui->inpPag->addItem("Parcelado");

    // Parcelas (inpParcelas)
    ui->inpParcelas->clear();
    ui->inpParcelas->addItem("1x");
    for (int i = 2; i <= 24; ++i) {
        ui->inpParcelas->addItem(QString("%1x").arg(i));
    }

    // Popula o ComboBox 'inpDesconto'
    ui->inpDesconto->clear();
    ui->inpDesconto->addItem("Sem desconto");
    ui->inpDesconto->addItem("Cliente Fidelidade");
    ui->inpDesconto->addItem("Promoção Especial");
    ui->inpDesconto->addItem("Vendas Corporativas");
}

// --- LÓGICA DE DESCONTO AUTOMÁTICO ---
std::string TelaResgistroVendas::determinarMelhorDesconto(Clientes* cliente)
{
    if (!cliente) {
        return "Sem desconto";
    }

    // ==========================================================
    // CORREÇÃO 1 AQUI:
    // Mudei de 'auto&' para 'const auto&'.
    // A função listar() retorna uma *cópia* (rvalue), e C++
    // não permite ligar uma referência não-constante (&) a um
    // valor temporário. 'const &' resolve isso.
    // ==========================================================
    const auto& vendas = GerenciadorDeVendas::getInstance().listar();

    for (Vendas* v : vendas) {
        if (v->getCliente() && v->getCliente()->getDocumento() == cliente->getDocumento()) {
            return "Cliente Fidelidade";
        }
    }

    return "Sem desconto";
}


// --- SLOTS DE AÇÃO (BOTÕES) ---

void TelaResgistroVendas::on_botVoltar_clicked()
{
    this->close();
}

void TelaResgistroVendas::on_botCancelar_clicked()
{
    estadoInicial(); // Reseta a tela
}

void TelaResgistroVendas::on_confirmCliente_clicked()
{
    int indice = ui->comboBox_2->currentIndex();

    if (indice <= 0) {
        ui->erroCliente->setText("* ERRO: Cliente inválido!");
        return;
    }

    clienteSelecionado = GerenciadorDeClientes::getInstance().listar()[indice - 1];

    if (clienteSelecionado) {
        ui->txtTipoCliente->setText(QString::fromStdString(clienteSelecionado->getTipoCliente()));
        ui->erroCliente->setText("* Cliente Confirmado!");

        ui->cliente->setEnabled(false);
        ui->venda->setEnabled(true);
        ui->erroVeiculo->setText("* Selecione o veículo");

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
        return;
    }

    veiculoSelecionado = veiculosFiltrados[linha];

    if (veiculoSelecionado) {
        ui->erroVeiculo->setText("* Veículo Confirmado!");

        ui->venda->setEnabled(false);
        ui->pagamento->setEnabled(true);

        atualizarCalculoPagamento();
    }
}

void TelaResgistroVendas::on_confirmPagamento_clicked()
{
    bool ok;
    float entrada = ui->inpEntrada->text().toFloat(&ok);

    float valorTotal = 0.0;
    if (politicaSelecionada && veiculoSelecionado) {
        valorTotal = politicaSelecionada->calcularDesconto(veiculoSelecionado->getValorBase());
    }

    if (!ok || entrada < 0) {
        QMessageBox::warning(this, "Erro", "Valor de entrada inválido.");
        return;
    }
    if (entrada > valorTotal) {
        QMessageBox::warning(this, "Erro", "Valor de entrada não pode ser maior que o valor total.");
        return;
    }

    ui->pagamento->setEnabled(false);
    ui->resumo->setEnabled(true);

    atualizarResumo();
}

void TelaResgistroVendas::on_confirmResumo_clicked() // REGISTRAR VENDA
{
    if (!vendedorLogado || !clienteSelecionado || !veiculoSelecionado || !politicaSelecionada) {
        QMessageBox::critical(this, "Erro", "Dados da venda incompletos. Não foi possível registrar.");
        return;
    }

    float valorBase = veiculoSelecionado->getValorBase();
    float valorFinal = politicaSelecionada->calcularDesconto(valorBase);
    float valorEntrada = ui->inpEntrada->text().toFloat();
    string formaPagamento = ui->inpPag->currentText().toStdString();

    string statusVenda = (valorEntrada >= valorFinal) ? "Concluída" : "Pendente";

    Data dataVenda(QDate::currentDate().day(), QDate::currentDate().month(), QDate::currentDate().year());
    string filialVenda = veiculoSelecionado->getFilial();

    Vendas* novaVenda = new Vendas(
        vendedorLogado,
        clienteSelecionado,
        veiculoSelecionado,
        valorBase,
        politicaSelecionada,
        valorEntrada,
        formaPagamento,
        statusVenda,
        dataVenda,
        filialVenda
        );

    novaVenda->setValorFinal(valorFinal); // Garante que o valor com desconto seja salvo

    GerenciadorDeVendas::getInstance().adicionar(novaVenda);

    bool removido = GerenciadorDeVeiculos::getInstance().remover_por_modelo(veiculoSelecionado->getModelo());
    if (!removido) {
        qWarning() << "ATENÇÃO: Venda registrada, mas não foi possível remover o veículo do estoque.";
    }

    QMessageBox::information(this, "Sucesso", "Venda registrada com sucesso!");

    politicaSelecionada = new SemDesconto(); // Evita double-delete

    this->close();
}


// --- SLOTS DE ATUALIZAÇÃO (LOGICA INTERNA) ---

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

    // ==========================================================
    // CORREÇÃO 2 AQUI:
    // O compilador sugeriu que o nome do objeto era 'txtDesconto'.
    // Mudei 'txtTipoDesconto' para 'txtDesconto' para bater
    // com o que o seu compilador espera.
    // ==========================================================
    ui->txtDesconto->setText(QString::number(politicaSelecionada->getPercentual(), 'f', 1) + "%");

    ui->txtTotalPag->setText(QString("R$ %1").arg(valorFinal, 0, 'f', 2));
}

void TelaResgistroVendas::atualizarResumo()
{
    if (!clienteSelecionado || !veiculoSelecionado) return;

    ui->txtNomeResumo->setText(QString::fromStdString(clienteSelecionado->getNome()));
    ui->txtModeloResumo->setText(QString::fromStdString(veiculoSelecionado->getModelo()));
    ui->txtTotalResumo->setText(ui->txtTotalPag->text()); // Pega o valor do label "Total"
}
