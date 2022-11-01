#ifndef _LOJA_H_
#define _LOJA_H_

#include <iostream>
#include <string>
#include <vector>
#include <QMainWindow>
#include <incluirlivro.h>
#include <incluircd.h>
#include <incluirdvd.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Produto
{
private:
  std::string nome;
  int preco;
public:
  /// Construtor
  Produto(const std::string& N="", int P=0);

  /// Funcoes de consulta
  std::string getNome() const;
  double getPreco() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(std::istream& I);
  void salvar(std::ostream& O) const;
};

std::istream& operator>>(std::istream& I, Produto& P);
std::ostream& operator<<(std::ostream& O, const Produto& P);

class Livro: public Produto
{
private:
  std::string autor;
public:
  /// Construtor
  Livro(const std::string& N="", int P=0, const std::string& A="");

  /// Funcoes de consulta
  std::string getAutor() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(std::istream& I);
  void salvar(std::ostream& O) const;
};

std::istream& operator>>(std::istream& I, Livro& L);
std::ostream& operator<<(std::ostream& O, const Livro& L);

class CD: public Produto
{
private:
  int nfaixas;
public:
  /// Construtor
  CD(const std::string& N="", int P=0, int NF=0);

  /// Funcoes de consulta
  int getNumFaixas() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(std::istream& I);
  void salvar(std::ostream& O) const;
};

std::istream& operator>>(std::istream& I, CD& C);
std::ostream& operator<<(std::ostream& O, const CD& C);

class DVD: public Produto
{
private:
  int duracao;
public:
  /// Construtor
  DVD(const std::string& N="", int P=0, int D=0);

  /// Funcoes de consulta
  int getDuracao() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(std::istream& I);
  void salvar(std::ostream& O) const;
};

std::istream& operator>>(std::istream& I, DVD& D);
std::ostream& operator<<(std::ostream& O, const DVD& D);

class Loja
{
private:
  std::vector<Livro> LL;
  std::vector<CD>    LC;
  std::vector<DVD>   LD;
public:
  /// Construtor
  Loja();

  /// Funcoes de consulta
  int getNumLivro() const;
  int getNumCD() const;
  int getNumDVD() const;
  Livro getLivro(int id) const;
  CD getCD(int id) const;
  DVD getDVD(int id) const;

  /// Funcoes de manipulacao (inclusao/exclusao) de itens do estoque
  void incluirLivro(const Livro& X);
  bool excluirLivro(int id);
  void incluirCD(const CD& X);
  bool excluirCD(int id);
  void incluirDVD(const DVD& X);
  bool excluirDVD(int id);

  /// Funcoes de entrada/saida
  bool ler(const std::string& arq);
  bool salvar(const std::string& arq) const;
  void imprimir() const;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Loja x;
    Ui::IncluirCD *inclCD;
    Ui::IncluirDVD *inclDVD;
    Ui::IncluirLivro *inclLivro;
};
#endif // _LOJA_H_
