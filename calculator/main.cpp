#include <QtCore/qcoreapplication.h>
#include <QApplication>
#include <QMainWindow>
#include <QtWidgets>
#include <QLineEdit> // 文本框
#include <QIntValidator>

class MainWindow : public QMainWindow {
    QLineEdit *m_line;
    enum Operator {
        None,
        Plus,
        Minus,
        Multiply,
        Divide
    };
    double m_last_value = 0;
    Operator m_last_operator = None;
public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Calculator");
        setFixedSize(400, 300);
        setCentralWidget(new QWidget);
        
        QGridLayout* layout = new QGridLayout;
        centralWidget()->setLayout(layout);

        m_line = new QLineEdit;
        m_line->setValidator(new QDoubleValidator); // 限制类型
        layout->addWidget(m_line, 0, 0, 1, 4);

        for (int i = 1; i <= 9; i++) {
            QPushButton* button = new QPushButton;
            button->setText(QString::number(i));
            layout->addWidget(button, 1 + (i - 1) / 3, (i - 1) % 3);
            
            connect(button, &QPushButton::clicked, this, [this, i] {
                pressNumber(i);
            });
        }

        QList<QPair<Operator, QString>> operators = {
            {Plus, "+"},
            {Minus, "-"},
            {Multiply, "*"},
            {Divide, "/"},
        };

        for (int i = 0; i < operators.size(); i++) {
            Operator op = operators[i].first;
            QString name = operators[i].second;

            QPushButton* button = new QPushButton;
            button->setText(name);
            layout->addWidget(button, 1 + i, 3);
            connect(button, &QPushButton::clicked, this, [this, op] {
                this->pressOperator(op);
            }); // 回调性质的要绑定对象，至少也应该是用=捕获，用&引用捕获有风险，空悬引用
        }

        QPushButton* button = new QPushButton;
        button->setText("0");
        layout->addWidget(button, 4, 1, 1, 1);
        connect(button, &QPushButton::clicked, this, [this] {
            this->pressNumber(0);
        });

        button = new QPushButton;
        button->setText("<-");
        layout->addWidget(button, 4, 2, 1, 1);
        connect(button, &QPushButton::clicked, this, [this] {
            this->pressBackspace();
        });

        button = new QPushButton;
        button->setText(".");
        layout->addWidget(button, 4, 0, 1, 1);
        connect(button, &QPushButton::clicked, this, [this] {
            this->pressDot();
        });

        button = new QPushButton;
        button->setText("=");
        layout->addWidget(button, 5, 0, 1, 4);
        connect(button, &QPushButton::clicked, this, [this] {
            this->pressEqual();
        });
    }
    
    void pressNumber(int i) {
        QString text = m_line->text();
        text += QString::number(i);
        setLineText(text);
    }

    void pressOperator(Operator op) {
        double value = m_line->text().toDouble();
        m_last_value = value;
        m_last_operator = op;
        m_line->setText("0");
    }

    void pressEqual() {
        double value1 = m_last_value;
        double value2 = m_line->text().toDouble();
        double value;
        switch (m_last_operator) {
            case None: value = value2; break;
            case Plus: value = value1 + value2; break;
            case Minus: value = value1 - value2; break;
            case Multiply: value = value1 * value2; break;
            case Divide: value = value1 / value2; break;
        }
        qDebug() << value;
        m_last_operator = None;
        setLineText(QString::number(value, 'g', 10));
    }

    void pressBackspace() {
        QString text = m_line->text();
        if (text.size() > 0) {
            text.remove(text.size() - 1, 1);
        }
        setLineText(text);
    }

    void pressDot() {
        QString text = m_line->text();
        text += ".";
        setLineText(text);
    }

    void setLineText(QString text) {
        int pos;
        if (m_line->validator()->validate(text, pos) != QValidator::Invalid) {
            while (text.size() >= 2 && text[0] == '0' && (text[1] >= '0' && text[1] <= '9')) {
                text.remove(0, 1);
            }
            if (text.size() == 0) {
                text = "0";
            }
            m_line->setText(text);
        }
    }

    ~MainWindow() {}
};

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow win;
    win.show();
    return QCoreApplication::exec();
}