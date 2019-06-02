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
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("query");
    writer.String("contacts");
    writer.EndObject();
    string data = sb.GetString();
    //socket->write(data.c_str(), data.length());
    socket_write(data);
    socket->flush();

    //延迟发送 有bug 两个json连一起了
    //delay();

    //请求群列表
    //rapidjson::StringBuffer sb2;
    //rapidjson::Writer<rapidjson::StringBuffer> writer2(sb2);
    //writer2.StartObject();
    //writer2.Key("query");
    //writer2.String("group");
    //writer2.EndObject();
    //string data2 = sb2.GetString();
    //socket->write(data2.c_str(), data2.length());

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

    //发送快捷键
    ui->pushButton_send->setShortcut(tr("ctrl+return"));

    //退出快捷键
    shortcut_close = new QShortcut(QKeySequence(tr("ctrl+q")), this);
    connect(shortcut_close, SIGNAL(activated()), this,SLOT(close()));

    //连接 单击联系人列表信号 文本编辑器焦点槽
    //connect(ui->listView_contacts, SIGNAL(clicked(QModelIndex)), this, SLOT(contacts_clicked(QModelIndex)));

    //连接 双击联系人列表信号 转发至manager的信号
    connect(ui->listView_contacts, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(double_clicked_on_contacts_list_view(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//关闭事件 断开连接
void MainWindow::closeEvent(QCloseEvent *){
    socket->disconnectFromHost();
}

//发送信息
void MainWindow::on_pushButton_send_clicked()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("send");
    writer.StartObject();
    writer.Key("name");
    writer.String(ui->listView_contacts->currentIndex().data().toString().toStdString().c_str());
    writer.Key("data");
    writer.String(ui->textEdit->toPlainText().toStdString().c_str());
    ui->textEdit->clear();
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    //socket->write(data.c_str(), data.length());
    socket_write(data);
}

//点击联系人后焦点到文本编辑器
//void MainWindow::contacts_clicked(QModelIndex)
//{
//    ui->textEdit->setFocus();
//}

//显示从manager接收到的信息
void MainWindow::recv_from_manager(unsigned int id, QString data)
{
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy/M/d h:m:s");
    ui->textBrowser->append(QString::number(id) + " " + time_str);
    ui->textBrowser->append(data + "\n");
}

//请求群列表按钮
//创建原因 请求联系人列表和请求群列表的json发送是连在一起的 如:{"query":"contacts"}{"query":"group"}
//需改进
void MainWindow::on_pushButton_getGroupList_clicked()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("query");
    writer.String("group");
    writer.EndObject();
    string data = sb.GetString();
    //socket->write(data.c_str(), data.length());
    socket_write(data);
}

void MainWindow::on_pushButton_clicked()
{
    emit ask_for_history_message_button();
}
