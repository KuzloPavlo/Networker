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
#include "Semaphore.h"
#include "Synchronization.h"
#include "foundedfileform.h"
#include "Filter.h"
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
            QWidget *parent = 0
            );
    ~FileForm();
    std::function<void(const FileStatus& fileStatus)> changeDownloader;
    std::function<void(const FileStatus& fileStatus,const int& filePercents)> changeFileStatus;
    std::function<void (const DownloadingFile& newFile)> setNewFile;
    Synchronization getPrimitives(){return m_primitives;}
    FileStatus* getDownloaderStatus(){return m_downloaderStatus;}
    void filter(const Filter& filter);
private slots:
    void on_SelectButton_clicked();
    void slotChangeFileStatus(const FileStatus& fileStatus,const int& filePercents);
    void slotSetFileStatus(const FileStatus& fileStatus);
    void on_SelectedButton_clicked();
    void slotSetFile(const DownloadingFile& newFile);
signals:
    void signalChangeFileStatus(const FileStatus& fileStatus,const int& filePercents);
    void signalSetFileStatus(const FileStatus& fileStatus);
    void signalSetFile(const DownloadingFile& newFile);
private:
    void insertMy();

    QTableWidget *m_mainTable;

    DownloadingFile m_dowloadingFile;
    Status* m_status;
    DescriptionForm* m_description;
    Ui::FileForm *ui;

    Synchronization m_primitives;
    FileStatus* m_downloaderStatus;  // pause/deleting/downloading

    int m_myRow;
};
