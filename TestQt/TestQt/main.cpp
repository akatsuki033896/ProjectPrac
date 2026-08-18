#include "TestQt.h"
#include <s421/S421.h>
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestQt window;
    
    // vs调试根目录：./TestQt 编码UTF-8
    s421::S421Parser parser;
    auto ds = parser.parseFile("RTE-TEST-GBASIC_20260604.s421");  // parse .s421
    // auto ds = parser.parseCompressedFile("C:\\Users\\Administrator\\Desktop\\TestQt\\GBASIC.s421p");            // parse .s421p

    // parseCompressedFile returns nullptr on failure: must check before dereferencing
    if (!ds) {
        QMessageBox::critical(nullptr, QStringLiteral("S421 parse failed"),
            QStringLiteral("parseCompressedFile returned nullptr:\n%1")
                .arg(QString::fromStdString(parser.getLastError())));
        return 1;
    }

    s421::S421Validator validator;
    auto result = validator.validate(*ds);

    qDebug() << "validate: ok =" << result.ok()
             << " errors =" << (int)result.errorCount()
             << " warnings =" << (int)result.warningCount();
    for (const auto& e : result.errors)
        qDebug() << QString::fromStdString(e.toString());
    for (const auto& w : result.warnings)
        qDebug() << QString::fromStdString(w.toString());

    s421::S421Serializer serializer;
    serializer.serializeToCompressed(*ds, "out.s421");

    window.show();
    return app.exec();
}
