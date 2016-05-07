#include "downloadingfiledialog.h"
#include "ui_downloadingfiledialog.h"

DownloadingFileDialog::DownloadingFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadingFileDialog)
{
    ui->setupUi(this);
    ui->okButton->setEnabled(false);
    ui->createLabel->setEnabled(false);

    QPalette mainPall;
    mainPall.setColor (this->backgroundRole (), QColor(255, 255, 255, 255));
     this->setPalette (mainPall);
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
    bool isFull = !arg1.isEmpty() && !ui->description->document()->isEmpty();

    ui->okButton->setEnabled(isFull);
    ui->createLabel->setEnabled(isFull);
}

void DownloadingFileDialog::on_description_textChanged()
{
    bool isFull = !ui->sourceSelection->text().isEmpty() && !ui->description->document()->isEmpty();

    ui->okButton->setEnabled(isFull);
    ui->createLabel->setEnabled(isFull);
}
