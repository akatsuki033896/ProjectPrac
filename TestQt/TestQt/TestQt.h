#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestQt.h"

class TestQt : public QMainWindow
{
    Q_OBJECT

public:
    TestQt(QWidget *parent = nullptr);
    ~TestQt();

private:
    Ui::TestQtClass ui;
};

