#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include <QStatusBar>
#include <QFileDialog>
#include "loja.h"
#include "ui_mainwindow.h"

using namespace std;

#include "loja.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , x()
    , inclCD(nullptr)
    , inclDVD(nullptr)
    , inclLivro(nullptr)
    , total_itens(nullptr)
{
    //instanciando os objetos da classe
    ui->setupUi(this);
    inclCD = new IncluirCD(this);
    inclDVD = new IncluirDVD(this);
    inclLivro = new IncluirLivro(this);
    total_itens = new QLabel;

    //configurando statusBar
    statusBar()->insertWidget(0, new QLabel("Total de itens: "));
    total_itens->setText(&""[(x.getNumLivro()+x.getNumCD()+x.getNumDVD())]);
    statusBar()->insertWidget(1, total_itens);

    //Configurando Interface das Tabelas
    //Livro
    ui->labelLivros->setStyleSheet("lightgray");
    ui->tableLivros->setStyleSheet("QHeaderView::section{background-color:lightgray}");
    ui->tableLivros->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableLivros->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableLivros->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);

    //CD
    ui->labelCDs->setStyleSheet("lightgray");
    ui->tableCDs->setStyleSheet("QHeaderView::section { background-color:lightgray }");
    ui->tableCDs->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableCDs->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableCDs->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);

    //DVD
    ui->labelDVDs->setStyleSheet("lightgray");
    ui->tableDVDs->setStyleSheet("QHeaderView::section { background-color:lightgray }");
    ui->tableDVDs->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableDVDs->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableDVDs->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);

    //Conectando sinais e slots de add produtos
    connect(inclLivro, SIGNAL(signIncluirLivro(QString,QString,QString)), this, SLOT(slotIncluirLivro(QString,QString,QString)) );
    connect(inclCD, SIGNAL(signIncluirCD(QString,QString,QString)), this, SLOT(slotIncluirCD(QString,QString,QString)) );
    connect(inclDVD, SIGNAL(signIncluirDVD(QString,QString,QString)), this, SLOT(slotIncluirDVD(QString,QString,QString)) );

    //Caixa para confirmar quando for excluir um item do estoque
    confirma = new QMessageBox();
    confirma->setWindowTitle("Excluir item");
    confirma->setIcon(QMessageBox::Warning);
    confirma->addButton(QMessageBox::Ok);
    confirma->addButton(QMessageBox::Cancel);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete inclLivro;
    delete inclCD;
    delete inclDVD;

    delete total_itens;

    delete confirma;
}

/// Construtor
Produto::Produto(const string& N, int P): nome(N), preco(P) {}

/// Funcoes de consulta
string Produto::getNome() const
{
  return nome;
}

double Produto::getPreco() const
{
  // ATENCAO: return preco/100; dah errado, pois faz a divisao inteira e soh
  // depois converte para double ao retornar, o que faz perder a parte fracionaria
  return preco/100.0;
}

/// Funcoes de entrada/saida
void Produto::digitar()
{
  // Leh o nome do produto
  do
  {
    cout << "Nome: ";
    getline(cin,nome);
  }
  while (nome=="");
  // Leh o preco do produto
  double Pr;
  do
  {
    cout << "Preco (##.##): ";
    cin >> Pr;
  }
  while (Pr <= 0.0);
  cin.ignore();
  preco = (int)round(100.0*Pr);
}

/// Define a funcao imprimir como sendo salvar em cout
void Produto::imprimir() const
{
  salvar(cout);
}

bool Produto::ler(istream& I)
{
  // Formato da linha no arquivo-> "STRING_NOME";$FLOAT_PRECO
  try
  {
    // Ignora ateh aspa inicial
    I.ignore(numeric_limits<streamsize>::max(),'"');
    // Leh ateh a aspa final do nome do produto
    getline(I,nome,'"');
    if (!I.good() || nome=="") throw 1;
    // Ignora o ";" e o "$"
    I.ignore(numeric_limits<streamsize>::max(),'$');
    // Leh o preco como numero real
    double Pr;
    I >> Pr;
    if (!I.good() || Pr<=0.0) throw 2;
    // Converte o preco para centavos (inteiro)
    preco = (int)round(100.0*Pr);
  }
  catch (int i)
  {
    // Erro na leitura
    nome = "";
    preco = 0;
    return false;
  }
  return true;
}

