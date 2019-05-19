#ifndef MANAGER_H
#define MANAGER_H

//git test

#include "common.h"
#include "login.h"
#include "mainwindow.h"
#include "KVP.h"
#include "format.h"
#include "pdebug.h"

#include <QObject>
#include <QTcpSocket>

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);

signals:

public slots:
    void f(void);

private:
    Login *login;
    MainWindow *mainWindow;
};

#endif // MANAGER_H
