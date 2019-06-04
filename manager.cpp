#include "manager.h"

#include "widget.h"

#define IP "192.168.196.171"
#define MAXSIZE 1024

using namespace std;

Manager::Manager(QObject *parent) : QObject(parent)
{
    //建立连接
    socket = new QTcpSocket();
    socket->connectToHost(IP, 8080);

    //连接 socket_readyRead to read
    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));

    //显示登录窗
    login = new Login();
    login->show();

    //manager 与 login 通信
    connect(this, SIGNAL(send_to_login(QString)), login, SLOT(recv_from_manager(QString)));
}

//读取来自服务器的消息
void Manager::read()
{
    unsigned int len = 0;
    while (socket->read((char*)&len, sizeof(len)) > 0) {
        char data[MAXSIZE] = {0};
        socket->read(data, len);
        pdebug << data << endl;
        my_parse(data);
    }
}

//解析/转发来自服务器的消息
void Manager::my_parse(char *data)
{
    rapidjson::Document doc;
    if (doc.Parse(data).HasParseError()) {
        pdebug << "json parse error" << endl;
        return;
    }
    //接收消息
    if (doc.HasMember("send") && doc["send"].IsObject()) {
        const rapidjson::Value &object = doc["send"];
        unsigned id = object["id"].GetUint();
        string time(object["time"].GetString());
        string message(object["message"].GetString());
        if (id_chatWindow.count(id)) {
            pdebug << string("to chatWindow") << endl;
            id_chatWindow[id]->recv_data(time, message);
        } else {
            pdebug << string("to mq") << endl;
            id_mq[id].push({time, message});
        }
        //验证
    } else if (doc.HasMember("query") && doc["query"].IsObject()) {
        const rapidjson::Value &object = doc["query"];
        //验证姓名
        if (object.HasMember("name") && object["name"].IsString()) {
            if (string("exist") == object["name"].GetString()) {
                emit send_to_login("name exist");
            } else if (string("not exist") == object["name"].GetString()) {
                pdebug << "name not exist" << endl;
                emit send_to_login("name not exist");
            }
            //验证密码
        } else if (object.HasMember("password") && object["password"].IsString()) {
            if (string("correct") == object["password"].GetString()) {
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
            const rapidjson::Value &object = contacts_array[i];
            mainWindow->contacts_vector_id.push_back(object["id"].GetUint());
            mainWindow->contacts_name_id[object["name"].GetString()] = object["id"].GetUint();
            mainWindow->contacts_list.append(QString::fromStdString(object["name"].GetString()));
        }
        mainWindow->contacts_list_model->setStringList(mainWindow->contacts_list);
        //群列表
    } else if (doc.HasMember("group list") && doc["group list"].IsArray()) {
        const rapidjson::Value &group_array = doc["group list"];
        for (rapidjson::SizeType i = 0; i < group_array.Size(); ++i) {
            const rapidjson::Value &object = group_array[i];
            mainWindow->group_vector_id.push_back(object["id"].GetUint());
            mainWindow->group_name_id[object["name"].GetString()] = object["id"].GetUint();
            //mainWindow->group_list.append(QString::fromStdString(group_array[i].GetString()));
            mainWindow->group_list.append(QString::fromStdString(object["name"].GetString()));
        }
        mainWindow->group_list_model->setStringList(mainWindow->group_list);
        //注册
    } else if (doc.HasMember("regist") && doc["regist"].IsString()) {
        if (string("failed") == doc["regist"].GetString()) {
            emit send_to_login("regist failed");
        } else {
            //注册成功后登录
            login_successed();
        }
        //查找
    } else if (doc.HasMember("search") && doc["search"].IsString()) {
        //查找用户
        if (doc["search"].GetString() == string("contacts")) {
            if (doc["name"].GetString() == string("not exist")) {
                QMessageBox msgBox;
                msgBox.setText("not such user!");
                msgBox.exec();
            } else {
                //mainWindow->name_id[doc["name"].GetString()] = mainWindow->contacts_search_id;
                mainWindow->contacts_name_id[doc["name"].GetString()] = mainWindow->contacts_search_id;
                mainWindow->contacts_list.append(QString::fromStdString(doc["name"].GetString()));
                mainWindow->contacts_list_model->setStringList(mainWindow->contacts_list);
            }
        //查找群
        } else if (doc["search"].GetString() == string("group")){
            if (doc["name"].GetString() == string("not exist")) {
                QMessageBox msgBox;
                msgBox.setText("not such user!");
                msgBox.exec();
            } else {
                mainWindow->group_name_id[doc["name"].GetString()] = mainWindow->group_search_id;
                mainWindow->group_list.append(QString::fromStdString(doc["name"].GetString()));
                mainWindow->group_list_model->setStringList(mainWindow->group_list);
            }
        }
        //群成员列表
    } else if (doc.HasMember("member list") && doc["member list"].IsObject()) {
        const rapidjson::Value &object = doc["member list"];
        unsigned int group_id = object["group id"].GetUint();
        const rapidjson::Value &member_array = object["list"];
        for (rapidjson::SizeType i = 0; i < member_array.Size(); ++i) {
            const rapidjson::Value &object2 = member_array[i];
            id_groupWindow[group_id]->member_list.append(QString::fromStdString(object2["name"].GetString()));
        }
        id_groupWindow[group_id]->model->setStringList(id_groupWindow[group_id]->member_list);
    }
    else {
        pdebug << "other" << endl;
    }
}

//登陆成功
void Manager::login_successed(void){
    mainWindow = new MainWindow();
    mainWindow->show();
    //双击mainWindow的联系人列表,创建一个ChatWindow
    connect(mainWindow, SIGNAL(double_clicked_on_contacts_list_view(QModelIndex)), this, SLOT(new_chatWindow(QModelIndex)));
    //双击mainWindow的群列表,创建一个GroupWindow
    connect(mainWindow, SIGNAL(double_clicked_on_group_list_view(QModelIndex)), this, SLOT(new_groupWindow(QModelIndex)));
    //请求历史消息
    ask_for_message();

    if (login) {
        delete login;
    }
}

//请求历史消息
void Manager::ask_for_message()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("message");
    writer.String("please");
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
}

