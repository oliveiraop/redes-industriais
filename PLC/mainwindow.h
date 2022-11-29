#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModbusTcpServer>
#include <QModbusRtuSerialMaster>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void updateWidgets(QModbusDataUnit::RegisterType table, int address, int size);

private slots:
    void on_abrir_clicked();


    void on_conectar_clicked();

private:
    Ui::MainWindow *ui;
    QModbusTcpServer * modbusTcpServer;
    QModbusRtuSerialMaster * modbusRtuMaster;
};

#endif // MAINWINDOW_H
