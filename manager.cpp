#include "manager.h"

#include "widget.h"

using namespace std;

Manager::Manager(QObject *parent) : QObject(parent)
{
    //建立连接
    socket = new QTcpSocket();
    socket->connectToHost("192.168.196.170", 8080);

    //连接 socket_readyRead to read
    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));

    //显示登录窗
    login = new Login();
    login->show();

    //manager 与 login 通信
    connect(this, SIGNAL(send_to_login(QString)), login, SLOT(recv_from_manager(QString)));
}

//登陆成功
void Manager::login_successed(void){
    mainWindow = new MainWindow();
    mainWindow->show();
    //发送信息给mainWindow
    connect(this, SIGNAL(send_to_mainWindow(unsigned int,QString)), mainWindow, SLOT(recv_from_manager(unsigned int,QString)));
    //双击mainWindow的联系人列表,创建一个ChatWindow
    connect(mainWindow, SIGNAL(double_clicked_on_contacts_list_view(QModelIndex)), this, SLOT(new_chatWindow(QModelIndex)));
    //请求消息队列
    ask_for_message();
    if (login) {
        delete login;
    }
}

void Manager::ask_for_message()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("message");
    writer.String("please");
    writer.EndObject();
    string data(sb.GetString());
    socket->write(data.c_str(), data.length());
    socket->flush();
}

//读取/解析/转发来自服务器的消息
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
        unsigned id = object["id"].GetUint();
        string data(object["data"].GetString());
        if (id_chatWindow.count(id)) {
            pdebug << string("to chatWindow") << endl;
            id_chatWindow[id]->recv_data(data);
        } else {
            pdebug << string("to mq") << endl;
            id_mq[id].push(data);
        }
        //pdebug << id << " " << data << endl;
        //明天解决 改成队列
        //bug-->若不打开聊天窗口则挂掉
        //id_chatWindow[id]->recv_data(data);
    //验证
    } else if (doc.HasMember("query") && doc["query"].IsObject()) {
        const rapidjson::Value &object = doc["query"];
        //验证名子
        if (object.HasMember("name") && object["name"].IsString()) {
            if (string("exist") == object["name"].GetString()) {
                pdebug << "name exist" << endl;
                emit send_to_login("name exist");
            } else if (string("not exist") == object["name"].GetString()) {
                pdebug << "name not exist" << endl;
                emit send_to_login("name not exist");
            }
        //验证密码
        } else if (object.HasMember("password") && object["password"].IsString()) {
            if (string("correct") == object["password"].GetString()) {
                pdebug << "password correct" << endl;
                //登录成功
                login_successed();
            } else if (string("incorrect") ==  object["password"].GetString()) {
                pdebug << "password incorrect" << endl;
                emit send_to_login("password incorrect");
            }
        }
    //联系人列表
    } else if (doc.HasMember("contacts list") && doc["contacts list"].IsArray()) {
        const rapidjson::Value &contacts_array = doc["contacts list"];
        for (rapidjson::SizeType i = 0; i < contacts_array.Size(); ++i) {
            //mainWindow->contacts_list.append(QString::fromStdString(contacts_array[i].GetString()));
            const rapidjson::Value &object = contacts_array[i];
            mainWindow->name_id[object["name"].GetString()] = object["id"].GetUint();
            mainWindow->contacts_list.append(QString::fromStdString(object["name"].GetString()));
        }
        mainWindow->contacts_list_model->setStringList(mainWindow->contacts_list);
    //群列表
    } else if (doc.HasMember("group list") && doc["group list"].IsArray()) {
        const rapidjson::Value &group_array = doc["group list"];
        for (rapidjson::SizeType i = 0; i < group_array.Size(); ++i) {
            mainWindow->group_list.append(QString::fromStdString(group_array[i].GetString()));
        }
        mainWindow->group_list_model->setStringList(mainWindow->group_list);
        //注册
    } else if (doc.HasMember("regist") && doc["regist"].IsString()) {
        if (string("failed") == doc["regist"].GetString()) {
            emit send_to_login("regist failed");
        } else {
            //注册成功
            login_successed();
        }
    }
    else {
        pdebug << "other" << endl;
    }
}

//新建聊天窗口
void Manager::new_chatWindow(QModelIndex index)
{
    unsigned int id = mainWindow->name_id[index.data().toString().toStdString()];
    //!!!有问题没解决 ChatWindow构造函数
    ChatWindow *chatWindow = new ChatWindow;
    connect(this, SIGNAL(chatWindow_show_historyMessage()), chatWindow, SLOT(showHistoryMessage()));
    chatWindow->contacts_id = id;
    //ChatWindow *chatWindow = new ChatWindow(id);
    connect(chatWindow, SIGNAL(close(uint)), this, SLOT(chatWindow_close(uint)));
    chatWindow->setAttribute(Qt::WA_DeleteOnClose);
    id_chatWindow[id] = chatWindow;
    chatWindow->show();
    emit chatWindow_show_historyMessage();
}

void Manager::chatWindow_close(unsigned int id)
{
    id_chatWindow.remove(id);
}
