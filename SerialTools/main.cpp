#include <QApplication>

#include "mainwindow.h"


int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    
    // QSerialPort port;
    // port.setPortName("COM3");
    // if(port.open(QIODevice::ReadWrite))
    //     qDebug() << "open ok";
    // else
    //     qDebug() << port.errorString();

    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
