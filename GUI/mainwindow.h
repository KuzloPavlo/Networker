#pragma once
#include <QMainWindow>
#include <QFileDialog>
#include <QTableWidget>
#include <newserverdialog.h>
#include <downloadingfiledialog.h>
#include <status.h>
#include <QMainWindow>
#include "Client.h"
#include <string>
#include <functional>
#include <memory>
Q_DECLARE_METATYPE(std::string)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_searchBut_clicked();

    void on_addServerBut_clicked();

    void on_createFileBut_clicked();

    void on_sendButton_clicked();

    void slotDisplay(const std::string& str);

    void slotConnectToServer(const std::string& IPaddress, const std::string& port);

    void slotCreateNewDownloadingFile(
            const QString& location,
            const QString& description);

signals:

    void signalDisplay(const std::string& str);

    void showNewFileStatus();

private:

    Ui::MainWindow *ui;
    Client *m_pClient;
    NewServerDialog *m_pNewServerDialog;
    DownloadingFileDialog *m_pNewFileDialog;
    std::string changeLocationStyle(const QString& QtStyleLocation);
};
