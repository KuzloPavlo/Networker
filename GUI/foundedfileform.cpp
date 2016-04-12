#include "foundedfileform.h"
#include "ui_foundedfileform.h"

FoundedFileForm::FoundedFileForm(
        const QString& fileName,
        const QString& fileDescription,
        const QString& fileSize,
        QTableWidget *parentTable,
        QWidget *parent) :
    QWidget(parent), m_parentTable(parentTable),
    ui(new Ui::FoundedFileForm)
{
    ui->setupUi(this);
    ui->SelectedButton->setVisible(false);
    ui->Description->setVisible(false);
    ui->DescriptionLabel->setVisible(false);
    ui->SizeLabel->setVisible(false);
    ui->Size->setVisible(false);
    ui->DownloadButton->setVisible(false);

    ui->FoundFileName->setText(fileName);
    ui->Description->setText(fileDescription);
    ui->Size->setText(fileSize);
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
    emit signalDownloadFile(0);
}