//新建聊天窗口
void Manager::new_chatWindow(QModelIndex index)
{
    unsigned int id = mainWindow->contacts_name_id[index.data().toString().toStdString()];
    if (id_chatWindow.count(id)) {
        //若已打开 置顶
        id_chatWindow[id]->raise();
        return;
    }

    //!!!有问题没解决 ChatWindow构造函数
    ChatWindow *chatWindow = new ChatWindow;
    connect(this, SIGNAL(chatWindow_show_historyMessage()), chatWindow, SLOT(showHistoryMessage()));
    chatWindow->contacts_id = id;
    //ChatWindow *chatWindow = new ChatWindow(id);

    //chatWindow关闭后从id_chatWindow中移除
    connect(chatWindow, SIGNAL(close(uint)), this, SLOT(chatWindow_close(uint)));
    chatWindow->setAttribute(Qt::WA_DeleteOnClose);
    id_chatWindow[id] = chatWindow;
    chatWindow->show();
    emit chatWindow_show_historyMessage();
}

//新建群窗口
void Manager::new_groupWindow(QModelIndex index)
{
    unsigned int id = mainWindow->group_name_id[index.data().toString().toStdString()];
    if (id_groupWindow.count(id)) {
        id_groupWindow[id]->raise();
        return;
    }
    GroupWindow *groupWindow = new GroupWindow;
    groupWindow->id = id;
    groupWindow->init();
    id_groupWindow[id] = groupWindow;
    groupWindow->show();
    ////unsigned int id = mainWindow->name_id[index.data().toString().toStdString()];
    //unsigned int id = mainWindow->contacts_name_id[index.data().toString().toStdString()];
    //if (id_chatWindow.count(id)) {
    //    //若已打开 置顶
    //    id_chatWindow[id]->raise();
    //    return;
    //}

    ////!!!有问题没解决 ChatWindow构造函数
    //ChatWindow *chatWindow = new ChatWindow;
    //connect(this, SIGNAL(chatWindow_show_historyMessage()), chatWindow, SLOT(showHistoryMessage()));
    //chatWindow->contacts_id = id;
    ////ChatWindow *chatWindow = new ChatWindow(id);

    ////chatWindow关闭后从id_chatWindow中移除
    //connect(chatWindow, SIGNAL(close(uint)), this, SLOT(chatWindow_close(uint)));
    //chatWindow->setAttribute(Qt::WA_DeleteOnClose);
    //id_chatWindow[id] = chatWindow;
    //chatWindow->show();
    //emit chatWindow_show_historyMessage();
}

void Manager::chatWindow_close(unsigned int id)
{
    id_chatWindow.remove(id);
}
