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
    QString fileWay = QFileDialog::getOpenFileName(this,"Create Torrent", "","");
    ui->sourceSelection->setText(fileWay);
}

void DownloadingFileDialog::on_okButton_clicked()
{
   // if(!ui->sourceSelection->text().isEmpty())
}
