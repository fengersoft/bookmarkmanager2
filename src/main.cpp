#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QFile file(":/res/Ubuntu.qss");
    file.open(QIODevice::ReadOnly);
    QTextStream stm(&file);
    QString text = stm.readAll();
    qApp->setStyleSheet(text);
    file.close();
    MainWindow w;
    w.show();
    return a.exec();
}
