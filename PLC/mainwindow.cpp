#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QModbusTcpServer>
#include <QUrl>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    modbusTcpServer = new QModbusTcpServer(this);
    ui->IP->setText("127.0.0.1:502");
    QModbusDataUnitMap reg;
    reg.insert(QModbusDataUnit::Coils, { QModbusDataUnit::Coils, 0, 0 });
    connect(modbusTcpServer, &QModbusServer::dataWritten,
            this, &MainWindow::updateWidgets);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWidgets(QModbusDataUnit::RegisterType table, int address, int size)
{

}

void MainWindow::on_abrir_clicked()
{
    qDebug() << modbusTcpServer->state();
    if (modbusTcpServer->state() == QModbusDevice::UnconnectedState) {
        const QUrl url = QUrl::fromUserInput(ui->IP->text());
        modbusTcpServer->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        modbusTcpServer->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        modbusTcpServer->setServerAddress(1);
        if (modbusTcpServer->connectDevice()) {
            ui->abrir->setText("Fechar");
        } else {
            qDebug() << modbusTcpServer->errorString();
        }
    } else {
        modbusTcpServer->disconnectDevice();
        ui->abrir->setText("Abrir");
    }
}
