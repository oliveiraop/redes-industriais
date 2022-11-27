#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModbusTcpServer>

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
    void on_abrir_clicked();

    void updateWidgets(QModbusDataUnit::RegisterType table, int address, int size);
private:
    Ui::MainWindow *ui;
    QModbusTcpServer * modbusTcpServer;
};

#endif // MAINWINDOW_H
