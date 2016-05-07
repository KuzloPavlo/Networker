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
    m_description(new DescriptionForm(file.m_fileInfo, this)),
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

    connect(this,
            SIGNAL (signalSetFileStatus(const FileStatus&)),
            this,
            SLOT
            (slotSetFileStatus(const FileStatus&)));

    ui->SelectedButton->setVisible(false);

    if(mainWidget)
    {
        m_description->setFilseStatus = std::bind(
                    &FileForm::signalSetFileStatus,
                    this,
                    std::placeholders::_1);

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
    ui->SelectButton->setVisible(false);
    ui->SelectedButton->setVisible(true);

    m_description->showDescription();
    m_mainTable->setRowHeight(m_myRow+1, 155);
}

void FileForm::on_SelectedButton_clicked()
{
    ui->SelectButton->setVisible(true);
    ui->SelectedButton->setVisible(false);

    m_description->hidDescription();
    m_mainTable->setRowHeight(m_myRow+1, 0);
}

void FileForm::slotChangeFileStatus(const FileStatus& fileStatus,const int& filePercents)
{
    m_status->changeStatus(fileStatus, filePercents);

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
    m_myRow = m_mainTable->rowCount();

    m_mainTable->insertRow(m_myRow);
    m_mainTable->setCellWidget(m_myRow,1,this);
    m_mainTable->setCellWidget(m_myRow,2,m_status);

    m_mainTable->insertRow(m_myRow+1);
    m_mainTable->setRowHeight(m_myRow+1,0);
    m_mainTable->setSpan(m_myRow+1,1,1,2);
    m_mainTable->setCellWidget(m_myRow+1,1,m_description);

    ui->FileName->setText(m_dowloadingFile.m_fileInfo.m_fileName);

    QTableWidgetItem* number = new QTableWidgetItem(tr("%1").arg((m_myRow/2)+1));
    number->setTextAlignment(Qt::AlignCenter);


    m_mainTable->setItem(m_myRow,0, number);

    float forSize = 0;
    QString sizeType;

    FoundedFileForm::changeSizeStyle(m_dowloadingFile.m_fileInfo.m_fileSize, &forSize,&sizeType);

    std::string sizeFile = std::to_string(forSize);

    sizeFile.erase( sizeFile.end()-4, sizeFile.end());

    sizeFile += " " + sizeType.toStdString();

    QTableWidgetItem* size = new QTableWidgetItem(tr("%1").arg(sizeFile.c_str()));

    m_mainTable->setItem(m_myRow,3,size);
}

void FileForm::createTwin(/*FileForm* ptwin,*/ QTableWidget *mainTable)
{
    m_myInDownloading = new FileForm(m_dowloadingFile, mainTable,nullptr,nullptr,nullptr,nullptr,nullptr,this,false );
    m_myInDownloading->m_description->setFilseStatus = std::bind(
                &FileForm::signalSetFileStatus,
                this,
                std::placeholders::_1);
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

void FileForm::slotSetFileStatus(const FileStatus& fileStatus)
{
    //------------------
    static int i = 2;
    i++;
    signalChangeFileStatus(fileStatus,i);
    //----------
    m_primitives.m_mutexCounter->lock();
    if ((*m_primitives.m_counter) == 0)
    {
        m_primitives.m_goRead->lock();
        m_primitives.m_Shared->lock();
    }
    (*m_primitives.m_counter)++;
    m_primitives.m_mutexCounter->unlock();

    *m_downloaderStatus = fileStatus;

    m_primitives.m_mutexCounter->lock();
    (*m_primitives.m_counter)--;
    if ((*m_primitives.m_counter) == 0)
    {
        m_primitives.m_Shared->unlock_one();
        m_primitives.m_goWrite->unlock_one();
    }
    m_primitives.m_mutexCounter->unlock();
}
