#include "foundedfileform.h"
#include "ui_foundedfileform.h"

FoundedFileForm::FoundedFileForm(QTableWidget *table , QWidget *parent) :
    QWidget(parent), m_parent(table),
    ui(new Ui::FoundedFileForm)
{
    ui->setupUi(this);
    ui->SelectedButton->setVisible(false);
    ui->textBrowser->setVisible(false);
    ui->DescriptionLabel->setVisible(false);
    ui->SizeLabel->setVisible(false);
    ui->Size->setVisible(false);
    ui->DownloadButton->setVisible(false);
}

FoundedFileForm::~FoundedFileForm()
{
    delete ui;
}

void FoundedFileForm::on_SelectButton_clicked()
{
    ui->SelectButton->setVisible(false);
    ui->SelectedButton->setVisible(true);
    ui->textBrowser->setVisible(true);
    ui->DescriptionLabel->setVisible(true);
    ui->SizeLabel->setVisible(true);
    ui->Size->setVisible(true);
    ui->DownloadButton->setVisible(true);
    m_parent->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void FoundedFileForm::on_SelectedButton_clicked()
{
    ui->SelectButton->setVisible(true);
    ui->SelectedButton->setVisible(false);
    ui->textBrowser->setVisible(false);
    ui->DescriptionLabel->setVisible(false);
    ui->SizeLabel->setVisible(false);
    ui->Size->setVisible(false);
    ui->DownloadButton->setVisible(false);
    m_parent->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
