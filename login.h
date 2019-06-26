#ifndef LOGIN_H
#define LOGIN_H

#include "common.h"
#include "pdebug.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <QShortcut>

#include <iostream>
#include <string>

#include <string.h>

//填入顺序bug
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void focus_out(void);
    void recv_from_manager(QString);
    void on_pushButton_clicked();
    void connected(void);

private:
    Ui::Login *ui;

    QShortcut *shortcut_close;
    bool doRegister = false;

    void closeEvent(QCloseEvent *);
};

#endif // LOGIN_H
