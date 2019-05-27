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
    connect(this, SIGNAL(recv_data(QString)), mainWindow, SLOT(recv_data(QString)));
    if (login) {
        delete login;
    }
}

void Manager::f()
{
    char data[64] = {0};
    socket->read(data, sizeof(data));
    pdebug << data << endl;
    rapidjson::Document doc;
    if (doc.Parse(data).HasParseError()) {
        pdebug << "json parse error" << endl;
        return;
    }
    if (doc.HasMember("query") && doc["query"].IsObject()) {
        const rapidjson::Value &object = doc["query"];
        if (object.HasMember("name") && object["name"].IsString()) {
            if (string("exist") == object["name"].GetString()) {
                pdebug << "name exist" << endl;
                emit send_message("name exist");
            } else if (string("not exist") == object["name"].GetString()) {
                pdebug << "name not exist" << endl;
                emit send_message("name not exist");
            }
        } else if (object.HasMember("password") && object["password"].IsString()) {
            if (string("correct") == object["password"].GetString()) {
                pdebug << "password correct" << endl;
                //emit send_message("password correct");
                login_successed();
            } else if (string("incorrect") ==  object["password"].GetString()) {
                pdebug << "password incorrect" << endl;
                emit send_message("password incorrect");
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
        pdebug << name << " " << data << endl;
        emit recv_data(QString::fromStdString(data));
    }
    else {
        pdebug << "other" << endl;
    }

    //KVP *p;
    //string_to_kvp(data, p);
    //if ("send" == p->key) {
    //    cout << p->sub->key << " " << p->sub->value << endl;
    //} else if ("room" == p->key) {
    //    cout << p->sub->key << " " << p->sub->value << endl;
    //} else if ("contacts" == p->key) {
    //    stringstream ss(p->value);
    //    string contact;
    //    QStringList contact_list;
    //    while(ss >> contact){
    //        contact_list.append(QString::fromStdString(contact));
    //    }
    //    mainWindow->contact_list = contact_list;
    //} else if ("query" == p->key){
    //    if ("name" == p->sub->key) {
    //        if ("not exist" == p->sub->value) {
    //            emit send_message("name not exist");
    //            pdebug << "name not exist" << endl;
    //        } else {
    //            emit send_message("name exist");
    //            pdebug << "name exist" << endl;
    //        }
    //    } else if ("password" == p->sub->key) {
    //        if ("correct" == p->sub->value) {
    //            pdebug << "password correct" << endl;
    //            //mainWindow = new MainWindow();
    //            //mainWindow->show();
    //            //if (login) {
    //            //	delete login;
    //            //}
    //            login_successed();
    //        } else {
    //            pdebug << "password incorrect" << endl;
    //            emit send_message("password incorrect");
    //        }
    //    }
    //} else if ("regist" == p->key) {
    //    if ("failed" == p->value) {
    //        pdebug << "regist failed" << endl;
    //    } else {
    //        pdebug << "regist success" << endl;
    //        login_successed();
    //    }
    //} else {
    //    cout << "other" << endl;
    //}
    //if (p) {
    //    delete p;
    //}
}
