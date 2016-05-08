#pragma once
#include <QWidget>
#include "DownloadingFile.h"
#include "FileStatus.h"
#include "functional"

#define CHANGESTATUS std::function<void(const FileStatus& fileStatus)>

namespace Ui {
class DescriptionForm;
}

class DescriptionForm : public QWidget
{
    Q_OBJECT

public:
    explicit DescriptionForm(const FileInfo& description, QWidget *parent = 0);
    ~DescriptionForm();
    void showDescription();
    void hidDescription();
    void setFileInfo(const FileInfo& description);
    CHANGESTATUS setFilseStatus;
private slots:
    void on_DowloadButton_clicked();

    void on_PauseButton_clicked();

    void on_DeleteButton_clicked();

private:
    Ui::DescriptionForm *ui;
};
