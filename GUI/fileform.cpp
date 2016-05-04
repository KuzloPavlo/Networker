#include "fileform.h"
#include "ui_fileform.h"

FileForm::FileForm(
        const DownloadingFile& file,
        QTableWidget *mainTable,
        QTableWidget *downloadingTable,
        QTableWidget *distributedTable,
        QTableWidget *complitedTable,
        QTableWidget *activeTable,
        QTableWidget *inactiveTable,
        QWidget *parent,
        bool mainWidget) :
    QWidget(parent),
    m_dowloadingFile(file),
    m_mainTable(mainTable),
    m_downloadingTable(downloadingTable),
    m_distributedTable(distributedTable),
    m_complitedTable(complitedTable),
    m_activeTable(activeTable),
    m_inactiveTable(inactiveTable),
    m_status(new Status(this)),
    m_description(new DescriptionForm(file.m_fileInfo.m_fileDescription, this)),
    m_downloaderStatus(new FileStatus(FileStatus::downloading)),
    ui(new Ui::FileForm),
    m_mainWidget(mainWidget)
{
    ui->setupUi(this);

    m_primitives.m_counter = std::make_shared<int> (int (0));
    m_primitives.m_mutexCounter = std::make_shared<std::mutex>();
    m_primitives.m_goWrite = std::make_shared<Semaphore>(Semaphore(NULL,1,1,NULL));
    m_primitives.m_goRead = std::make_shared<Semaphore>(Semaphore(NULL,1,1,NULL));
    m_primitives.m_Shared =  std::make_shared<Semaphore>(Semaphore(NULL,1,1,NULL));


    //  ui->SelectedButton->setVisible(false);

    //--------------
    //ui->tableDownloads->setSpan(0,0,1,ui->tableDownloads->columnCount());
    //---------

    if(mainWidget)
    {
        createTwin(m_downloadingTable);
        createTwinActi(m_activeTable);
    }

    insertMy();

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

void FileForm::on_SelectedButton_clicked()
{

}

void FileForm::slotChangeFileStatus(const FileStatus& fileStatus,const int& filePercents)
{
    m_status->changeStatus(FileStatus::creating, filePercents);

    if(m_mainWidget)
    {
        if(m_myInDownloading)
        {
            this->m_myInDownloading->changeFileStatus(fileStatus, filePercents);
        }

        if(m_myInDistributed)
        {
            this->m_myInDistributed->changeFileStatus(fileStatus, filePercents);
        }

        if(m_myInDistributed)
        {
            this->m_myInDownloading->changeFileStatus(fileStatus, filePercents);
        }

        if(m_myInComplited)
        {
            this->m_myInComplited->changeFileStatus(fileStatus, filePercents);
        }

        if(m_myInActive)
        {
            this->m_myInActive->changeFileStatus(fileStatus, filePercents);
        }

        if(m_myInInactive)
        {
            this->m_myInInactive->changeFileStatus(fileStatus, filePercents);
        }
    }
}

void FileForm::insertMy()
{
    int myRow = m_mainTable->rowCount();

    m_mainTable->insertRow(myRow);
    m_mainTable->setCellWidget(myRow,1,this);
    m_mainTable->setCellWidget(myRow,2,m_status);
    m_mainTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_mainTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void FileForm::createTwin(/*FileForm* ptwin,*/ QTableWidget *mainTable)
{
    m_myInDownloading = new FileForm(m_dowloadingFile, mainTable,nullptr,nullptr,nullptr,nullptr,nullptr,this,false );
}

void FileForm::createTwinDist(/*FileForm* ptwin,*/ QTableWidget *mainTable)
{
    m_myInDistributed = new FileForm(m_dowloadingFile, mainTable,nullptr,nullptr,nullptr,nullptr,nullptr,this,false );
}
void FileForm::createTwinCom(/*FileForm* ptwin,*/ QTableWidget *mainTable)
{
    m_myInComplited = new FileForm(m_dowloadingFile, mainTable,nullptr,nullptr,nullptr,nullptr,nullptr,this,false );
}

void FileForm::createTwinActi(/*FileForm* ptwin,*/ QTableWidget *mainTable)
{
    m_myInActive = new FileForm(m_dowloadingFile, mainTable,nullptr,nullptr,nullptr,nullptr,nullptr,this,false );
}

void FileForm::createTwinIna(/*FileForm* ptwin,*/ QTableWidget *mainTable)
{
    m_myInInactive = new FileForm(m_dowloadingFile, mainTable,nullptr,nullptr,nullptr,nullptr,nullptr,this,false );
}
