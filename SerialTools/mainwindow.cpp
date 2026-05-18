#include "mainwindow.h"

#include <qbytearray.h>
#include <qcombobox.h>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qserialport.h>
#include <qserialportinfo.h>
#include <qtextbrowser.h>
#include <qtextcursor.h>
#include <qtextedit.h>

#include <QMessageBox>
#include <QStringList>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // ui
    resize(850, 500);
    setWindowTitle("Serial Tool");
    browser_bar = new QTextBrowser(this);
    browser_bar->setGeometry(20, 20, 540, 290);
    edit_bar = new QTextEdit(this);
    edit_bar->setGeometry(20, 340, 540, 145);

    available_com = new QComboBox(this);
    available_com->setGeometry(650, 20, 150, 40);
    available_com_text = new QLabel(this);
    available_com_text->setText("Serial");
    available_com_text->setGeometry(580, 20, 150, 40);

    baud_rate = new QComboBox(this);
    baud_rate->setGeometry(650, 70, 150, 40);
    QStringList baud_list = {"115200", "38400", "19200", "9600"};
    baud_rate->addItems(baud_list);
    baud_rate_text = new QLabel(this);
    baud_rate_text->setText("Baud Rate");
    baud_rate_text->setGeometry(580, 70, 150, 40);

    refresh = new QPushButton(this);
    refresh->setGeometry(580, 145, 150, 40);
    refresh->setText("Refresh Serial");
    connect(refresh, &QPushButton::clicked, this,
            &MainWindow::on_refresh_clicked);

    on_off = new QPushButton(this);
    on_off->setGeometry(580, 205, 150, 40);
    on_off->setText("Open Serial");
    connect(on_off, &QPushButton::clicked, this,
            &MainWindow::on_switch_clicked);

    clean_rx = new QPushButton(this);
    clean_rx->setGeometry(580, 265, 150, 40);
    clean_rx->setText("Clean Receive");
    connect(clean_rx, &QPushButton::clicked, this,
            &MainWindow::on_clean_rx_clicked);

    send = new QPushButton(this);
    send->setGeometry(580, 385, 150, 40);
    send->setText("Send");
    connect(send, &QPushButton::clicked, this, &MainWindow::on_tx_clicked);

    clean_tx = new QPushButton(this);
    clean_tx->setGeometry(580, 445, 150, 40);
    clean_tx->setText("Clean");
    connect(clean_tx, &QPushButton::clicked, this,
            &MainWindow::on_clean_tx_clicked);

    COM = new QSerialPort(this);  // 实例化串口
    // initial all COM
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& portInfo : serialPortInfos) {
        qDebug() << "Port:" << portInfo.portName() << '\n'
                 << "Serial number:" << portInfo.serialNumber();
        available_com->addItem(portInfo.portName());
    }
    connect(COM, &QSerialPort::readyRead, this, &MainWindow::Rx_receive);
}

MainWindow::~MainWindow() {}

void MainWindow::on_clean_tx_clicked() { edit_bar->clear(); }

void MainWindow::on_clean_rx_clicked() { browser_bar->clear(); }

void MainWindow::on_refresh_clicked() {
    available_com->clear();
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& portInfo : serialPortInfos) {
        qDebug() << "Port:" << portInfo.portName() << '\n'
                 << "Serial number:" << portInfo.serialNumber();
        available_com->addItem(portInfo.portName());
    }
}

void MainWindow::on_switch_clicked() {
    // initialize serial port param

    QSerialPort::BaudRate baudRate;
    QSerialPort::StopBits stopBits = QSerialPort::OneStop;  // 停止位
    QSerialPort::DataBits dataBits = QSerialPort::Data8;    // 数据位
    QSerialPort::Parity checkBits = QSerialPort::NoParity;  // 校验位
    if (baud_rate->currentText() == "115200") {
        baudRate = QSerialPort::Baud115200;
    } else if (baud_rate->currentText() == "38400") {
        baudRate = QSerialPort::Baud38400;
    } else if (baud_rate->currentText() == "19200") {
        baudRate = QSerialPort::Baud19200;
    } else if (baud_rate->currentText() == "9600") {
        baudRate = QSerialPort::Baud9600;
    }

    // initialize serial port
    COM->setPortName(available_com->currentText());
    COM->setBaudRate(baudRate);
    COM->setStopBits(stopBits);
    COM->setDataBits(dataBits);
    COM->setParity(checkBits);

    qDebug() << baudRate << " " << stopBits << " " << dataBits << " "
             << checkBits;
    qDebug() << COM->isOpen();
    qDebug() << COM->errorString();

    if (on_off->text() == "Open Serial") {
        // https://doc.qt.io/qt-6/qserialport.html#open
        if (COM->open(QIODevice::ReadWrite)) {
            on_off->setText("Close Serial");
        } else {
            QMessageBox::critical(this, "warning",
                                  "Open serial failed: Select correct serial "
                                  "port or the port is used.\n");
        }
    } else if (on_off->text() == "Close Serial") {
        COM->close();
        on_off->setText("Open Serial");
    }
}

void MainWindow::on_tx_clicked() {
    // qDebug() << available_com->currentText();
    qDebug() << COM->openMode();
    if (edit_bar->document()->isEmpty()) {
        QMessageBox::critical(this, "warning", "Can't send empty message.\n");
    }
    if (COM->openMode() == QIODevice::ReadWrite) {
        COM->write(edit_bar->toPlainText().toLatin1());
    } else {
        QMessageBox::critical(this, "warning", "Device not opened.\n");
    }
    edit_bar->clear();
}

void MainWindow::Rx_receive() {
    QByteArray buffer;  // 接收字符缓冲区
    buffer = COM->readAll();
    qDebug() << "buffer: " << buffer;

    QString str;
    if (!buffer.isEmpty()) {
        str = QString(buffer);
        browser_bar->insertPlainText(str + '\n');
        browser_bar->moveCursor(QTextCursor::End);
    }
}