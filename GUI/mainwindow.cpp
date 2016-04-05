#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_pNewServerDialog(new NewServerDialog(this)),
    m_pNewFileDialog(new DownloadingFileDialog(this)),
    ui(new Ui::MainWindow), m_pClient(new Client())
{
    ui->setupUi(this);

    qRegisterMetaType<std::string>();
    qRegisterMetaType<DownloadingFile>();

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

    ui->tableSearch->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableSearch->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableSearch->setSelectionMode(QAbstractItemView::NoSelection);
    ui->searchEdit->setVisible(false);

    //-----------------------------------------------------------+
    // Section for experements                                   |
    //-----------------------------------------------------------+

    // 0,127,255,100

    // Status* newStatus = new Status(this);

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
            SIGNAL(signalShowFoundFile(const DownloadingFile&)),
            this,
            SLOT(slotShowFoundFile(const DownloadingFile&)));



    connect(m_pNewServerDialog,
            SIGNAL(signalConnectToServer(std::string,std::string)),
            this,
            SLOT(slotConnectToServer(std::string,std::string)));

    connect(m_pNewFileDialog,
            SIGNAL(signalCreateNewDownloadingFile(QString,QString)),
            this,
            SLOT(slotCreateNewDownloadingFile(QString,QString))
            );
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotDisplay(const std::string& str)
{
    char strstr[100];
    std::strcpy(strstr, str.c_str());
    this->ui->textDisplay->append(strstr);
}

void MainWindow::slotConnectToServer(const std::string &IPaddress, const std::string &port)
{
    m_pClient->connectToServer(IPaddress, port);
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
    m_pClient->display(ui->textinput->toPlainText().toStdString());
}


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
}

void MainWindow::slotShowFoundFile(const DownloadingFile& foundFile)
{
    std::string sizeFile = std::to_string(foundFile.m_fileSize);
    ui->tableSearch->insertRow(ui->tableSearch->rowCount());
    ui->tableSearch->setCellWidget(ui->tableSearch->rowCount()-1,0,
                                   new FoundedFileForm(foundFile.m_fileName,
                                                       foundFile.m_fileDescription,
                                                       sizeFile.c_str(),
                                                       ui->tableSearch,                                                      ui->tableSearch));
}
