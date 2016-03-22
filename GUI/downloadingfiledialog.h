#pragma once
#include <QDialog>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class DownloadingFileDialog;
}

class DownloadingFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadingFileDialog(QWidget *parent = 0);
    ~DownloadingFileDialog();

private slots:
    void on_selectPathBut_clicked();

    void on_okButton_clicked();

private:
    Ui::DownloadingFileDialog *ui;
};
