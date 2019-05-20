#ifndef MANAGER_H
#define MANAGER_H

#include "common.h"
#include "login.h"
#include "mainwindow.h"
#include "KVP.h"
#include "format.h"
#include "pdebug.h"

#include <QObject>
#include <QTcpSocket>
#include <QString>

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);

signals:
    void send_message(QString);

public slots:
    void f(void);

private:
    void login_successed(void);

    Login *login;
    MainWindow *mainWindow;
};

#endif // MANAGER_H
