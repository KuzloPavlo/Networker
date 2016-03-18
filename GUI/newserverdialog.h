#pragma once
#include <QDialog>

namespace Ui {
class NewServerDialog;
}

class NewServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewServerDialog(QWidget *parent = 0);
    ~NewServerDialog();

signals:

private slots:
    void on_okButton_clicked();

    void on_serverAdressEdit_textChanged(const QString &arg1);

    void on_serverPortEdit_textChanged(const QString &arg1);

private:
    Ui::NewServerDialog *ui;
};
