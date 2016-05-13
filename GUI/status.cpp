#include "status.h"
#include "ui_status.h"

Status::Status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Status),
    m_status(FileStatus::creating)
{
    ui->setupUi(this);
}

Status::~Status()
{
    delete ui;
}

void Status::changeStatus(const FileStatus& fileStatus,const int& filePercents)
{
    if(m_percent < filePercents )
    {
        changeStatusBar(filePercents);
    }
    if(m_status != fileStatus)
    {
        changeStatusLabel(fileStatus);
    }
}

void Status::changeStatusBar(const int &filePercents)
{

    while (m_percent < filePercents && m_percent < 100)
    {
        QWidget* part = new QWidget(this);

        QPalette pall;

        pall.setColor (part->backgroundRole(), QColor(0,127,255,100));

        part->setPalette(pall);

        part->resize(2,30);

        part->move(m_percent*2,0);

        part->setAutoFillBackground(true);

        part->show();

        m_percent++;

        ui->value->setText(std::to_string(m_percent).c_str());
    }

    if(filePercents == 99)
    {
     m_percent++;

     QWidget* part = new QWidget(this);

        QPalette pall;

        pall.setColor (part->backgroundRole(), QColor(0,127,255,100));

        part->setPalette(pall);

        part->resize(2,30);

        part->move(m_percent*2,0);

        part->setAutoFillBackground(true);

        part->show();

        ui->value->setText(std::to_string(m_percent).c_str());
    }


}

void Status::changeStatusLabel(const FileStatus &fileStatus)
{
    m_status = fileStatus;

    switch (fileStatus)
    {
    case FileStatus::creating:
        ui->status->setText("Creating");
        changeStatusFon(Qt::darkGray);
        break;

    case FileStatus::downloading:
        ui->status->setText("Downloading");
        changeStatusFon(QColor(0,0,0,0));
        break;

    case FileStatus::verification:
        ui->status->setText("Verification");
        changeStatusFon(QColor(200,0,0,150));
        break;

    case FileStatus::distribution:
        ui->status->setText("Distribution");
        changeStatusFon(QColor(0,0,0,0));
        break;

    case FileStatus::downloaded:
        ui->status->setText("Downloaded");
        changeStatusFon(QColor(0,0,0,0));
        break;

    case FileStatus::pause:
        ui->status->setText("Pause");
        changeStatusFon(QColor(0,0,0,0));
        break;

    case FileStatus::deleting:
        ui->status->setText("Deleting");
        changeStatusFon(QColor(200,0,0,150));
        break;

    case FileStatus::failing:
        ui->status->setText("Failing");
        changeStatusFon(QColor(200,0,0,150));
        break;
    }
}


void Status::changeStatusFon(const QColor& color)
{
    QPalette pall;
    pall.setColor (ui->widget->backgroundRole (), color);
    ui->widget->setPalette(pall);
    ui->widget->setAutoFillBackground(true);
}
