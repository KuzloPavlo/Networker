#include "fileform.h"
#include "ui_fileform.h"

FileForm::FileForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileForm)
{
    ui->setupUi(this);
}

FileForm::~FileForm()
{
    delete ui;
}

void FileForm::on_SelectButton_clicked()
{

}

void FileForm::slotChangeFileStatus(const int& fileHash,const FileStatus& fileStatus,const int& filePercents)
{

}
