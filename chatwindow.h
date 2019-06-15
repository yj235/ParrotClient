#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "common.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <string>

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

    void recv_data(std::string time, std::string message);

    void open(void);

public slots:

signals:
    void close(unsigned int);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_send_clicked();

    void showHistoryMessage(void);

    void on_pushButton_clicked();

private:
    Ui::ChatWindow *ui;

    //退出快捷键
    QShortcut *shortCut_close;
};

#endif // CHATWINDOW_H
