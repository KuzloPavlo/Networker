#include "status.h"
#include "ui_status.h"

Status::Status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);



int i = 0;


 for ( i ; i < 200; i+=2)
 {
 QWidget* part = new QWidget(this);

    QPalette pall;

     pall.setColor (part->backgroundRole (), QColor(0,127,255,100));

     part->setPalette(pall);

     part->resize(2,40);

     part->move(i,0);

     part->setAutoFillBackground(true);
}



}

Status::~Status()
{
    delete ui;
}


void Status::changeStatus()
{

    QWidget* part = new QWidget(this);

    QPalette pall;

     pall.setColor (part->backgroundRole (), QColor(0,127,255,100));

     part->setPalette(pall);

     part->resize(2,40);

     part->move(10,0);

     part->setAutoFillBackground(true);
}

void Status::changeLoadValue()
{

}
