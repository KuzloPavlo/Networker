#pragma once
#include <QWidget>
#include <QLabel>
#include "FileStatus.h"

namespace Ui {
class Status;
}

class Status : public QWidget
{
    Q_OBJECT

public:
    explicit Status(QWidget *parent = 0);
    ~Status();
    void changeStatus(const FileStatus& fileStatus,const int& filePercents);

private:
    void changeStatusBar(const int& filePercents);
    void changeStatusLabel(const FileStatus& fileStatus);
    Ui::Status *ui;
    int m_percent = 0;
    FileStatus m_status;
};
