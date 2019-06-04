#include "groupwindow.h"
#include "ui_groupwindow.h"

using namespace std;

GroupWindow::GroupWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWindow)
{
    ui->setupUi(this);

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

//请求群成员
void GroupWindow::query_member()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("query");
    writer.String("group member");
    writer.Key("id");
    writer.Uint(id);
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
}
