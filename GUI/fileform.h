#pragma once
#include <QWidget>
#include <QTableWidget>
#include "status.h"
#include "descriptionform.h"
#include "DownloadingFile.h"
#include <QFileDialog>
#include <functional>
#include <mutex>
#include <memory>
#include <condition_variable>

namespace Ui {
class FileForm;
}

class FileForm : public QWidget
{
    Q_OBJECT

public:
    explicit FileForm(
            const DownloadingFile& file,
            QTableWidget *mainTable,
            QTableWidget *downloadingTable = nullptr,
            QTableWidget *distributedTable = nullptr,
            QTableWidget *complitedTable = nullptr,
            QTableWidget *activeTable = nullptr,
            QTableWidget *inactiveTable  = nullptr,
            QWidget *parent = 0,
            bool mainWidget = true);
    ~FileForm();
    std::function<void(const FileStatus& fileStatus)> changeDownloader;
    std::function<void(const FileStatus& fileStatus,const int& filePercents)> changeFileStatus;
    std::shared_ptr<std::mutex> getMutex(){return m_mutexStatus;}
    std::shared_ptr<std::condition_variable> getEvent(){return m_eventStatus;}
    FileStatus* getDownloaderStatus(){return m_downloaderStatus;}

private slots:
    void on_SelectButton_clicked();
    void slotChangeFileStatus(const FileStatus& fileStatus,const int& filePercents);
    void on_SelectedButton_clicked();
signals:
    void signalChangeFileStatus(const FileStatus& fileStatus,const int& filePercents);
private:
    void insertMy();
    void createTwin(/*FileForm* ptwin,*/ QTableWidget *m_mainTable);
    void createTwinDist(/*FileForm* ptwin,*/ QTableWidget *m_mainTable);
    void createTwinCom(/*FileForm* ptwin,*/ QTableWidget *m_mainTable);
    void createTwinActi(/*FileForm* ptwin,*/ QTableWidget *m_mainTable);
    void createTwinIna(/*FileForm* ptwin,*/ QTableWidget *m_mainTable);

    QTableWidget *m_mainTable;
    QTableWidget *m_downloadingTable;
    QTableWidget *m_distributedTable;
    QTableWidget *m_complitedTable;
    QTableWidget *m_activeTable;
    QTableWidget *m_inactiveTable;
    DownloadingFile m_dowloadingFile;
    Status* m_status;
    DescriptionForm* m_description;
    Ui::FileForm *ui;
    std::shared_ptr<std::mutex>m_mutexStatus;
    std::shared_ptr<std::condition_variable> m_eventStatus;
    FileStatus* m_downloaderStatus;  // pause/deleting/downloading

    FileForm* m_myInDownloading;// = nullptr;
    FileForm* m_myInDistributed = nullptr;
    FileForm* m_myInComplited = nullptr;
    FileForm* m_myInActive= nullptr;
    FileForm* m_myInInactive= nullptr;

    bool m_mainWidget;
};
