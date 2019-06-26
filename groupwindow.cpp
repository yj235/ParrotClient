#include "groupwindow.h"
#include "ui_groupwindow.h"

using namespace std;

GroupWindow::GroupWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWindow)
{
    ui->setupUi(this);

    //退出快捷键ctrl+q
    shortCut_close = new QShortcut(QKeySequence(tr("ctrl+q")), this);
    connect(shortCut_close, SIGNAL(activated()), this,SLOT(close()));

    //发送快捷键
    ui->pushButton_send->setShortcut(tr("ctrl+return"));

    //请求群成员
    //query_member();
    model = new QStringListModel;
    model->setStringList(member_list);
    ui->listView->setModel(model);
}

GroupWindow::~GroupWindow()
{
    delete ui;
}

void GroupWindow::init()
{
    query_member();
}

void GroupWindow::show_history_message()
{
    while (!group_mq[group_id].empty()) {
        ui->textBrowser->append(QString::fromStdString(to_string(group_mq[group_id].front().id)));
        ui->textBrowser->append(QString::fromStdString(group_mq[group_id].front().time));
        ui->textBrowser->append(QString::fromStdString(group_mq[group_id].front().message) + "\n");
        group_mq[group_id].pop();
    }
}

void GroupWindow::open()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("group window open");
    writer.StartObject();
    writer.Key("id");
    writer.Uint(ID);
    writer.Key("group_id");
    writer.Uint(group_id);
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
}

void GroupWindow::recv_data(string name, string time, string message)
{
    ui->textBrowser->append(QString::fromStdString(name + " " + time));
    ui->textBrowser->append(QString::fromStdString(message + "\n"));
}

void GroupWindow::setID(unsigned int group_id)
{
    ui->label_groupID->setText("group ID" + QString::number(group_id));
}

void GroupWindow::closeEvent(QCloseEvent *event)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("group window close");
    writer.StartObject();
    writer.Key("id");
    writer.Uint(ID);
    writer.Key("group_id");
    writer.Uint(group_id);
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
    emit close(group_id);
}

//请求群成员
void GroupWindow::query_member()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("query");
    writer.String("group member");
    writer.Key("id");
    writer.Uint(group_id);
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
}

//发送群消息
void GroupWindow::on_pushButton_send_clicked()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("send group");
    writer.StartObject();
    writer.Key("group id");
    writer.Uint(group_id);
    writer.Key("user id");
    writer.Uint(group_id);
    writer.Key("time");
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy-M-d h:m:s");
    writer.String(time_str.toStdString().c_str());
    writer.Key("message");
    writer.String(ui->textEdit->toPlainText().toStdString().c_str());
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
    ui->textEdit->clear();
}
