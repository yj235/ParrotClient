#ifndef LOGIN_H
#define LOGIN_H

#include "common.h"
#include "KVP.h"
#include "format.h"

#include "mainwindow.h"

#include <QWidget>
#include <QTcpSocket>

#include <iostream>
#include <string>

#include <string.h>

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
    void on_pushButton_login_clicked();

    void logined(void);

    void on_pushButton_name_clicked();

    void on_pushButton_password_clicked();

private:
    Ui::Login *ui;

    MainWindow *mainWindow;
    void closeEvent(QCloseEvent *);
};

#endif // LOGIN_H
