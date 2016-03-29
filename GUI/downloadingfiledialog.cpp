#include "downloadingfiledialog.h"
#include "ui_downloadingfiledialog.h"

DownloadingFileDialog::DownloadingFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadingFileDialog)
{

    ui->setupUi(this);
    ui->okButton->setEnabled(false);
    ui->createLabel->setEnabled(false);
}

DownloadingFileDialog::~DownloadingFileDialog()
{
    delete ui;
}

void DownloadingFileDialog::on_selectPathBut_clicked()
{
    QString fileWay = QFileDialog::getOpenFileName(this,"Create Downloading File", "","");
    ui->sourceSelection->setText(fileWay);
}

void DownloadingFileDialog::on_okButton_clicked()
{
    emit signalCreateNewDownloadingFile(
                ui->sourceSelection->text(),
                ui->description->document()->toPlainText());
    close();
}

void DownloadingFileDialog::on_sourceSelection_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty() && !ui->description->document()->isEmpty())
    {
        ui->okButton->setEnabled(true);
        ui->createLabel->setEnabled(true);
    }
    else
    {
        ui->okButton->setEnabled(false);
        ui->createLabel->setEnabled(false);
    }
}

void DownloadingFileDialog::on_description_textChanged()
{
    if(!ui->sourceSelection->text().isEmpty() && !ui->description->document()->isEmpty())
    {
        ui->okButton->setEnabled(true);
        ui->createLabel->setEnabled(true);
    }
    else
    {
        ui->okButton->setEnabled(false);
        ui->createLabel->setEnabled(false);
    }
}