void Produto::salvar(ostream& O) const
{
  // Formato da linha-> "STRING_NOME";$FLOAT_PRECO;
  O << '"' << nome << '"' << ";$" << fixed << setprecision(2) << double(preco)/100.0;
}

istream& operator>>(istream& I, Produto& P)
{
  P.digitar();
  return I;
}

ostream& operator<<(ostream& O, const Produto& P)
{
  P.salvar(O);
  return O;
}

/// CLASSE LIVRO

Livro::Livro(const string& N, int P, const string& A): Produto(N,P), autor(A) {}

/// Funcoes de consulta
string Livro::getAutor() const
{
  return autor;
}

/// Funcoes de entrada/saida
void Livro::digitar()
{
  Produto::digitar();
  do
  {
    cout << "Autor: ";
    getline(cin,autor);
  }
  while (autor=="");
}

void Livro::imprimir() const
{
  return salvar(cout);
}

bool Livro::ler(istream& I)
{
  // Formato da linha no arquivo->  L: <Conteudo do Produto>;"STRING_AUTOR"
  try
  {
    // Leh o caractere inicial
    char opcao;
    I >> opcao;
    if (!I.good() || opcao!='L') throw 1;

    // Ignora o ":" apos a letra inicial
    I.ignore(numeric_limits<streamsize>::max(),':');
    // Comeca da aspa da string do nome do Produto
    if (!Produto::ler(I)) throw 2;

    // Ignora o ";" e a aspa inicial do nome do autor
    I.ignore(numeric_limits<streamsize>::max(),'"');
    // Leh o nome do autor
    getline(I,autor,'"');
    if (!I.good() || autor == "") throw 3;

    // Ignora o "\n" no fim da linha
    I.ignore();
  }
  catch (int i)
  {
    *this = Livro();
    return false;
  }
  return true;
}

void Livro::salvar(ostream& O) const
{
  // Formato da linha-> L: <Conteudo do Produto>;"STRING_AUTOR"
  O << "L: ";
  Produto::salvar(O);
  O << ';' << '"' << autor << '"';
}

istream& operator>>(istream& I, Livro& L)
{
  L.digitar();
  return I;
}

ostream& operator<<(ostream& O, const Livro& L)
{
  L.salvar(O);
  return O;
}

/// CLASSE CD

CD::CD(const string& N, int P, int NF): Produto(N,P), nfaixas(NF) {}

/// Funcoes de consulta
int CD::getNumFaixas() const
{
  return nfaixas;
}

/// Funcoes de entrada/saida
void CD::digitar()
{
  Produto::digitar();
  do
  {
    cout << "Numero de faixas: ";
    cin >> nfaixas;
  }
  while (nfaixas <= 0);
  cin.ignore();
}

void CD::imprimir() const
{
  salvar(cout);
}

bool CD::ler(istream& I)
{
  // Formato da linha no arquivo->  C: <Conteudo do Produto>;UNSIGNED_NFAIXAS
  try
  {
    // Leh o caractere inicial
    char opcao;
    I >> opcao;
    if (!I.good() || opcao!='C') throw 1;

    // Ignora o ":" apos a letra inicial
    I.ignore(numeric_limits<streamsize>::max(),':');
    // Comeca da aspa da string do nome do Produto
    if (!Produto::ler(I)) throw 2;

    // Ignora o ";"
    I.ignore(numeric_limits<streamsize>::max(),';');
    // Leh o numero de faixa
    I >> nfaixas;
    if (!I.good() || nfaixas <= 0) throw 3;

    // Ignora o "\n" no fim da linha
    I.ignore();
  }
  catch (int i)
  {
    *this = CD();
    return false;
  }
  return true;
}

void CD::salvar(ostream& O) const
{
  // Formato da linha-> C: <Conteudo do Produto>;UNSIGNED_NFAIXAS
  O << "C: ";
  Produto::salvar(O);
  O << ';' << nfaixas;
}

istream& operator>>(istream& I, CD& C)
{
  C.digitar();
  return I;
}

ostream& operator<<(ostream& O, const CD& C)
{
  C.salvar(O);
  return O;
}

