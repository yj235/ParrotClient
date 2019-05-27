#ifndef MANAGER_H
#define MANAGER_H

#include "common.h"
#include "login.h"
#include "mainwindow.h"
#include "KVP.h"
#include "format.h"
#include "pdebug.h"
#include "user.h"
#include "rapidjson/document.h"

#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QDebug>

class Manager : public QObject
{
    Q_OBJECT
public:
    std::vector<User> vu;

    explicit Manager(QObject *parent = 0);

signals:
    void send_message(QString);
    void recv_data(QString);

public slots:
    void f(void);

private:
    Login *login;
    MainWindow *mainWindow;

    void login_successed(void);
};

#endif // MANAGER_H
