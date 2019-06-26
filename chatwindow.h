#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "common.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "pdebug.h"

#include <string>
#include <iostream>

#include <QWidget>
#include <QString>
#include <QShortcut>
#include <QDateTime>
#include <QDebug>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QTcpSocket>
#include <QTcpServer>

#define FILE_TRANSFER_PORT 12346
#define BLOCK_SIZE 4096

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    unsigned int contacts_id;

    explicit ChatWindow(QWidget *parent = 0);
    //有问题
    //ChatWindow(unsigned int id, QWidget *parent = 0) : contacts_id(id){}
    ~ChatWindow();

    //接收消息
    void recv_data(std::string time, std::string message);
    //被manager调用 向服务器发送联系人对话框打开消息
    void open(void);
    //发送文件
    //void startFileTransfer(unsigned int ip);
    void startFileTransfer(std::string ip);

signals:
    void close(unsigned int);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    //发送
    void on_pushButton_send_clicked();
    //显示历史消息
    void showHistoryMessage(void);
    //发送文件
    void on_pushButton_fileTransfer_clicked();
    //新链接
    void newConnection(void);
    //准备读
    void readyRead(void);
    void continueTransfer(qint64);

    void on_pushButton_cancel_clicked();
    void disconnected(void);
    void clean(void);

private:
    Ui::ChatWindow *ui;

    //退出快捷键
    QShortcut *shortCut_close;
    QTcpServer *server = NULL;
    QTcpSocket *socket = NULL;
    QFile *file = NULL;
    QString fileName;
    qint64 restBytes = 0;
    bool flag = true;
    double div = 1;
    bool cancel = false;

    //建立服务器
    void startServer(void);
};

#endif // CHATWINDOW_H
