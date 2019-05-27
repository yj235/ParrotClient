#include "manager.h"

using namespace std;

Manager::Manager(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
    socket->connectToHost("192.168.196.169", 8080);

    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));

    login = new Login();
    login->show();

    connect(this, SIGNAL(send_to_login(QString)), login, SLOT(recv_from_manager(QString)));
}

void Manager::login_successed(void){
    mainWindow = new MainWindow();
    mainWindow->show();
    mainWindow->setWindowTitle();
    connect(this, SIGNAL(send_to_mainWindow(QString,QString)), mainWindow, SLOT(recv_from_manager(QString,QString)));
    if (login) {
        delete login;
    }
}

void Manager::read()
{
    char data[64] = {0};
    socket->read(data, sizeof(data));
    pdebug << data << endl;
    rapidjson::Document doc;
    if (doc.Parse(data).HasParseError()) {
        pdebug << "json parse error" << endl;
        return;
    }
        if (doc.HasMember("send") && doc["send"].IsObject()) {
        const rapidjson::Value &object = doc["send"];
        string name(object["name"].GetString());
        string data(object["data"].GetString());
        emit send_to_mainWindow(QString::fromStdString(name), QString::fromStdString(data));
    }
    else if (doc.HasMember("query") && doc["query"].IsObject()) {
        const rapidjson::Value &object = doc["query"];
        if (object.HasMember("name") && object["name"].IsString()) {
            if (string("exist") == object["name"].GetString()) {
                pdebug << "name exist" << endl;
                emit send_to_login("name exist");
            } else if (string("not exist") == object["name"].GetString()) {
                pdebug << "name not exist" << endl;
                emit send_to_login("name not exist");
            }
        } else if (object.HasMember("password") && object["password"].IsString()) {
            if (string("correct") == object["password"].GetString()) {
                pdebug << "password correct" << endl;
                login_successed();
            } else if (string("incorrect") ==  object["password"].GetString()) {
                pdebug << "password incorrect" << endl;
                emit send_to_login("password incorrect");
            }
        }
    } else if (doc.HasMember("contacts list") && doc["contacts list"].IsArray()) {
        const rapidjson::Value &contacts_array = doc["contacts list"];
        for (rapidjson::SizeType i = 0; i < contacts_array.Size(); ++i) {
            mainWindow->contacts_list.append(QString::fromStdString(contacts_array[i].GetString()));
        }
        mainWindow->contacts_list_model->setStringList(mainWindow->contacts_list);
    } else if (doc.HasMember("send") && doc["send"].IsObject()) {
        const rapidjson::Value &object = doc["send"];
        string name(object["name"].GetString());
        string data(object["data"].GetString());
        emit send_to_mainWindow(QString::fromStdString(name), QString::fromStdString(data));
    } else if (doc.HasMember("regist") && doc["regist"].IsString()) {
        if (string("failed") == doc["regist"].GetString()) {
            emit send_to_login("regist failed");
        } else {
            login_successed();
        }
    }
    else {
        pdebug << "other" << endl;
    }
}
