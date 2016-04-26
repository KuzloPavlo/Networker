#include "fileform.h"
#include "ui_fileform.h"

FileForm::FileForm(const DownloadingFile& file,QTableWidget *parentTable ,QWidget *parent) :
    QWidget(parent),
    m_parentTable(parentTable),
    m_status(new Status(this)),
    m_description(new DescriptionForm(file.m_fileInfo.m_fileDescription, this)),
    m_mutexStatus(new std::mutex),
    m_eventStatus(new std::condition_variable),
    m_downloaderStatus(new FileStatus(FileStatus::downloading)),
    ui(new Ui::FileForm)
{
    ui->setupUi(this);

    int myRow = m_parentTable->rowCount();
    m_parentTable->insertRow(myRow);
    m_parentTable->setCellWidget(myRow,1,this);
    m_parentTable->setCellWidget(myRow,2,m_status);

    changeFileStatus = std::bind(
                &FileForm::signalChangeFileStatus,
                this,
                std::placeholders::_1,
                std::placeholders::_2);

    connect(this,
            SIGNAL(signalChangeFileStatus(FileStatus,int)),
            this,
            SLOT(slotChangeFileStatus(FileStatus,int)) );
}

FileForm::~FileForm()
{
    delete ui;
}

void FileForm::on_SelectButton_clicked()
{

}

void FileForm::slotChangeFileStatus(const FileStatus& fileStatus,const int& filePercents)
{
   ui->FileName->setText(std::to_string(fileStatus).c_str());
}
