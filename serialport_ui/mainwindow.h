#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

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
    void on_pushButton_openCOM_clicked();

    void on_pushButton_send_clicked();

    void readMycom();

    void on_pushButton_refresh_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *ui_serialport;
    QByteArray readdata;
    QTimer *timer;
};

#endif // MAINWINDOW_H
