#ifndef MANAGER_H
#define MANAGER_H

#include "common.h"
#include "login.h"
#include "mainwindow.h"
#include "chatwindow.h"
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
#include <QModelIndex>
#include <QMap>

class Manager : public QObject
{
    Q_OBJECT
public:
    std::vector<User> vu;

    explicit Manager(QObject *parent = 0);

signals:
    //发送至login的信息
    void send_to_login(QString);
    //发送至mainWindow的信息
    void send_to_mainWindow(unsigned int id, QString data);

public slots:
    //读取并分析来自服务器的信息
    void read(void);
    //创建聊天窗口
    void new_chatWindow(QModelIndex);

private slots:

private:
    //Login窗口
    Login *login;
    //MainWindow窗口
    MainWindow *mainWindow;
    //id_ChatWindow
    QMap<unsigned int, ChatWindow*> id_chatWindow;

    //登录成功
    void login_successed(void);
};

#endif // MANAGER_H
