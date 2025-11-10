#ifndef TELARESGISTROVENDAS_H
#define TELARESGISTROVENDAS_H

#include <QDialog>
#include <vector>
#include <QCompleter> // Para o AutoComplete

#include "gerenciadores.h"
#include "vendedor.h"
#include "clientes.h"
#include "veiculos.h"
#include "descontos.h"
#include "vendas.h"


namespace Ui {
class TelaResgistroVendas;
}

class TelaResgistroVendas : public QDialog
{
    Q_OBJECT

public:
    // O construtor continua recebendo o vendedor que fez o login
    explicit TelaResgistroVendas(Vendedor* vendedor, QWidget *parent = nullptr);
    ~TelaResgistroVendas();

private slots:
    // --- BOTÕES PRINCIPAIS ---
    void on_botVoltar_clicked();
    void on_botCancelar_clicked();      // Nome atualizado
    void on_confirmCliente_clicked();   // Nome atualizado
    void on_confirmVeiculo_clicked();   // Nome atualizado
    void on_confirmPagamento_clicked(); // Nome atualizado
    void on_confirmResumo_clicked();    // Nome atualizado

    // --- SLOTS DE ATUALIZAÇÃO ---
    void atualizarTabelaVeiculos();
    void atualizarCalculoPagamento();

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
