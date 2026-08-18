/********************************************************************************
** Form generated from reading UI file 'TestQt.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTQT_H
#define UI_TESTQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TestQtClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TestQtClass)
    {
        if (TestQtClass->objectName().isEmpty())
            TestQtClass->setObjectName(QString::fromUtf8("TestQtClass"));
        TestQtClass->resize(600, 400);
        menuBar = new QMenuBar(TestQtClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        TestQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TestQtClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        TestQtClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(TestQtClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        TestQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(TestQtClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        TestQtClass->setStatusBar(statusBar);

        retranslateUi(TestQtClass);

        QMetaObject::connectSlotsByName(TestQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *TestQtClass)
    {
        TestQtClass->setWindowTitle(QCoreApplication::translate("TestQtClass", "TestQt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TestQtClass: public Ui_TestQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTQT_H
