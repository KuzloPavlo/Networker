#include "downloadingfiledialog.h"
#include "ui_downloadingfiledialog.h"

DownloadingFileDialog::DownloadingFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadingFileDialog)
{
    ui->setupUi(this);
}

DownloadingFileDialog::~DownloadingFileDialog()
{
    delete ui;
}

void DownloadingFileDialog::on_selectPathBut_clicked()
{
    QString fileWay = QFileDialog::getOpenFileName(this,"Create Torrent", "","");
    qDebug() << fileWay;
}
