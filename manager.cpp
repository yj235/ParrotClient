#include "manager.h"

using namespace std;

Manager::Manager(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
    socket->connectToHost("192.168.196.169", 8080);

    connect(socket, SIGNAL(readyRead()), this, SLOT(f()));

    login = new Login();
    login->show();

    connect(this, SIGNAL(send_message(QString)), login, SLOT(receive_message(QString)));
}

void Manager::login_successed(void){
    mainWindow = new MainWindow();
    mainWindow->show();
    if (login) {
        delete login;
    }
}

void Manager::f()
{
    char data_array[64] = {0};
    socket->read(data_array, sizeof(data_array));
    pdebug << data_array << endl;
    string data(data_array);
    KVP *p;
    string_to_kvp(data, p);
    if ("send" == p->key) {
        cout << p->sub->key << " " << p->sub->value << endl;
    } else if ("room" == p->key) {
        cout << p->sub->key << " " << p->sub->value << endl;
    } else if ("query" == p->key){
        if ("name" == p->sub->key) {
            if ("not exist" == p->sub->value) {
                emit send_message("name not exist");
                pdebug << "name not exist" << endl;
            } else {
                emit send_message("name exist");
                pdebug << "name exist" << endl;
            }
        } else if ("password" == p->sub->key) {
            if ("correct" == p->sub->value) {
                pdebug << "password correct" << endl;
                //mainWindow = new MainWindow();
                //mainWindow->show();
                //if (login) {
                //	delete login;
                //}
                login_successed();
            } else {
                pdebug << "password incorrect" << endl;
                emit send_message("password incorrect");
            }
        }
    } else if ("regist" == p->key) {
        if ("failed" == p->value) {
            pdebug << "regist failed" << endl;
        } else {
            pdebug << "regist success" << endl;
            login_successed();
        }
    } else {
        cout << "other" << endl;
    }
    if (p) {
        delete p;
    }
}
