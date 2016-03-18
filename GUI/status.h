#pragma once
#include <QWidget>
#include <QLabel>

namespace Ui {
class Status;
}

class Status : public QWidget
{
    Q_OBJECT

public:
    explicit Status(QWidget *parent = 0);
    ~Status();

    void changeStatus();

    void changeLoadValue();

private:
    Ui::Status *ui;
};
