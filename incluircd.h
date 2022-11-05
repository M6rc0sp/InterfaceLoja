#ifndef INCLUIRCD_H
#define INCLUIRCD_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class IncluirCD;
}

class IncluirCD : public QDialog
{
    Q_OBJECT

public:
    explicit IncluirCD(QWidget *parent = nullptr);
    ~IncluirCD();
    void clear();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void signIncluirCD(QString nome, QString preco, QString nfaixas);

private:
    Ui::IncluirCD *ui;

    QMessageBox  *erroBox;

    QString nome;
    QString preco;
    QString nfaixas;
};

#endif // INCLUIRCD_H
