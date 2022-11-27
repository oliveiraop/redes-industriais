#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModbusDataUnit>
#include <QModbusTcpClient>

QT_BEGIN_NAMESPACE

class QModbusClient;

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
    void on_ligar_clicked();

    void on_desligar_clicked();

    void on_conectar_clicked();

private:
    Ui::MainWindow *ui;
    QModbusTcpClient *modbusDevice;
};

#endif // MAINWINDOW_H
