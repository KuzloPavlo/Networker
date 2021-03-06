#pragma once
#include <QMainWindow>
#include <QFileDialog>
#include <QTableWidget>
#include <newserverdialog.h>
#include <downloadingfiledialog.h>
#include <status.h>
#include <QMainWindow>
#include "Client.h"
#include <string>
#include <functional>
#include <memory>
#include "FileStatus.h"
#include "fileform.h"
#include "foundedfileform.h"
#include "DownloadingFile.h"
#include "temdebuger.h"    // will delete
#include "Semaphore.h"
#include "descriptionform.h"
#include <QMessageBox>
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(DownloadingFile)
Q_DECLARE_METATYPE(FileInfo)
Q_DECLARE_METATYPE(FileStatus)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_searchBut_clicked();

    void on_addServerBut_clicked();

    void on_createFileBut_clicked();

    // void on_sendButton_clicked();

    void slotDisplay(const std::string& str);

    void slotConnectToServer(const std::string& IPaddress, const std::string& port);

    void slotCreateNewDownloadingFile(
            const QString& location,
            const QString& description);

    void slotAddNewDownloadingFile(const DownloadingFile& newFile);

    void slotShowFoundFile(const FileInfo& foundFile);

    void slotDownloadFile(const FileInfo& foundFile, const QString& QtLocation);

    void on_searchEdit_returnPressed();




    void on_homeButton_clicked();

    void on_showAllButton_clicked();

    void on_pushButton_7_clicked();

    void on_AllButton_clicked();

    void on_downloadingButton_clicked();

    void on_distributedButton_clicked();

    void on_completedButton_clicked();

    void on_activeButton_clicked();

    void on_inactiveButton_clicked();

signals:

    void signalDisplay(const std::string& str);

    void signalChangeFileStatus(
            const std::string& fileHash,
            const int& fleStatusPercents,
            FileStatus fileStatus = FileStatus::downloading);

    void signalAddNewDownloadingFile(const DownloadingFile& newFile);

    void sigalshowNewFileStatus();

    void signalShowFoundFile(const FileInfo& foundFile);

    void signalDownloadFile(const FileInfo& foundFile, const QString& QtLocation);


private:

    Ui::MainWindow *ui;
    Client *m_pClient;
    NewServerDialog *m_pNewServerDialog;
    DownloadingFileDialog *m_pNewFileDialog;
    std::string changeLocationStyle(const QString& QtStyleLocation);

    std::vector<FileForm*> m_fileForms;
    void flushDownloadingFiles();

    void tableSetting(QTableWidget* table);
    void setFileForms();

    void filter(const Filter& filter);


    //--------------------------
    TemDebuger* m_debuger;
    //---------------------
};
