#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qsqldatabase.h>
#include <qstackedwidget.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void on_loginButton_clicked();
    void on_signUp_clicked();
private:
    QStackedWidget* stack;
    QWidget* loginPage;
    QWidget* signUpPage;
    
    void init_sign_up();
    void init_sign_in();
    void set_sign_in();
    void set_sign_up();
    
    void open_db();
    bool check_empty_text(QString acc_str, QString pwd_str);
    bool search_db(QString acc_str, QString pwd_str);
    bool search_db(QString str);
    bool insert_db(QString acc_str, QString pwd_str);
    
    // ui
    QLabel* login_title;
    QLabel* acc_label;
    QLabel* pwd_label;
    
    QLineEdit* acc;
    QLineEdit* pwd;

    QPushButton* login;
    QPushButton* sign_up;
    QPushButton* switch_sign;
    // sql
    QSqlDatabase db;
};
#endif  // MAINWINDOW_H
