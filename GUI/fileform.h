#pragma once
#include <QWidget>
#include <QTableWidget>
#include "status.h"
#include "descriptionform.h"
#include "DownloadingFile.h"
#include <QFileDialog>
#include <functional>

namespace Ui {
class FileForm;
}

class FileForm : public QWidget
{
    Q_OBJECT

public:
    explicit FileForm(const DownloadingFile& file, QTableWidget *parentTable ,QWidget *parent = 0);
    ~FileForm();
    std::function<void(const FileStatus& fileStatus)> changeDownloader;
    std::function<void(const FileStatus& fileStatus,const int& filePercents)> changeFileStatus;
private slots:
    void on_SelectButton_clicked();
    void slotChangeFileStatus(const FileStatus& fileStatus,const int& filePercents);

signals:
    void signalChangeFileStatus(const FileStatus& fileStatus,const int& filePercents);

private:
    QTableWidget* m_parentTable;
    Status* m_status;
    DescriptionForm* m_description;
    Ui::FileForm *ui;
};
