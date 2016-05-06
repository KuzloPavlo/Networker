#include "temdebuger.h"
#include "ui_temdebuger.h"

TemDebuger::TemDebuger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TemDebuger)
{
    ui->setupUi(this);
    //this->move() // first v, second h
}

TemDebuger::~TemDebuger()
{
    delete ui;
}

void TemDebuger::display(const std::string &str)
{
    char strstr[10000];
    std::strcpy(strstr, str.c_str());
    this->ui->textEdit->append(strstr);
}
