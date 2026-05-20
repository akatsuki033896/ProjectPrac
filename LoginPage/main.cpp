#include <QApplication>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QFile qssFile(":/style.qss");
    qDebug() << QFile::exists(":/style.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString style = qssFile.readAll();
        a.setStyleSheet(style);
        qssFile.close();
    }
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
