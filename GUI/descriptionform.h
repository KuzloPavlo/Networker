#pragma once
#include <QWidget>

namespace Ui {
class DescriptionForm;
}

class DescriptionForm : public QWidget
{
    Q_OBJECT

public:
    explicit DescriptionForm(const QString& description, QWidget *parent = 0);
    ~DescriptionForm();
    void showDescription();
    void hidDescription();

private slots:
    void on_DowloadButton_clicked();

    void on_PauseButton_clicked();

    void on_DeleteButton_clicked();

private:
    Ui::DescriptionForm *ui;
};