/// CLASSE DVD

DVD::DVD(const string& N, int P, int D): Produto(N,P), duracao(D) {}

/// Funcoes de consulta
int DVD::getDuracao() const
{
  return duracao;
}

/// Funcoes de entrada/saida
void DVD::digitar()
{
  Produto::digitar();
  do
  {
    cout << "Duracao: ";
    cin >> duracao;
  }
  while (duracao <= 0);
  cin.ignore();
}

void DVD::imprimir() const
{
  salvar(cout);
}

bool DVD::ler(istream& I)
{
  // Formato da linha no arquivo-> D: <Conteudo do Produto>;UNSIGNED_DURACAO
  try
  {
    // Leh o caractere inicial
    char opcao;
    I >> opcao;
    if (!I.good() || opcao!='D') throw 1;

    // Ignora o ":" apos a letra inicial
    I.ignore(numeric_limits<streamsize>::max(),':');
    // Comeca da aspa da string do nome do Produto
    if (!Produto::ler(I)) throw 2;

    // Ignora o ";"
    I.ignore(numeric_limits<streamsize>::max(),';');
    // Leh a duracao
    I >> duracao;
    if (!I.good() || duracao <= 0) throw 3;

    // Ignora o "\n" no fim da linha
    I.ignore();
  }
  catch (int i)
  {
    *this = DVD();
    return false;
  }
  return true;
}

void DVD::salvar(ostream& O) const
{
  // Formato da linha-> D: <Conteudo do Produto>;UNSIGNED_DURACAO
  O << "D: ";
  Produto::salvar(O);
  O << ';' << duracao;
}

istream& operator>>(istream& I, DVD& D)
{
  D.digitar();
  return I;
}

ostream& operator<<(ostream& O, const DVD& D)
{
  D.salvar(O);
  return O;
}

/// CLASSE LOJA

/// Metodos basicos
Loja::Loja(): LL(), LC(), LD() {}

/// Funcoes de consulta
int Loja::getNumLivro() const
{
  return LL.size();
}

int Loja::getNumCD() const
{
  return LC.size();
}

int Loja::getNumDVD() const
{
  return LD.size();
}

Livro Loja::getLivro(int id) const
{
  if (LL.empty() || id<0 || id>=(int)LL.size()) return Livro();
  return LL[id];
}

CD Loja::getCD(int id) const
{
  if (LC.empty() || id<0 || id>=(int)LC.size()) return CD();
  return LC[id];
}

DVD Loja::getDVD(int id) const
{
  if (LD.empty() || id<0 || id>=(int)LD.size()) return DVD();
  return LD[id];
}

/// Funcoes de manipulacao (inclusao/exclusao) de itens do estoque

void Loja::incluirLivro(const Livro& X)
{
  LL.push_back(X);
}

bool Loja::excluirLivro(int id)
{
  if (LL.empty() || id<0 || id>=(int)LL.size()) return false;
  LL.erase(LL.begin()+id);
  return true;
}

void Loja::incluirCD(const CD& X)
{
  LC.push_back(X);
}

bool Loja::excluirCD(int id)
{
  if (LC.empty() || id<0 || id>=(int)LC.size()) return false;
  LC.erase(LC.begin()+id);
  return true;
}

void Loja::incluirDVD(const DVD& X)
{
  LD.push_back(X);
}

bool Loja::excluirDVD(int id)
{
  if (LD.empty() || id<0 || id>=(int)LD.size()) return false;
  LD.erase(LD.begin()+id);
  return true;
}

/// Funcoes de entrada/saida

void Loja::imprimir() const
{
  cout << ">> LIVROS:" << endl;
  for (int i=0; i<(int)LL.size(); i++) cout << i << ") " << LL[i] << endl;

  cout << ">> CDS:" << endl;
  for (int i=0; i<(int)LC.size(); i++) cout << i << ") " << LC[i] << endl;

  cout << ">> DVDS:" << endl;
  for (int i=0; i<(int)LD.size(); i++) cout << i << ") " << LD[i] << endl;
}

