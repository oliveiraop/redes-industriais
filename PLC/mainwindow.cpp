#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QModbusTcpServer>
#include <QUrl>
#include <QDebug>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    modbusTcpServer = new QModbusTcpServer(this);
    ui->IP->setText("127.0.0.1:502");
    QModbusDataUnitMap reg;
    reg.insert(QModbusDataUnit::Coils, { QModbusDataUnit::Coils, 0, 10 });
    modbusTcpServer->setMap(reg);
    connect(modbusTcpServer, &QModbusServer::dataWritten, this, &MainWindow::updateWidgets);

    modbusRtuMaster = new QModbusRtuSerialMaster(this);
#ifdef WIN32
    ui->porta->setText("COM1");
#endif
#ifdef __linux__
    ui->porta->setText("/dev/ttyUSB0");
#endif


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWidgets(QModbusDataUnit::RegisterType table, int address, int size)
{
    qDebug() << table;
    quint16 value;
    qDebug() << "teste received";
    modbusTcpServer->data(QModbusDataUnit::Coils, 0, &value);
    if (value) {
        ui->lamp->setStyleSheet("background-color: rgb(255, 255, 0);");
        QVector<quint16> dataReceived = {1};
        if (modbusRtuMaster->state() == QModbusDevice::ConnectedState) {
            QModbusDataUnit writeRequest(QModbusDataUnit::Coils, 0, dataReceived);
            modbusRtuMaster->sendWriteRequest(writeRequest, 1);
        }
    } else {
        ui->lamp->setStyleSheet("background-color: rgb(160, 160, 160);");
        QVector<quint16> dataReceived = {0};
        if (modbusRtuMaster->state() == QModbusDevice::ConnectedState) {
            QModbusDataUnit writeRequest(QModbusDataUnit::Coils, 0, dataReceived);
            modbusRtuMaster->sendWriteRequest(writeRequest, 1);
        }
    }

}

void MainWindow::on_abrir_clicked()
{
    qDebug() << modbusTcpServer->state();
    if (modbusTcpServer->state() == QModbusDevice::UnconnectedState) {

        const QUrl url = QUrl::fromUserInput(ui->IP->text());
        qDebug() << url.host();
        qDebug() << url.port();
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

void MainWindow::on_conectar_clicked()
{
    if (modbusRtuMaster->state() != QModbusDevice::ConnectedState) {
        modbusRtuMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
            ui->porta->text());
        modbusRtuMaster->setConnectionParameter(QModbusDevice::SerialParityParameter,
            QSerialPort::EvenParity);
        modbusRtuMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
            QSerialPort::Baud19200);
        modbusRtuMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
            QSerialPort::Data8);
        modbusRtuMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
            QSerialPort::OneStop);
        modbusRtuMaster->setTimeout(1000);
        modbusRtuMaster->setNumberOfRetries(3);
        if (!modbusRtuMaster->connectDevice()) {
            qDebug() << modbusRtuMaster->errorString();
            ui->conectar->setText("Desconectar");
        }
    } else {
        modbusRtuMaster->disconnectDevice();
        ui->conectar->setText("Conectar");
    }
}
