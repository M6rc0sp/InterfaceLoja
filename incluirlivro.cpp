#include "incluirlivro.h"
#include "ui_incluirlivro.h"

IncluirLivro::IncluirLivro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirLivro)
{
    ui->setupUi(this);

    erroBox = new QMessageBox();
    erroBox->setIcon(QMessageBox::Critical);
    erroBox->setWindowTitle("Livro Invalido");
}

IncluirLivro::~IncluirLivro()
{
    delete ui;
    delete erroBox;
}

void IncluirLivro::clear(){
    ui->getNome->setText("");
    ui->getPreco->setText("");
    ui->getAutor->setText("");
}

void IncluirLivro::on_buttonBox_accepted()
{
    nome = ui->getNome->text();
    preco = ui->getPreco->text();
    autor = ui->getAutor->text();

    this->hide();

    if(nome.isEmpty() || preco.toFloat() < 0 || autor.isEmpty())
    {
        erroBox->setText("Não foi possivel incluir o Livro:\n"
                            "Nome = " + nome + "\n"
                            "Preco = " + preco +"\n"
                            "Autor = " + autor);
        erroBox->show();

        return;
    }

    clear();

    emit signIncluirLivro(nome, preco, autor);

    this->close();
}

void IncluirLivro::on_buttonBox_rejected()
{
    this->close();
}

