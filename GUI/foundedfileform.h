#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QString>
#include "DownloadingFile.h"
#include <functional>
#include <QFileDialog>
#include <QDebug>
#include <QDialog>

namespace Ui {
class FoundedFileForm;
}

class FoundedFileForm : public QWidget
{
    Q_OBJECT

public:
    explicit FoundedFileForm(
          const FileInfo& fileInfo,
            QTableWidget *parentTable,
            QWidget *parent = 0);
    ~FoundedFileForm();
    std::function<void(const FileInfo& file, const QString& QtLocation)>downloadFile;
  static  void changeSizeStyle(const int& fileSize, float* size, QString* sizeType);
private slots:
    void on_SelectButton_clicked();
    void on_SelectedButton_clicked();
    void on_DownloadButton_clicked();

signals:
    void signalChangeChartSize();
   // void signalDownloadFile(const int& fileHash);
private:
    QTableWidget* m_parentTable;
    FileInfo m_fileInfo;
    Ui::FoundedFileForm *ui;
};
