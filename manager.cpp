#include "manager.h"

using namespace std;

Manager::Manager(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
    socket->connectToHost("192.168.196.169", 8080);

    connect(socket, SIGNAL(readyRead()), this, SLOT(f()));

    login = new Login();
    login->show();
}

void Manager::f()
{
    char data_array[64] = {0};
    socket->read(data_array, sizeof(data_array));
    string data(data_array);
    KVP *p;
    string_to_kvp(data, p);
    if ("send" == p->key) {
        cout << p->sub->key << " " << p->sub->value << endl;
    } else if ("room" == p->key) {
        cout << p->sub->key << " " << p->sub->value << endl;
    } else if ("query" == p->key){
        if ("name" == p->sub->key) {
            if ("exist" == p->sub->value) {
                pdebug << "name exist" << endl;
            } else {
                pdebug << "name not exist" << endl;
            }
        } else if ("password" == p->sub->key) {
            if ("correct" == p->sub->value) {
                pdebug << "password correct" << endl;
                mainWindow = new MainWindow();
                mainWindow->show();
                if (login) {
                    delete login;
                }
            } else {
                pdebug << "password incorrect" << endl;
            }
        }
    } else {
        cout << "other" << endl;
    }
    if (p) {
        delete p;
    }
}
