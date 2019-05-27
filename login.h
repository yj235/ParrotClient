#ifndef LOGIN_H
#define LOGIN_H

#include "common.h"
#include "KVP.h"
#include "format.h"
#include "pdebug.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <QWidget>
#include <QTcpSocket>
#include <QString>

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
    void on_pushButton_name_clicked();
    void on_pushButton_password_clicked();
    void focus_out(void);
    void receive_message(QString);

    void on_pushButton_clicked();

private:
    Ui::Login *ui;

    void closeEvent(QCloseEvent *);
};

#endif // LOGIN_H
