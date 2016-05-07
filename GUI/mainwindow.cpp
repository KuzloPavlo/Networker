#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_pNewServerDialog(new NewServerDialog(this)),
    m_pNewFileDialog(new DownloadingFileDialog(this)),
    ui(new Ui::MainWindow), m_pClient(new Client()), m_debuger(new TemDebuger(this))
{
    ui->setupUi(this);

    qRegisterMetaType<std::string>();
    qRegisterMetaType<DownloadingFile>();
    qRegisterMetaType<FileInfo>();
    qRegisterMetaType<FileStatus>();

    QPalette mainPall;
    mainPall.setColor (this->backgroundRole (), QColor(255, 255, 255, 255));

    this->setPalette (mainPall);

    QPalette topPall;
    topPall.setColor (ui->widget->backgroundRole (), QColor(54, 54, 54, 255));

    ui->widget->setPalette(topPall);

    ui->widget->setAutoFillBackground(true);

    this->setStyleSheet ("color: rgb(0, 0, 255)");

    ui->homeButton->setVisible(false);

    ui->addServerBut->setToolTip("Add new Server");
    ui->createFileBut->setToolTip("Create new downloading file");
    ui->searchBut->setToolTip("Search files");
    ui->homeButton->setToolTip("Go Home");
    ui->showAllButton->setToolTip("Show All files");
    ui->refreshButton->setToolTip("Refresh Distribution");

    tableSetting(ui->tableAll);

    tableSetting(ui->tableDownloading);

    tableSetting(ui->tableDistributed);

    tableSetting(ui->tableCompleted);

    tableSetting(ui->tableActive);

    tableSetting(ui->tableInactive);

    // ui->tableFilters->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableSearch->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableSearch->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableSearch->setSelectionMode(QAbstractItemView::NoSelection);
    ui->searchEdit->setVisible(false);
    ui->showAllButton->setVisible(false);
    ui->refreshButton->setVisible(false);
    //-----------------------------------------------------------+
    // Section for experements                                   |
    //-----------------------------------------------------------+

    // 0,127,255,100

    // Status* newStatus = new Status(this);

    //    ui->textDisplay->setVisible(false);
    //    ui->textinput->setVisible(false);
    //    ui->sendButton->setVisible(false);
    //    emit on_sendButton_clicked();

   // this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);



    m_pClient->display = std::bind(&MainWindow::signalDisplay, this, std::placeholders::_1);

    connect(this,
            SIGNAL(signalDisplay(const std::string&)),
            this,
            SLOT(slotDisplay(const std::string&)));


    //-----------------------------------------------------------+
    // Section for experements                                   |
    //-----------------------------------------------------------+

    m_pClient->showFoundFile = std::bind(&MainWindow::signalShowFoundFile, this, std::placeholders::_1);

    connect(this,
            SIGNAL(signalShowFoundFile(const FileInfo&)),
            this,
            SLOT(slotShowFoundFile(const FileInfo&)));



    connect(m_pNewServerDialog,
            SIGNAL(signalConnectToServer(std::string,std::string)),
            this,
            SLOT(slotConnectToServer(std::string,std::string)));

    connect(m_pNewFileDialog,
            SIGNAL(signalCreateNewDownloadingFile(QString,QString)),
            this,
            SLOT(slotCreateNewDownloadingFile(QString,QString))
            );

    connect(this,
            SIGNAL (signalDownloadFile(const FileInfo&, const QString&)),
            this,
            SLOT(slotDownloadFile(const FileInfo&, const QString&))
            );


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotDisplay(const std::string& str)
{
    m_debuger->display(str);
}

void MainWindow::slotConnectToServer(const std::string &IPaddress, const std::string &port)
{
    m_pClient->connectToServer(IPaddress, port);
}

void MainWindow::on_searchBut_clicked()
{
    ui->searchEdit->setVisible(true);
    ui->showAllButton->setVisible(true);
    ui->stackedWidgetLeft->setCurrentIndex(1);
    ui->homeButton->setVisible(true);
    ui->searchBut->setVisible(false);
    ui->refreshButton->setVisible(true);
}

void MainWindow::on_homeButton_clicked()
{
    ui->searchEdit->setVisible(false);
    ui->showAllButton->setVisible(false);
    ui->stackedWidgetLeft->setCurrentIndex(0);
    ui->homeButton->setVisible(false);
    ui->searchBut->setVisible(true);
    ui->refreshButton->setVisible(false);
}

void MainWindow::on_addServerBut_clicked()
{
    m_pNewServerDialog->show();
}

void MainWindow::on_createFileBut_clicked()
{
    m_pNewFileDialog->show();
}

//void MainWindow::on_sendButton_clicked()
//{
//    //ui->tableDownloading->setSpan(0,0,1,ui->tableDownloading->columnCount());
//    FileInfo fi;
//    emit slotDownloadFile(fi, "akdsjf");

//}


std::string MainWindow::changeLocationStyle(const QString& QtStyleLocation)
{
    std::string windowsStyleLocation = QtStyleLocation.toStdString();
    std::string winDivider = "\\";

    while(true)
    {
        size_t i = windowsStyleLocation.find('/');

        if(i == std::string::npos)
        {
            break;
        }

        windowsStyleLocation.replace(i,1,winDivider);
    }
    return windowsStyleLocation;
}


void MainWindow::slotCreateNewDownloadingFile(const QString &location, const QString &description)
{
    std::string windowsStyleLocation = changeLocationStyle(location);
    std::string specification = description.toStdString();
    emit slotDisplay("slotCreateNewDownloadingFile");
    m_pClient->createNewDownloadingFile(windowsStyleLocation,specification);
}

void MainWindow::slotAddNewDownloadingFile(const DownloadingFile& newFile)
{

}

void MainWindow::on_searchEdit_returnPressed()
{
    if(!ui->searchEdit->text().isEmpty())
    {
        ui->tableSearch->setRowCount(0);
        m_pClient->searchFile(ui->searchEdit->text().toStdString());
    }
    else
    {
        ui->tableSearch->setRowCount(0);
        m_pClient->searchFile("*");
    }
}

void MainWindow::slotShowFoundFile(const FileInfo& foundFile)
{
    //------------------------------------
    emit slotDisplay(foundFile.m_fileName);
    //------------------------------------------
    FoundedFileForm* newForm = new FoundedFileForm(foundFile,ui->tableSearch, ui->tableSearch);
    newForm->downloadFile = std::bind(&MainWindow::signalDownloadFile,this, std::placeholders::_1, std::placeholders::_2);

    ui->tableSearch->insertRow(ui->tableSearch->rowCount());
    ui->tableSearch->setCellWidget(ui->tableSearch->rowCount()-1,0, newForm);
}

void MainWindow::slotDownloadFile(const FileInfo &foundFile, const QString &QtLocation)
{
    //-------------------------------------------
    emit slotDisplay(QtLocation.toStdString());
    //------------------------------------------

    std::string location = changeLocationStyle(QtLocation);

    DownloadingFile file;
    file.m_fileInfo = foundFile;
    strcpy_s(file.m_fileLocation,location.c_str());
    file.m_fileStatus = FileStatus::creating;

    FileForm* newFile = new FileForm(
                file,
                ui->tableAll,
                ui->tableDownloading,
                ui->tableDistributed,
                ui->tableCompleted,
                ui->tableActive,
                ui->tableInactive,
                ui->tableDownloading);

    m_pClient->downloadFile(file, newFile->changeFileStatus, newFile->changeDownloader,
                            newFile->getPrimitives(), newFile->getDownloaderStatus());

}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedDownloads->setCurrentIndex(0);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedDownloads->setCurrentIndex(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedDownloads->setCurrentIndex(2);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedDownloads->setCurrentIndex(3);
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedDownloads->setCurrentIndex(4
                                          );
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->stackedDownloads->setCurrentIndex(5);
}


void MainWindow::tableSetting(QTableWidget* table)
{
    table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch );
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    table->setColumnWidth(2,200);
    table->setColumnWidth(0,75);
    table->setColumnWidth(3,100);
    table->horizontalHeader()->setVisible(true);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_showAllButton_clicked()
{
    ui->tableSearch->setRowCount(0);
    m_pClient->searchFile("*");
}

void MainWindow::on_pushButton_7_clicked()
{
    if(m_debuger->isHidden())
    {
        m_debuger->move(x()+this->frameGeometry().width(),y());
        m_debuger->resize(200,this->height());
        m_debuger->show();
    }
    else
    {
        m_debuger->hide();
    }
}
