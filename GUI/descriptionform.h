#pragma once
#include <QWidget>

namespace Ui {
class DescriptionForm;
}

class DescriptionForm : public QWidget
{
    Q_OBJECT

public:
    explicit DescriptionForm(QWidget *parent = 0);
    ~DescriptionForm();

private:
    Ui::DescriptionForm *ui;
};
