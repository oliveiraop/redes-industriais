#include "mainwindow.h"
#include "ui_mainwindow.h",
#include <QModbusDataUnit>
#include <QModbusTcpClient>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    modbusDevice(nullptr)
{
    ui->setupUi(this);
    modbusDevice = new QModbusTcpClient(this);
    ui->ip->setText("127.0.0.1:502");
    ui->ligar->setDisabled(true);
    ui->desligar->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ligar_clicked()
{
    ui->lamp->setStyleSheet("background-color: rgb(255, 255, 0);");
    QVector<quint16> data = {1};
    QModbusDataUnit writeRequest(QModbusDataUnit::Coils, 0, data);
    modbusDevice->sendWriteRequest(writeRequest, 1);
}

void MainWindow::on_desligar_clicked()
{
    ui->lamp->setStyleSheet("background-color: rgb(160, 160, 160);");
    QVector<quint16> data = {0};
    QModbusDataUnit writeRequest(QModbusDataUnit::Coils, 0, data);
    modbusDevice->sendWriteRequest(writeRequest, 1);

}

void MainWindow::on_conectar_clicked()
{
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        const QUrl url = QUrl::fromUserInput(ui->ip->text());
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        modbusDevice->setTimeout(1000);
        modbusDevice->setNumberOfRetries(3);
        if (modbusDevice->connectDevice()) {
            ui->conectar->setText("Desconectar");
            ui->ligar->setDisabled(false);
            ui->desligar->setDisabled(false);
        }
    } else {
        modbusDevice->disconnectDevice();
        ui->conectar->setText("Conectar");
        ui->ligar->setDisabled(true);
        ui->desligar->setDisabled(true);
    }
}