bool Loja::ler(const string& arq)
{
  // A stream de leitura
  ifstream I(arq.c_str());

  // O resultado logico da leitura
  bool resultado=true;

  try
  {
    if (!I.is_open()) throw 1;

    string pS;
    int NN;
    Livro L;
    CD C;
    DVD D;

    // Leh os livros
    I >> pS >> NN;
    if (!I.good() || pS!="LISTALIVRO") throw 2;
    I >> ws;
    LL.clear();
    for (int i=0; i<NN; i++)
    {
      if (!L.ler(I)) throw 3;
      LL.push_back(L);
    }

    // Leh os CDs
    I >> pS >> NN;
    if (!I.good() || pS!="LISTACD") throw 4;
    I.ignore();
    LC.clear();
    for (int i=0; i<NN; i++)
    {
      if (!C.ler(I)) throw 5;
      LC.push_back(C);
    }

    // Leh os DVDs
    I >> pS >> NN;
    if (!I.good() || pS!="LISTADVD") throw 6;
    I.ignore();
    LD.clear();
    for (int i=0; i<NN; i++)
    {
      if (!D.ler(I)) throw 7;
      LD.push_back(D);
    }
  }
  catch (int i)
  {
    LL.clear();
    LC.clear();
    LD.clear();
    resultado = false;
  }
  if (I.is_open()) I.close();
  return resultado;
}

bool Loja::salvar(const string& arq) const
{
  ofstream O(arq.c_str());
  if (!O.is_open()) return false;

  O << "LISTALIVRO " << LL.size() << endl;
  for (int i=0; i<(int)LL.size(); i++) O << LL[i] << endl;

  O << "LISTACD " << LC.size() << endl;
  for (int i=0; i<(int)LC.size(); i++) O << LC[i] << endl;

  O << "LISTADVD " << LD.size() << endl;
  for (int i=0; i<(int)LD.size(); i++) O << LD[i] << endl;

  O.close();
  return true;
}

void MainWindow::on_tableLivros_cellDoubleClicked(int row, int column)
{
    confirma->setText("Realmente deseja excluir o Livro "+ QString::fromStdString(x.getLivro(row).getNome()) +" ?");
    int opcao = confirma->exec();

    switch (opcao) {
    case QMessageBox::Ok:
    {
        if(row < 0 || unsigned(row) > unsigned(x.getNumLivro())) return;

        x.excluirLivro(row);
        atualizaLoja();
    }
        break;

    case QMessageBox::Cancel:
        confirma->close();
        break;
    }
}


void MainWindow::on_tableCDs_cellDoubleClicked(int row, int column)
{
    confirma->setText("Realmente deseja excluir o CD "+ QString::fromStdString(x.getCD(row).getNome()) +" ?");
    int opcao = confirma->exec();

    switch (opcao) {
    case QMessageBox::Ok:
    {
        if(row < 0 || unsigned(row) > unsigned(x.getNumCD())) return;

        x.excluirCD(row);
        atualizaLoja();
    }
        break;

    case QMessageBox::Cancel:
        confirma->close();
        break;
    }
}


void MainWindow::on_tableDVDs_cellDoubleClicked(int row, int column)
{
        confirma->setText("Realmente deseja excluir o DVD "+ QString::fromStdString(x.getDVD(row).getNome()) +" ?");
        int opcao = confirma->exec();

        switch (opcao) {
        case QMessageBox::Ok:
        {
            if(row < 0 || unsigned(row) > unsigned(x.getNumDVD())) return;

            x.excluirDVD(row);
            atualizaLoja();
        }
            break;

        case QMessageBox::Cancel:
            confirma->close();
            break;
        }
}

void MainWindow::on_actionLer_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Ler estoque", QString(), "Text Files (*.txt)");

    if (!x.ler(fileName.toStdString())){
        QMessageBox::critical(this, "ERRO", "Não foi possível ler o arquivo.");
    }
    atualizaLoja();
}

void MainWindow::on_actionSalvar_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Salvar estoque", QString(), "Text Files (*.txt)");

    if(!x.salvar(fileName.toStdString())){
        QMessageBox::critical(this, "ERRO", "Não foi possível salvar o arquivo.");
    }
}

void MainWindow::on_actionSair_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionIncluir_Livro_triggered()
{
    inclLivro->clear();
    inclLivro->show();
}

void MainWindow::on_actionIncluir_CD_triggered()
{
    inclCD->clear();
    inclCD->show();
}

