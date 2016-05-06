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
    ui->SizeType->setVisible(false);
    ui->DownloadButton->setVisible(false);

    ui->FoundFileName->setText(fileInfo.m_fileName);
    ui->Description->setText(fileInfo.m_fileDescription);

    float size = 0;
    QString sizeType;

    changeSizeStyle(fileInfo.m_fileSize,&size,&sizeType);

    std::string sizeFile = std::to_string(size);

    sizeFile.erase( sizeFile.end()-4, sizeFile.end());

    ui->Size->setText(sizeFile.c_str());
    ui->SizeType->setText(sizeType);
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
    ui->SizeType->setVisible(true);
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
    ui->SizeType->setVisible(false);
    ui->DownloadButton->setVisible(false);
    m_parentTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void FoundedFileForm::on_DownloadButton_clicked()
{
    QString fileWay = QFileDialog::getExistingDirectory(this,"Downloading","");
    downloadFile(m_fileInfo, fileWay);
}

void FoundedFileForm::changeSizeStyle(const int& fileSize, float* size, QString* sizeType)
{
    if(fileSize >= 1000)
    {
        *size = ((float)fileSize)/1000;
        *sizeType = "Kb";
    }

    if(fileSize >= 1000000)
    {
        *size = ((float)fileSize)/1000000;
        *sizeType ="Mb";
    }

    if(fileSize >= 1000000000)
    {
        *size = ((float)fileSize)/1000000000;
        *sizeType = "Gb";
    }
}
