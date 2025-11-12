#include "telacadastrocliente.h"
#include "ui_telacadastrocliente.h"
#include <regex>


TelaCadastroCliente::TelaCadastroCliente(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TelaCadastroCliente)
{
    ui->setupUi(this);

    ui->botCadastro->setEnabled(true);

    connect(ui->inpDocumento, &QLineEdit::textChanged, this, [this](){
        ui->erroDocumento->setText("");
    });
    connect(ui->inpEmail, &QLineEdit::textChanged, this, [this](){
        ui->erroEmail->setText("");
    });
    connect(ui->inpNome, &QLineEdit::textChanged, this, [this](){
        ui->erroNome->setText("");
    });
    connect(ui->inpTelefone, &QLineEdit::textChanged, this, [this](){
        ui->erroTelefone->setText("");
    });
    connect(ui->inpEndereco, &QLineEdit::textChanged, this, [this](){
        ui->erroTelefone->setText("");
    });
}

TelaCadastroCliente::~TelaCadastroCliente()
{
    delete ui;
}

void TelaCadastroCliente::on_botVoltar_clicked()
{
    this->close();
}

// check fisica
void TelaCadastroCliente::on_checkFisica_stateChanged(int estadoCheck)
{

    if (estadoCheck == Qt::Checked) {
        // dermarca juridica
        ui->checkJuridica->setChecked(false);
        ui->txtDocumento->setText("CPF");
    }
    else {
        if (!ui->checkJuridica->isChecked()) {
            ui->txtDocumento->setText("Documento");
        }
    }
}

//check juridica
void TelaCadastroCliente::on_checkJuridica_stateChanged(int estadoCheck)
{
    if(estadoCheck == Qt::Checked){
        ui->checkFisica->setChecked(false);
        ui->txtDocumento->setText("CNPJ");
    }
    else {
        if (!ui->checkFisica->isChecked()) {
            ui->txtDocumento->setText("Documento");
        }
    }
}


// bot cadastrar cliente
void TelaCadastroCliente::on_botCadastro_clicked()
{
    resetarCamposErro();

    // pega os dados
    std::string nome = ui->inpNome->text().toStdString();
    std::string documento = ui->inpDocumento->text().toStdString();
    std::string telefone = ui->inpTelefone->text().toStdString();
    std::string email = ui->inpEmail->text().toStdString();
    std::string endereco = ui->inpEndereco->text().toStdString();

    bool ehPF = ui->checkFisica->isChecked();
    bool ehPJ = ui->checkJuridica->isChecked();

    bool tudoValido = true; // valida tudo


    // valida tipo
    if (!ehPF && !ehPJ) {
        ui->erroTipo->setText("Selecione Pessoa Física ou Jurídica");
        tudoValido = false;
    }else {
        ui->erroTipo->setStyleSheet("color: green;");
        ui->erroTipo->setText("Válido");
    }

    // valida nome
    if (nome.empty()) {
        ui->erroNome->setText("Campo obrigatório");
        tudoValido = false;
    } else if (!std::regex_match(nome, std::regex("^[a-zA-ZÀ-ú\\s]+$"))) { // permite letras, estaco e acento
        ui->erroNome->setText("Nome deve conter apenas letras e espaços");
        tudoValido = false;
    } else {
        ui->erroNome->setStyleSheet("color: green;");
        ui->erroNome->setText("Válido");
    }

    // limpa formatacao doc e telefone
    std::string docLimpo = limparFormatacao(documento);
    std::string telLimpo = limparFormatacao(telefone);

    // valida doc
    if (documento.empty()) {
        ui->erroDocumento->setText("Campo obrigatório");
        tudoValido = false;
    } else if (ehPF) {
        if (!std::regex_match(docLimpo, std::regex("^[0-9]{11}$"))) {
            ui->erroDocumento->setText("CPF inválido (deve ter 11 dígitos)");
            tudoValido = false;
        } else {
            ui->erroDocumento->setStyleSheet("color: green;");
            ui->erroDocumento->setText("Válido");
        }
    } else if (ehPJ) {
        if (!std::regex_match(docLimpo, std::regex("^[0-9]{14}$"))) {
            ui->erroDocumento->setText("CNPJ inválido (deve ter 14 dígitos)");
            tudoValido = false;
        } else {
            ui->erroDocumento->setStyleSheet("color: green;");
            ui->erroDocumento->setText("Válido");
        }
    } else {
        // n selecionou nd
        ui->erroDocumento->setText("Selecione o tipo (PF/PJ) primeiro");
        tudoValido = false;
    }

    // valida tel
    if (telefone.empty()) {
        ui->erroTelefone->setText("Campo obrigatório");
        tudoValido = false;
    } else if (!std::regex_match(telLimpo, std::regex("^[0-9]{10,11}$"))) { // (XX) XXXXX-XXXX ou (XX) XXXX-XXXX
        ui->erroTelefone->setText("Telefone inválido (10 ou 11 dígitos)");
        tudoValido = false;
    } else {
        ui->erroTelefone->setStyleSheet("color: green;");
        ui->erroTelefone->setText("Válido");
    }

    // valida email
    if (email.empty()) {
        ui->erroEmail->setText("Campo obrigatório");
        tudoValido = false;
    } else if (!std::regex_match(email, std::regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"))) {
        ui->erroEmail->setText("Formato de email inválido");
        tudoValido = false;
    } else {
        ui->erroEmail->setStyleSheet("color: green;");
        ui->erroEmail->setText("Válido");
    }

    // valida endereco
    if (endereco.empty()) {
        ui->erroEndereco->setText("Campo obrigatório");
        tudoValido = false;
    } else {
        ui->erroEndereco->setStyleSheet("color: green;");
        ui->erroEndereco->setText("Válido");
    }

    // checagem final
    if (tudoValido) {
        // checa se ja existe
        if(GerenciadorDeClientes::getInstance().buscarPorDocumento(docLimpo) != nullptr) {
            ui->erroDocumento->setStyleSheet("color: red;");
            ui->erroDocumento->setText("Este documento já está cadastrado");
            return;
        }

        // cria o cliente
        Clientes* novoCliente = nullptr;
        if (ehPF) {
            novoCliente = new PessoaFisica(nome, docLimpo, telLimpo, endereco);
        } else {
            novoCliente = new PessoaJuridica(nome, docLimpo, telLimpo, endereco);
        }

        GerenciadorDeClientes::getInstance().adicionar(novoCliente);

        QMessageBox::information(this, "Sucesso", "Cliente cadastrado com sucesso!");
        this->close();
    }

}

// funcao limpa a formatacao
std::string TelaCadastroCliente::limparFormatacao(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), '.'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '-'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '/'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '('), str.end());
    str.erase(std::remove(str.begin(), str.end(), ')'), str.end());
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

// reseta campos de erro
void TelaCadastroCliente::resetarCamposErro() {
    ui->erroNome->setText("");
    ui->erroDocumento->setText("");
    ui->erroTelefone->setText("");
    ui->erroEmail->setText("");
    ui->erroEndereco->setText("");

    // cor padrao dos erros
    ui->erroNome->setStyleSheet("color: red;");
    ui->erroDocumento->setStyleSheet("color: red;");
    ui->erroTelefone->setStyleSheet("color: red;");
    ui->erroEmail->setStyleSheet("color: red;");
    ui->erroEndereco->setStyleSheet("color: red;");
}




void TelaCadastroCliente::on_botCadastro_2_clicked()
{

}