void MainWindow::on_actionIncluir_DVD_triggered()
{
    inclDVD->clear();
    inclDVD->show();
}

void MainWindow::slotIncluirLivro(QString nome, QString preco, QString autor)
{
    x.incluirLivro(Livro(nome.toStdString(), round(preco.toFloat() *100.0), autor.toStdString()));
    atualizaLoja();
}
void MainWindow::slotIncluirCD(QString nome, QString preco, QString numfaixas)
{
    x.incluirCD(CD(nome.toStdString(), (int)round(preco.toFloat() *100.0), (int)numfaixas.toFloat()));
    atualizaLoja();
}
void MainWindow::slotIncluirDVD(QString nome, QString preco, QString duracao)
{
    x.incluirDVD(DVD(nome.toStdString(), (int)round(preco.toFloat() *100.0), (int)duracao.toFloat()));
    atualizaLoja();
}

void MainWindow::atualizaLoja()
{
    atualizaLivros();
    atualizaCDs();
    atualizaDVDs();

    QString total_i = QString::number(x.getNumCD() + x.getNumDVD() + x.getNumLivro());
    total_itens->setText(total_i);
}

void MainWindow::atualizaLivros()
{
    ui->tableLivros->clearContents();

    ui->tableLivros->setRowCount(x.getNumLivro());

    for(int i=0; i<ui->tableLivros->rowCount(); ++i)
    {

        for(int j=0; j<=2; ++j)
        {
            prov = new QLabel();

            Livro L = x.getLivro(i);

            if(j==0)
            {
                info = QString::fromStdString(L.getNome());
                prov->setText(info);
                prov->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
                ui->tableLivros->setCellWidget(i, j, prov);
            }

            else if(j==1)
            {
                info = QString::number(L.getPreco(), 'f', 2);
                prov->setText(info);
                prov->setAlignment(Qt::AlignRight | Qt::AlignCenter);
                ui->tableLivros->setCellWidget(i, j, prov);
            }

            else
            {
                info = QString::fromStdString(L.getAutor());
                prov->setText(info);
                prov->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
                ui->tableLivros->setCellWidget(i, j, prov);
            }
        }
    }
}

void MainWindow::atualizaCDs()
{
    ui->tableCDs->clearContents();

    ui->tableCDs->setRowCount(x.getNumCD());

    for(int i=0; i<ui->tableCDs->rowCount(); ++i)
    {

        for(int j=0; j<=2; ++j)
        {
            prov = new QLabel();
            CD cd = x.getCD(i);

            if(j==0)
            {
                info = QString::fromStdString(cd.getNome());
                prov->setText(info);
                prov->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
                ui->tableCDs->setCellWidget(i, j, prov);
            }

            else if(j==1)
            {
                info = QString::number(cd.getPreco(), 'f', 2);
                prov->setText(info);
                prov->setAlignment(Qt::AlignRight | Qt::AlignCenter);
                ui->tableCDs->setCellWidget(i, j, prov);
            }

            else
            {
                info = QString::number(cd.getNumFaixas());
                prov->setText(info);
                prov->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
                ui->tableCDs->setCellWidget(i, j, prov);
            }
        }
    }
}

void MainWindow::atualizaDVDs()
{
    ui->tableDVDs->clearContents();

    ui->tableDVDs->setRowCount(x.getNumDVD());

    for(int i=0; i<ui->tableDVDs->rowCount(); ++i)
    {

        for(int j=0; j<=2; ++j)
        {
            prov = new QLabel();
            DVD dvd = x.getDVD(i);

            if(j==0)
            {
                info = QString::fromStdString(dvd.getNome());
                prov->setText(info);
                prov->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
                ui->tableDVDs->setCellWidget(i, j, prov);
            }

            else if(j==1)
            {
                info = QString::number(dvd.getPreco(), 'f', 2);
                prov->setText(info);
                prov->setAlignment(Qt::AlignRight | Qt::AlignCenter);
                ui->tableDVDs->setCellWidget(i, j, prov);
            }

            else
            {
                info = QString::number(dvd.getDuracao());
                prov->setText(info);
                prov->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
                ui->tableDVDs->setCellWidget(i, j, prov);
            }
        }
    }
}
