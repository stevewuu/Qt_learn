#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* get info of serial port */
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "port name    :" << info.portName();
        qDebug() << "description  :" << info.description();
        qDebug() << "manufacturer :" << info.manufacturer();

        QSerialPort com_port;
        com_port.setPort(info);

        if(com_port.open( QIODevice::ReadWrite))
        {
            ui->comboBox_comport->addItem(info.portName());
            com_port.close();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_openCOM_clicked()
{
    if(ui->pushButton_openCOM->text() == "Open")
    {
        // open port
        qDebug() << "open port";
        ui->pushButton_refresh->setEnabled(false);
        ui->pushButton_openCOM->setText(tr("Close"));
        ui_serialport = new QSerialPort(this);
        ui_serialport->setPortName(ui->comboBox_comport->currentText());
        ui_serialport->open(QIODevice::ReadWrite);

        //qDebug() << ui->comboBox_comport->currentText();

        ui_serialport->setBaudRate(ui->comboBox_baudrate->currentText().toInt());
        switch (ui->comboBox_data_bit->currentIndex()) {
        case 0:
            ui_serialport->setDataBits(QSerialPort::Data8);
            break;
        case 1:
            ui_serialport->setDataBits(QSerialPort::Data7);
            break;
    //    case 2: //Data9 not support yet

    //        break;
        default:
            ui_serialport->setDataBits(QSerialPort::Data8);
            break;
        }
        //ui_serialport->setDataBits(QSerialPort::Data8);
        //ui_serialport->setDataBits((QSerialPort::DataBits)(ui->comboBox_data_bit->currentText().toInt()));
        switch (ui->comboBox_parity->currentIndex()) {
        case 0:
            ui_serialport->setParity(QSerialPort::NoParity);
            break;
        case 1:
            ui_serialport->setParity(QSerialPort::OddParity);
            break;
        case 2:
            ui_serialport->setParity(QSerialPort::EvenParity);
            break;
        default:
            ui_serialport->setParity(QSerialPort::NoParity);
            break;
        }
        //ui_serialport->setParity(QSerialPort::NoParity);
        switch (ui->comboBox_flow_ctrl->currentIndex()) {
        case 0:
            ui_serialport->setFlowControl(QSerialPort::NoFlowControl);
            break;
        case 1:
            ui_serialport->setFlowControl(QSerialPort::HardwareControl);
            break;
        case 2:
            ui_serialport->setFlowControl(QSerialPort::SoftwareControl);
            break;
        default:
            ui_serialport->setFlowControl(QSerialPort::NoFlowControl);
            break;
        }
        //ui_serialport->setFlowControl(QSerialPort::NoFlowControl);
        switch (ui->comboBox_stop_bit->currentIndex()) {
        case 0:
            ui_serialport->setStopBits(QSerialPort::OneStop);
            break;
        case 1:
            ui_serialport->setStopBits(QSerialPort::OneAndHalfStop);
            break;
        case 2:
            ui_serialport->setStopBits(QSerialPort::TwoStop);
            break;
        default:
            ui_serialport->setStopBits(QSerialPort::OneStop);
            break;
        }
        //ui_serialport->setStopBits(QSerialPort::OneStop);

        //timer = new QTimer(this);

        connect(ui_serialport, SIGNAL( readyRead() ), this, SLOT( readMycom() ));

        qDebug() << "port name    :" << ui->comboBox_comport->currentText();
        qDebug() << "baudrate     :" << ui_serialport->baudRate();
        qDebug() << "data_bit     :" << ui_serialport->dataBits();
        qDebug() << "stop_bit     :" << ui_serialport->stopBits();
        qDebug() << "parity       :" << ui_serialport->parity();
        //timer->start(2000);
    }
    else
    {
        qDebug() << "Close port";
        if(ui_serialport->isOpen())
            ui_serialport->close();
        ui->pushButton_openCOM->setText(tr("Open"));
        ui->pushButton_refresh->setEnabled(true);
    }
}

void MainWindow::on_pushButton_send_clicked()
{
    ui_serialport->write(ui->lineEdit_write->text().toUtf8());
}


/*
** issue    : Add item duplicates
** solution : no yet
*/
void MainWindow::on_pushButton_refresh_clicked()
{
    // ! [1]
    ui->comboBox_comport->clear();
    QString _description;
    QString _manufacturer;
    QString _serialNumber;

    const auto infos = QSerialPortInfo::availablePorts();
    /* get info of serial port */
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        _description = info.description();
        _manufacturer = info.manufacturer();
        _serialNumber = info.serialNumber();
        list << info.portName()
             << (!_description.isEmpty() ? _description : blankString)
             << (!_manufacturer.isEmpty() ? _manufacturer : blankString)
             << (!_serialNumber.isEmpty() ? _serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->comboBox_comport->addItem(list.first(), list);
    }
//    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
//    {
//        //qDebug() << "port name    :" << info.portName();
//        //qDebug() << "description  :" << info.description();
//        //qDebug() << "manufacturer :" << info.manufacturer();
//        //qDebug() << "Open mode    :" << ;

//        QSerialPort com_port;
//        com_port.setPort(info);

//        if(com_port.open( QIODevice::ReadWrite))
//        {
//            qDebug() << "port name    :" << info.portName();
//            //if((ui->comboBox_comport->currentText()).contains(info.portName(), Qt::CaseInsensitive))
//            //ui->comboBox_comport->clear();
//            if( ( info.isValid() ) && ( ui->comboBox_comport->findText(info.portName()) ) )
//            ui->comboBox_comport->addItem(info.portName());
//            com_port.close();
//        }
//    }
}

void MainWindow::readMycom()
{
    readdata = ui_serialport->readAll();
    if(readdata != NULL)
    {
        //ui->textEdit_read->append(tr(readdata));  //received data append to textedit auto change to new line
        ui->textEdit_read->insertPlainText(readdata);
        ui->textEdit_read->moveCursor(QTextCursor::End);
    }
    readdata.clear();
}
