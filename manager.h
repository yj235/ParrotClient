#ifndef MANAGER_H
#define MANAGER_H

#include "common.h"
#include "login.h"
#include "mainwindow.h"
#include "chatwindow.h"
#include "pdebug.h"
#include "user.h"
#include "rapidjson/document.h"
#include "groupwindow.h"

#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QModelIndex>
#include <QMap>
#include <QMessageBox>

class Manager : public QObject
{
    Q_OBJECT
public:
    //没用?
    std::vector<User> vu;
    //MainWindow窗口
    MainWindow *mainWindow;

    explicit Manager(QObject *parent = 0);

signals:
    //发送至login的信息
    void send_to_login(QString);
    //发送至mainWindow的信息
    //void send_to_mainWindow(unsigned int id, QString data);

    //***临时***
    //id_mq 在new ChatWindow后显示空
    void chatWindow_show_historyMessage(void);

public slots:
    //读取来自服务器的信息
    void read(void);
    //创建聊天窗口
    void new_chatWindow(QModelIndex);
    //创建群窗口
    void new_groupWindow(QModelIndex index);

private slots:
    //聊天窗口关闭处理
    //移除id_chatWindow
    void chatWindow_close(unsigned int id);

private:
    //Login窗口
    Login *login;
    //id_ChatWindow
    QMap<unsigned int, ChatWindow*> id_chatWindow;
    QMap<unsigned int, GroupWindow*> id_groupWindow;

    //登录成功
    void login_successed(void);
    //请求消息队列
    void ask_for_message(void);
    //转换函数
    void my_parse(char *data);
};

#endif // MANAGER_H
