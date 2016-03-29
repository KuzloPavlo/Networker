#pragma once
#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <string>

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

    void on_sourceSelection_textChanged(const QString &arg1);

    void on_description_textChanged();

signals:

    void signalCreateNewDownloadingFile(
            const QString& location,
            const QString& description);

private:
    Ui::DownloadingFileDialog *ui;
};
