#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QString>

namespace Ui {
class FoundedFileForm;
}

class FoundedFileForm : public QWidget
{
    Q_OBJECT

public:
    explicit FoundedFileForm(
            const QString& fileName,
            const QString& fileDescription,
            const QString& fileSize,
            QTableWidget *parentTable,
            QWidget *parent = 0);
    ~FoundedFileForm();
private slots:
    void on_SelectButton_clicked();
    void on_SelectedButton_clicked();
signals:
    void signalChangeChartSize();
private:
    QTableWidget* m_parentTable;
    Ui::FoundedFileForm *ui;
};
