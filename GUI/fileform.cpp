#include "fileform.h"
#include "ui_fileform.h"

FileForm::FileForm(
        const DownloadingFile& file,
        QTableWidget *mainTable,
        QWidget *parent) :
    QWidget(parent),
    m_dowloadingFile(file),
    m_mainTable(mainTable),
    m_status(new Status(this)),
    m_description(new DescriptionForm(file.m_fileInfo, this)),
    m_downloaderStatus(new FileStatus(FileStatus::downloading)),
    ui(new Ui::FileForm)
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


    m_description->setFilseStatus = std::bind(
                &FileForm::signalSetFileStatus,
                this,
                std::placeholders::_1);

    insertMy();

    changeFileStatus = std::bind(
                &FileForm::signalChangeFileStatus,
                this,
                std::placeholders::_1,
                std::placeholders::_2);

    setNewFile = std::bind(&FileForm::signalSetFile, this, std::placeholders::_1);

    connect(this,
            SIGNAL(signalChangeFileStatus(FileStatus,int)),
            this,
            SLOT(slotChangeFileStatus(FileStatus,int)) );

    connect(this,
            SIGNAL(signalSetFile(const DownloadingFile &)),
            this,
            SLOT(slotSetFile(const DownloadingFile &)));
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
    m_dowloadingFile.m_fileStatus = fileStatus;
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

    showSize();

    //    float forSize = 0;
    //    QString sizeType;

    //    FoundedFileForm::changeSizeStyle(m_dowloadingFile.m_fileInfo.m_fileSize, &forSize,&sizeType);

    //    std::string sizeFile = std::to_string(forSize);

    //    sizeFile.erase( sizeFile.end()-4, sizeFile.end());

    //    sizeFile += " " + sizeType.toStdString();

    //    QTableWidgetItem* size = new QTableWidgetItem(tr("%1").arg(sizeFile.c_str()));

    //    m_mainTable->setItem(m_myRow,3,size);
}

void FileForm::showSize()
{
    float forSize = 0;
    QString sizeType;

    FoundedFileForm::changeSizeStyle(m_dowloadingFile.m_fileInfo.m_fileSize, &forSize,&sizeType);

    std::string sizeFile = std::to_string(forSize);

    sizeFile.erase( sizeFile.end()-4, sizeFile.end());

    sizeFile += " " + sizeType.toStdString();

    QTableWidgetItem* size = new QTableWidgetItem(tr("%1").arg(sizeFile.c_str()));

    m_mainTable->setItem(m_myRow,3,size);
}


void FileForm::slotSetFileStatus(const FileStatus& fileStatus)
{
    //------------------
    static int i = 2;
    i++;
    signalChangeFileStatus(fileStatus,i);
    //----------
    //    m_primitives.m_mutexCounter->lock();
    //    if ((*m_primitives.m_counter) == 0)
    //    {
    //        m_primitives.m_goRead->lock();
    //        m_primitives.m_Shared->lock();
    //    }
    //    (*m_primitives.m_counter)++;
    //    m_primitives.m_mutexCounter->unlock();

    //    *m_downloaderStatus = fileStatus;

    //    m_primitives.m_mutexCounter->lock();
    //    (*m_primitives.m_counter)--;
    //    if ((*m_primitives.m_counter) == 0)
    //    {
    //        m_primitives.m_Shared->unlock_one();
    //        m_primitives.m_goWrite->unlock_one();
    //    }
    //    m_primitives.m_mutexCounter->unlock();
}

void FileForm::filter(const Filter& filter)
{
    switch (filter) {
    case Filter::all:
        m_mainTable->setRowHeight(m_myRow, 30);
        on_SelectedButton_clicked();
        break;

    case Filter::download:
        if(m_dowloadingFile.m_fileStatus == FileStatus::downloading)
        {
            m_mainTable->setRowHeight(m_myRow, 30);
        }
        else
        {
            m_mainTable->setRowHeight(m_myRow, 0);
        }

        break;

    default:
        break;
    }

}

void FileForm::slotSetFile(const DownloadingFile &newFile)
{
    m_dowloadingFile = newFile;
    m_description->setFileInfo(newFile.m_fileInfo);
    showSize();
}
