#pragma once
#include <QWidget>
#include <QTableWidget>
#include "status.h"
#include "descriptionform.h"
#include "DownloadingFile.h"
#include <QFileDialog>

namespace Ui {
class FileForm;
}

class FileForm : public QWidget
{
    Q_OBJECT

public:
    explicit FileForm(QWidget *parent = 0);
    ~FileForm();

private slots:
    void on_SelectButton_clicked();
    void slotChangeFileStatus(const int& fileHash,const FileStatus& fileStatus,const int& filePercents);

signals:
    void signalChangeFileStatus(const int& fileHash,const FileStatus& fileStatus,const int& filePercents);

private:
    QTableWidget* m_parentTable;
    Status* m_status;
    DescriptionForm* m_description;
    Ui::FileForm *ui;
};
