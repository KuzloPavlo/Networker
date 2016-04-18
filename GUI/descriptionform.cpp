#include "descriptionform.h"
#include "ui_descriptionform.h"

DescriptionForm::DescriptionForm(const QString& description, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DescriptionForm)
{
    ui->setupUi(this);
}

DescriptionForm::~DescriptionForm()
{
    delete ui;
}
