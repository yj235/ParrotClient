#include "manager.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QApplication::setQuitOnLastWindowClosed(true);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    Manager m;

    return a.exec();
}
