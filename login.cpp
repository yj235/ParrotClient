#include "login.h"
#include "ui_login.h"

using namespace std;

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //socket = new QTcpSocket();
    //socket->connectToHost("192.168.196.169", 8080);

    //connect(socket, SIGNAL(readyRead()), this, SLOT(logined()));
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_login_clicked()
{
    KVP *login = new KVP("login");
    //KVP *name = new KVP("name", ui->lineEdit_name->text().toStdString());
    //KVP *password = new KVP("password", ui->lineEdit_password->text().toStdString());
    KVP *user = new KVP(ui->lineEdit_name->text().toStdString(), ui->lineEdit_password->text().toStdString());

    login->sub = user;

    std::string s;
    format(s,login);
    delete login;
    socket->write(s.c_str(), s.length());
}

void Login::logined()
{
    char data[2] = {0};
    socket->read(data, 2);
    if (!strcmp(data, "1")){
        mainWindow = new MainWindow();
        mainWindow->show();
        this->hide();
    } else {
        std::cout << "failed" << std::endl;
    }
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
