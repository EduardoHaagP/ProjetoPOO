#ifndef TELARESGISTROVENDAS_H
#define TELARESGISTROVENDAS_H

#include <QDialog>
#include <vector>
#include <QCompleter>

// --- DECLARAÇÕES ANTECIPADAS ---
class GerenciadorDeVendas;
class GerenciadorDeClientes;
class GerenciadorDeVeiculos;
class Vendedor;
class Clientes;
class Veiculos;
class PoliticaDesconto;
class Vendas;
class FabricaPoliticasDesconto;
class SemDesconto;
// --- FIM ---


namespace Ui {
class TelaResgistroVendas;
}

class TelaResgistroVendas : public QDialog
{
    Q_OBJECT

public:
    explicit TelaResgistroVendas(Vendedor* vendedor, QWidget *parent = nullptr);
    ~TelaResgistroVendas();

private slots:
    // --- BOTÕES PRINCIPAIS ---
    void on_botVoltar_clicked();
    void on_botCancelar_clicked();
    void on_confirmCliente_clicked();
    void on_confirmVeiculo_clicked();
    void on_confirmPagamento_clicked();
    void on_confirmResumo_clicked();

    // --- SLOTS DE ATUALIZAÇÃO ---
    void atualizarTabelaVeiculos();
    void atualizarCalculoPagamento();
    void atualizarCalculoParcelas();

    // Slots dos bugs
    void on_inpPag_currentTextChanged(const QString &text);
    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::TelaResgistroVendas *ui;

    // --- DADOS DA SESSÃO DE VENDA ---
    Vendedor* vendedorLogado;
    Clientes* clienteSelecionado;
    Veiculos* veiculoSelecionado;
    PoliticaDesconto* politicaSelecionada;
    std::vector<Veiculos*> veiculosFiltrados;

    // --- FUNÇÕES DE SETUP ---
    void estadoInicial();
    void carregarClientes();
    void carregarFiltrosVeiculo();
    void carregarOpcoesPagamento();
    void configurarTabela();
    void atualizarResumo();

    std::string determinarMelhorDesconto(Clientes* cliente);
};

#endif // TELARESGISTROVENDAS_H

