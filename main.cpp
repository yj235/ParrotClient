#include "manager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QApplication::setQuitOnLastWindowClosed(true);
    Manager m;

    return a.exec();
}
