#include "descriptionform.h"
#include "ui_descriptionform.h"

DescriptionForm::DescriptionForm(const FileInfo& description, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DescriptionForm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->Hash->setText(std::to_string( description.m_fileHash).c_str());
    ui->Parts->setText(std::to_string(description.m_numberParts).c_str());
    ui->Description->setText(description.m_fileDescription);
    this->hidDescription();
}

DescriptionForm::~DescriptionForm()
{
    delete ui;
}

void DescriptionForm::showDescription()
{
    ui->DeleteButton->setVisible(true);
    ui->Description->setVisible(true);
    ui->DescriptionLabel->setVisible(true);
    ui->DowloadButton->setVisible(true);
    ui->Hash->setVisible(true);
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    //ui->label_3->setVisible(true);
    ui->Parts->setVisible(true);
    ui->PauseButton->setVisible(true);
    //ui->Status->setVisible(true);
}

void DescriptionForm::hidDescription()
{

    ui->DeleteButton->setVisible(false);
    ui->Description->setVisible(false);
    ui->DescriptionLabel->setVisible(false);
    ui->DowloadButton->setVisible(false);
    ui->Hash->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    //ui->label_3->setVisible(false);
    ui->Parts->setVisible(false);
    ui->PauseButton->setVisible(false);
    //ui->Status->setVisible(false);
}

void DescriptionForm::on_DowloadButton_clicked()
{

    setFilseStatus(FileStatus::downloading);

    //--------------
     ui->Hash->setText("DescriptionForm::on_DowloadButton_clicked()");
    //---------
}

void DescriptionForm::on_PauseButton_clicked()
{
    setFilseStatus(FileStatus::pause);

    //--------------
    ui->Hash->setText("DescriptionForm::on_PauseButton_clicked()");
    //---------
}

void DescriptionForm::on_DeleteButton_clicked()
{
    setFilseStatus(FileStatus::deleting);

    //--------------
    ui->Hash->setText("escriptionForm::on_DeleteButton_clicked()");
    //---------
}
