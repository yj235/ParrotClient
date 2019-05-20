#include "login.h"
#include "ui_login.h"

using namespace std;

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    connect(ui->lineEdit_name, SIGNAL(focus_out()), this, SLOT(focus_out()));
}

Login::~Login()
{
    delete ui;
}

void Login::closeEvent(QCloseEvent *)
{
    socket->disconnectFromHost();
}

void Login::on_pushButton_name_clicked()
{
    string data = "{query{name " + ui->lineEdit_name->text().toStdString() + "}}";
    socket->write(data.c_str(), data.length());
}

void Login::on_pushButton_password_clicked()
{
    string data = "{query{password " + ui->lineEdit_password->text().toStdString() + "}}";
    socket->write(data.c_str(), data.length());
}

void Login::focus_out()
{
    string data = "{query{name " + ui->lineEdit_name->text().toStdString() + "}}";
    socket->write(data.c_str(), data.length());
}

void Login::receive_message(QString message)
{
    if ("name not exist" == message) {
        ui->label_name_exist->setText(message);
    } else if ("password incorrect" == message) {
        ui->label_password_incorrect->setText(message);
    }
}

void Login::on_pushButton_clicked()
{
    if (ui->lineEdit_password->text().isEmpty()){
        ui->label_password_incorrect->setText("密码不能为空");
    } else {
        string data = "{query{password " + ui->lineEdit_password->text().toStdString() + "}}";
        socket->write(data.c_str(), data.length());
    }
}
