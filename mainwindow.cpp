#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pdebug.h"
#include <qdebug.h>

using namespace std;

//延迟函数
//暂没用
void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置联系人列表只读
    ui->listView_contacts->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置群列表只读
    ui->listView_group->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置联系人列表刷新
    ui->listView_contacts->setUpdatesEnabled(true);
    //设置群列表刷新
    ui->listView_group->setUpdatesEnabled(true);

    //请求联系人列表
    query_contacts_list();

    //请求群列表
    query_group_list();

    //设置联系人model/view
    contacts_list_model = new QStringListModel;
    contacts_list_model->setStringList(contacts_list);
    ui->listView_contacts->setModel(contacts_list_model);

    //设置群model/view
    group_list_model = new QStringListModel;
    group_list_model->setStringList(group_list);
    ui->listView_group->setModel(group_list_model);

    //联系人列表只读
    ui->listView_contacts->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //退出快捷键
    shortcut_close = new QShortcut(QKeySequence(tr("ctrl+q")), this);
    connect(shortcut_close, SIGNAL(activated()), this,SLOT(close()));

    //转发信号 双击联系人列表信号 转发至manager信号 由manager创建聊天窗口
    connect(ui->listView_contacts, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(double_clicked_on_contacts_list_view(QModelIndex)));
    //转发信号 双击群列表信号 转发至manager信号 由manager创建群窗口
    connect(ui->listView_group, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(double_clicked_on_group_list_view(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setID(unsigned int id)
{
    ui->label_myID->setText("my id:" + QString::number(id));
}

//请求联系人列表
void MainWindow::query_contacts_list()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("query");
    writer.String("contacts");
    writer.EndObject();
    string query_contacts_json = sb.GetString();
    socket_write(query_contacts_json);
}

//请求群列表
void MainWindow::query_group_list()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("query");
    writer.String("group");
    writer.EndObject();
    string query_group_json = sb.GetString();
    socket_write(query_group_json);
}

//关闭事件 断开连接
void MainWindow::closeEvent(QCloseEvent *){
    socket->disconnectFromHost();
}

//搜索用户
void MainWindow::on_pushButton_searchContacts_clicked()
{
    contacts_search_id = ui->lineEdit_searchContacts->text().toUInt();
    //if (std::find(vector_id.begin(), vector_id.end(), contacts_search_id) != vector_id.end()) {
    if (std::find(contacts_vector_id.begin(), contacts_vector_id.end(), contacts_search_id) != contacts_vector_id.end()) {
        QMessageBox msgBox;
        msgBox.setText("already exist");
        msgBox.exec();
        return;
    }
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("search");
    writer.String("contacts");
    writer.Key("id");
    writer.String(ui->lineEdit_searchContacts->text().toStdString().c_str());
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
}

//搜索群
void MainWindow::on_pushButton_searchGroup_clicked()
{
    group_search_id = ui->lineEdit_searchGroup->text().toUInt();
    if (std::find(group_vector_id.begin(), group_vector_id.end(), group_search_id) != group_vector_id.end()) {
        QMessageBox msgBox;
        msgBox.setText("already exist");
        msgBox.exec();
        return;
    }
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("search");
    writer.String("group");
    writer.Key("id");
    writer.Uint(ID);
    writer.Key("group id");
    writer.Uint(group_search_id);
    //writer.String(ui->lineEdit_searchContacts->text().toStdString().c_str());
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
}

void MainWindow::on_pushButton_createGroup_clicked()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("create group");
    writer.StartObject();
    writer.Key("id");
    writer.Uint(ID);
    writer.Key("name");
    writer.String(ui->lineEdit_groupName->text().toStdString().c_str());
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
}
