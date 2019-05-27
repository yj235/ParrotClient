#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pdebug.h"
#include <qdebug.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listView->setUpdatesEnabled(true);

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("query");
    writer.String("contacts");
    writer.EndObject();
    string data = sb.GetString();
    socket->write(data.c_str(), data.length());

    contacts_list_model = new QStringListModel;
    contacts_list_model->setStringList(contacts_list);
    ui->listView->setModel(contacts_list_model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::contacts_list_select(QModelIndex index)
//{
//    qDebug() << index.data().toString();
//}

void MainWindow::closeEvent(QCloseEvent *){
    socket->disconnectFromHost();
}

void MainWindow::on_pushButton_send_clicked()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("send");
    writer.StartObject();
    writer.Key("name");
    writer.String(ui->listView->currentIndex().data().toString().toStdString().c_str());
    writer.Key("data");
    writer.String(ui->lineEdit->text().toStdString().c_str());
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    socket->write(data.c_str(), data.length());
}

void MainWindow::recv_from_manager(QString name, QString data)
{
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy/M/d h:m:s");
    ui->textBrowser->append(name + " " + time_str);
    ui->textBrowser->append(data + "\n");
}
