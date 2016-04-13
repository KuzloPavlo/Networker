#include "descriptionform.h"
#include "ui_descriptionform.h"

DescriptionForm::DescriptionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DescriptionForm)
{
    ui->setupUi(this);
}

DescriptionForm::~DescriptionForm()
{
    delete ui;
}
