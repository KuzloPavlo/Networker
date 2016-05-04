#include "newserverdialog.h"
#include "ui_newserverdialog.h"

NewServerDialog::NewServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewServerDialog)
{
    ui->setupUi(this);
   // ui->okButton->setEnabled(false);
    ui->label->setEnabled(false);

    QPalette mainPall;
    mainPall.setColor (this->backgroundRole (), QColor(255, 255, 255, 255));

    this->setPalette (mainPall);
}

NewServerDialog::~NewServerDialog()
{
    delete ui;
}

void NewServerDialog::on_serverAdressEdit_textChanged(const QString &arg1)
{
    bool isFull = !ui->serverPortEdit->text().isEmpty() && !ui->serverAdressEdit->text().isEmpty();

    ui->okButton->setEnabled(isFull);
    ui->label->setEnabled(isFull);
}

void NewServerDialog::on_serverPortEdit_textChanged(const QString &arg1)
{
    bool isFull = !ui->serverPortEdit->text().isEmpty() && !ui->serverAdressEdit->text().isEmpty();

    ui->okButton->setEnabled(isFull);
    ui->label->setEnabled(isFull);
}

void NewServerDialog::on_okButton_clicked()
{

    emit signalConnectToServer(
                ui->serverAdressEdit->text().toStdString(),
                ui->serverPortEdit->text().toStdString()
                );

    close();
}


