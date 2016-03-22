#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_pNewServerDialog(new NewServerDialog(this)),
    m_pNewFileDialog(new DownloadingFileDialog(this)),
    ui(new Ui::MainWindow), m_pClient(new Client())
{
    ui->setupUi(this);

    QPalette mainPall;
    mainPall.setColor (this->backgroundRole (), QColor(255, 255, 255, 255));

    this->setPalette (mainPall);

    QPalette topPall;
    topPall.setColor (ui->widget->backgroundRole (), QColor(54, 54, 54, 255));

    ui->widget->setPalette(topPall);

    ui->widget->setAutoFillBackground(true);

    this->setStyleSheet ("color: rgb(0, 0, 255)");

    ui->addServerBut->setToolTip("Add new Server");
    ui->createFileBut->setToolTip("Create new downloading file");


    ui->tableDownloads->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch );
    ui->tableDownloads->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableDownloads->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableDownloads->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);

    ui->tableFilters->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->searchEdit->setVisible(false);

    //-----------------------------------------------------------+
    // Section for experements                                   |
    //-----------------------------------------------------------+

    // 0,127,255,100

    // Status* newStatus = new Status(this);

     m_pClient->setCallBackFunctions(MainWindow::display);





    //-----------------------------------------------------------+
    // Section for experements                                   |
    //-----------------------------------------------------------+

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::display(const std::string str)
{
   // ui->textDisplay->setText(str);
    char strstr[100];

     std::strcpy(strstr, str.c_str());

     qDebug() << strstr;
}



void MainWindow::on_searchBut_clicked()
{
    if(!ui->searchEdit->isVisible())
    {
        ui->searchEdit->setVisible(true);
        ui->stackedWidgetLeft->setCurrentIndex(1);
    }
    else
    {
        ui->searchEdit->setVisible(false);
        ui->stackedWidgetLeft->setCurrentIndex(0);
    }
}

void MainWindow::on_addServerBut_clicked()
{
    m_pNewServerDialog->show();
}

void MainWindow::on_createFileBut_clicked()
{
   m_pNewFileDialog->show();
}

void MainWindow::on_sendButton_clicked()
{
    m_pClient->show(ui->textinput->toPlainText().toStdString());
}
