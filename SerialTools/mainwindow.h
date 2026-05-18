#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qserialport.h>
#include <qtextbrowser.h>
#include <qtextedit.h>

#include <QMainWindow>
#include <QSerialPort>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
private slots:
    void on_clean_tx_clicked();
    void on_clean_rx_clicked();
    void on_refresh_clicked();
    void on_tx_clicked();
    void on_switch_clicked();  // open serial
    void Rx_receive();         // receive data via serial port
private:
    QTextBrowser* browser_bar;
    QTextEdit* edit_bar;

    QLabel* available_com_text;
    QLabel* baud_rate_text;
    QComboBox* available_com;
    QComboBox* baud_rate;

    QPushButton* refresh;  // refresh serial
    QPushButton* on_off;   // open serial
    QPushButton* clean_rx;

    QPushButton* send;      // send via serial
    QPushButton* clean_tx;  // clean input
    
    QSerialPort* COM;
};
#endif  // MAINWINDOW_H
