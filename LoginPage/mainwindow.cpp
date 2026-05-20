#include "mainwindow.h"

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qlogging.h>
#include <QtSql/qsqldatabase.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qstackedwidget.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qwidget.h>
#include <qmessagebox.h>
#include <qsqlerror.h>
#include <qsqlquery.h>

#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(400, 600);
    stack = new QStackedWidget(this);
    setCentralWidget(stack);
    open_db();
    init_sign_in();
}

void MainWindow::open_db() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/akatsuki/account.db");
    if (db.open()) {
        qDebug() << "open db success";
    } else {
        qDebug() << db.lastError().text();
    }
}

void MainWindow::init_sign_in() {
    loginPage = new QWidget();

    setWindowTitle("Sign In");

    login_title = new QLabel("Sign In", loginPage);
    login_title->setGeometry(30, 30, 100, 30);
    login_title->setObjectName("login_title");

    acc_label = new QLabel("Account", loginPage);
    acc_label->setGeometry(30, 80, 100, 30);

    acc = new QLineEdit(loginPage);
    acc->setGeometry(100, 130, 200, 30);

    pwd_label = new QLabel("Password", loginPage);
    pwd_label->setGeometry(30, 180, 100, 30);

    pwd = new QLineEdit(loginPage);
    pwd->setGeometry(100, 230, 200, 30);

    login = new QPushButton("Sign In", loginPage);
    login->setGeometry(125, 300, 150, 30);
    connect(login, &QPushButton::clicked, this,
            &MainWindow::on_loginButton_clicked);

    switch_sign = new QPushButton("Click here to sign up!", loginPage);
    switch_sign->setGeometry(125, 350, 150, 30);
    switch_sign->setStyleSheet(
        "QPushButton {"
        " border: none;"
        " background: transparent;"
        " color: white;"
        " text-decoration: underline;"
        "}"
        "QPushButton:hover {"
        " color: rgb(111, 207, 151);"
        "}");
    connect(switch_sign, &QPushButton::clicked, this, [&] {
        qDebug() << "switch clicked";
        init_sign_up();
        stack->setCurrentWidget(signUpPage);
    });  // switch to signup page

    stack->addWidget(loginPage);
}

void MainWindow::init_sign_up() {
    signUpPage = new QWidget();
    setWindowTitle("Sign Up");

    login_title->setText("Sign Up");
    login_title->setParent(signUpPage);

    acc_label->setParent(signUpPage);
    acc->setParent(signUpPage);
    pwd_label->setParent(signUpPage);
    pwd->setParent(signUpPage);

    sign_up = new QPushButton("Sign Up", signUpPage);
    sign_up->setGeometry(125, 300, 150, 30);
    connect(sign_up, &QPushButton::clicked, this,
            &MainWindow::on_signUp_clicked);

    switch_sign = new QPushButton("Back to Sign In!", signUpPage);
    switch_sign->setGeometry(125, 350, 150, 30);
    switch_sign->setStyleSheet(
        "QPushButton {"
        " border: none;"
        " background: transparent;"
        " color: white;"
        " text-decoration: underline;"
        "}"
        "QPushButton:hover {"
        " color: rgb(111, 207, 151);"
        "}");
    connect(switch_sign, &QPushButton::clicked, this, [&] {
        qDebug() << "switch clicked";
        set_sign_in();
        stack->setCurrentWidget(loginPage);
    });

    stack->addWidget(signUpPage);
}

void MainWindow::set_sign_in() {
    setWindowTitle("Sign In");
    login_title->setText("Sign In");
    login_title->setParent(loginPage);

    acc_label->setParent(loginPage);
    acc->setParent(loginPage);
    pwd_label->setParent(loginPage);
    pwd->setParent(loginPage);
}

void MainWindow::set_sign_up() {
    setWindowTitle("Sign Up");
    login_title->setText("Sign Up");
    login_title->setParent(signUpPage);

    acc_label->setParent(signUpPage);
    acc->setParent(signUpPage);
    pwd_label->setParent(signUpPage);
    pwd->setParent(signUpPage);
}

bool MainWindow::check_empty_text(QString acc_str, QString pwd_str) {
    if (acc_str.isEmpty() || pwd_str.isEmpty()) {
        qDebug() << "Empty str";
        QMessageBox::critical(this, "warning", "Warning: Empty Input!\n");
        return false;
    }
    qDebug() << acc_str << " " << pwd_str;
    return true;
}

bool MainWindow::search_db(QString acc_str, QString pwd_str) {
    // search
    QSqlQuery query(db);
    query.prepare(
        "SELECT * FROM USER WHERE username = :username AND password = "
        ":password");
    query.bindValue(":username", acc_str);
    query.bindValue(":password", pwd_str);
    if (query.exec()) {
        if (query.next()) {
            // qDebug() << "Login Success!";
            QMessageBox::information(this, "information", "Login Success!\n");
            return true;
        } else {
            QMessageBox::critical(this, "warning", "Warning: Login Failed!\n");
            return false;
        }
    } else {
        qDebug() << query.lastError();
        return false;
    }
}

bool MainWindow::search_db(QString str) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM USER WHERE username = :username");
    query.bindValue(":username", str);
    if (query.exec()) {
        if (query.next()) {
            return true;
        } else {
            return false;
        }
    } else {
        qDebug() << query.lastError();
        return false;
    }
}

bool MainWindow::insert_db(QString acc_str, QString pwd_str) {
    if (search_db(acc_str)) {
        QMessageBox::critical(this, "warning", "Warning: Existed Username\n");
        qDebug() << "search error";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO USER (username, password) VALUES (?, ?)");
    query.addBindValue(acc_str);
    query.addBindValue(pwd_str);
    if (!query.exec()) {
        qDebug() << "Failed to insert user:" << query.lastError();
        return false;
    }
    qDebug() << "Insert Succecs";
    return true;
}

void MainWindow::on_loginButton_clicked() {
    qDebug() << "login_clicked";

    QString acc_str = acc->text();
    QString pwd_str = pwd->text();
    check_empty_text(acc_str, pwd_str);

    search_db(acc_str, pwd_str);

    acc->clear();
    pwd->clear();
}

void MainWindow::on_signUp_clicked() {
    qDebug() << "sign_up_clicked";
    QString acc_str = acc->text();
    QString pwd_str = pwd->text();
    check_empty_text(acc_str, pwd_str);
    if (insert_db(acc_str, pwd_str)) {
        QMessageBox::information(this, "information", "Sign Up Success!\n");
    }
    acc->clear();
    pwd->clear();
}

MainWindow::~MainWindow() {
    if (db.isOpen()) {
        db.close();
    }
}
