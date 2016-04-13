#include "foundedfileform.h"
#include "ui_foundedfileform.h"

FoundedFileForm::FoundedFileForm(
        const FileInfo& fileInfo,
        QTableWidget *parentTable,
        QWidget *parent) :
    QWidget(parent), m_parentTable(parentTable), m_fileInfo(fileInfo),
    ui(new Ui::FoundedFileForm)
{
    ui->setupUi(this);
    ui->SelectedButton->setVisible(false);
    ui->Description->setVisible(false);
    ui->DescriptionLabel->setVisible(false);
    ui->SizeLabel->setVisible(false);
    ui->Size->setVisible(false);
    ui->DownloadButton->setVisible(false);

    ui->FoundFileName->setText(fileInfo.m_fileName);
    ui->Description->setText(fileInfo.m_fileDescription);
    std::string sizeFile = std::to_string(fileInfo.m_fileSize);
    ui->Size->setText(sizeFile.c_str());
}

FoundedFileForm::~FoundedFileForm()
{
    delete ui;
}

void FoundedFileForm::on_SelectButton_clicked()
{
    ui->SelectButton->setVisible(false);
    ui->SelectedButton->setVisible(true);
    ui->Description->setVisible(true);
    ui->DescriptionLabel->setVisible(true);
    ui->SizeLabel->setVisible(true);
    ui->Size->setVisible(true);
    ui->DownloadButton->setVisible(true);
    m_parentTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void FoundedFileForm::on_SelectedButton_clicked()
{
    ui->SelectButton->setVisible(true);
    ui->SelectedButton->setVisible(false);
    ui->Description->setVisible(false);
    ui->DescriptionLabel->setVisible(false);
    ui->SizeLabel->setVisible(false);
    ui->Size->setVisible(false);
    ui->DownloadButton->setVisible(false);
    m_parentTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void FoundedFileForm::on_DownloadButton_clicked()
{
    // в конструктор необходимо добавить еще хеш, и в сигнал его пихать
    QString fileWay = QFileDialog::getExistingDirectory(this,"Downloading","");
   // DownloadingFile
  //  ui->sourceSelection->setText(fileWay);
    emit signalDownloadFile(0);
}
