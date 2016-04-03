#pragma once
#include <QWidget>
#include <QTableWidget>
//#include <DownloadingFile.h>

namespace Ui {
class FoundedFileForm;
}

class FoundedFileForm : public QWidget
{
    Q_OBJECT

public:
    explicit FoundedFileForm(QTableWidget *table ,QWidget *parent = 0);
    ~FoundedFileForm();

private slots:
    void on_SelectButton_clicked();

    void on_SelectedButton_clicked();

signals:

    void signalChangeChartSize();

private:
    //   DownloadingFile m_FoundedFile;

    QTableWidget* m_parent;
    Ui::FoundedFileForm *ui;

};
