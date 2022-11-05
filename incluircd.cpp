#include "incluircd.h"
#include "ui_incluircd.h"

IncluirCD::IncluirCD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirCD)
{
    ui->setupUi(this);

    erroBox = new QMessageBox();
    erroBox->setIcon(QMessageBox::Critical);
    erroBox->setWindowTitle("CD Invalido");
}

IncluirCD::~IncluirCD()
{
    delete ui;
    delete erroBox;
}

void IncluirCD::clear(){
    ui->getNome->setText("");
    ui->getPreco->setText("");
    ui->getNFaixas->setText("");
}

void IncluirCD::on_buttonBox_accepted()
{
    nome = ui->getNome->text();
    preco = ui->getPreco->text();
    nfaixas = ui->getNFaixas->text();

    this->hide();

    if(nome.isEmpty() || preco.toFloat() < 0 || nfaixas.isEmpty())
    {
        erroBox->setText("Não foi possivel incluir o CD:\n"
                            "Nome = " + nome + "\n"
                            "Preco = " + preco +"\n"
                            "N Faixas = " + nfaixas);
        erroBox->show();

        return;
    }

    clear();

    emit signIncluirCD(nome, preco, nfaixas);

    this->close();
}

void IncluirCD::on_buttonBox_rejected()
{
    this->close();
}
