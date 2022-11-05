#include "incluirdvd.h"
#include "ui_incluirdvd.h"

IncluirDVD::IncluirDVD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirDVD)
{
    ui->setupUi(this);

    erroBox = new QMessageBox();
    erroBox->setIcon(QMessageBox::Critical);
    erroBox->setWindowTitle("DVD Invalido");
}

IncluirDVD::~IncluirDVD()
{
    delete ui;
    delete erroBox;
}

void IncluirDVD::clear(){
    ui->getNome->setText("");
    ui->getPreco->setText("");
    ui->getDuracao->setText("");
}

void IncluirDVD::on_buttonBox_accepted()
{
    nome = ui->getNome->text();
    preco = ui->getPreco->text();
    duracao = ui->getDuracao->text();

    this->hide();

    if(nome.isEmpty() || preco.toFloat() < 0 || duracao.isEmpty())
    {
        erroBox->setText("Não foi possivel incluir o DVD:\n"
                            "Nome = " + nome + "\n"
                            "Preco = " + preco +"\n"
                            "Duração = " + duracao);
        erroBox->show();

        return;
    }

    clear();

    emit signIncluirDVD(nome, preco, duracao);

    this->close();
}

void IncluirDVD::on_buttonBox_rejected()
{
    this->close();
}
