#include "login.h"
#include "ui_login.h"

using namespace std;
using namespace rapidjson;

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

void Login::focus_out()
{
    if (ui->lineEdit_name->text().isEmpty()) {
        return;
    }
    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("query");
    writer.StartObject();
    writer.Key("name");
    writer.String(ui->lineEdit_name->text().toStdString().c_str());
    writer.EndObject();
    writer.EndObject();
    string data = sb.GetString();
    pdebug << data << endl;
    socket->write(data.c_str(), data.length());
}

void Login::recv_from_manager(QString data)
{
    if ("name exist" == data) {
        ui->label_name_existence->setText("");
    } else if ("name not exist" == data) {
        ui->label_name_existence->setText(data);
    } else if ("password incorrect" == data) {
        ui->label_password_incorrect->setText(data);
    } else if ("regist failed" == data) {
        ui->label_regist_failed->setText(data);
    }
}

void Login::on_pushButton_clicked()
{
    if (ui->lineEdit_password->text().isEmpty()){
        ui->label_password_incorrect->setText("密码不能为空");
        return;
    }
    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    writer.StartObject();
    if ("name not exist" == ui->label_name_existence->text()) {
        writer.Key("regist");
        writer.StartObject();
        writer.Key("name");
        writer.String(ui->lineEdit_name->text().toStdString().c_str());
        writer.Key("password");
        writer.String(ui->lineEdit_password->text().toStdString().c_str());
        writer.EndObject();
    } else {
        writer.Key("query");
        writer.StartObject();
        writer.Key("password");
        writer.String(ui->lineEdit_password->text().toStdString().c_str());
        writer.EndObject();
    }
    writer.EndObject();
    string data = sb.GetString();
    pdebug << data << endl;
    socket->write(data.c_str(), data.length());
}
