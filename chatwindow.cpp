#include "chatwindow.h"
#include "ui_chatwindow.h"

#include <QDebug>

using namespace std;

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    //退出快捷键ctrl+q
    shortCut_close = new QShortcut(QKeySequence(tr("ctrl+q")), this);
    connect(shortCut_close, SIGNAL(activated()), this,SLOT(close()));

    //发送快捷键
    ui->pushButton_send->setShortcut(tr("ctrl+return"));

    //迷... new chatwindow 后contacts_mq为空
    //改用manager 发送信号
    //new chatWiindow 后 后续工作未完成
    //while (!contacts_mq[contacts_id].empty()) {
    //    qDebug() << "inside";
    //    ui->textBrowser->append(QString::fromStdString(contacts_mq[contacts_id].front().time));
    //    ui->textBrowser->append(QString::fromStdString(contacts_mq[contacts_id].front().message));
    //    contacts_mq[contacts_id].pop();
    //}
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::recv_data(string time, string message)
{
    ui->textBrowser->append(QString::fromStdString(time));
    ui->textBrowser->append(QString::fromStdString(message) + "\n");
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("contacts window close");
    writer.StartObject();
    writer.Key("id");
    writer.Uint(ID);
    writer.Key("contacts_id");
    writer.Uint(contacts_id);
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
    emit close(this->contacts_id);
}

void ChatWindow::on_pushButton_send_clicked()
{
    if (ui->textEdit->toPlainText().isEmpty()) {
        return;
    }
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("send");
    writer.StartObject();
    writer.Key("id");
    writer.Uint(contacts_id);
    writer.Key("time");
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy/M/d h:m:s");
    writer.String(time_str.toStdString().c_str());
    writer.Key("message");
    writer.String(ui->textEdit->toPlainText().toStdString().c_str());
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
    ui->textBrowser->append(time_str);
    ui->textBrowser->append(ui->textEdit->toPlainText() + "\n");
    ui->textEdit->clear();
}

void ChatWindow::showHistoryMessage()
{
    while (!contacts_mq[contacts_id].empty()) {
        ui->textBrowser->append(QString::fromStdString(contacts_mq[contacts_id].front().time));
        ui->textBrowser->append(QString::fromStdString(contacts_mq[contacts_id].front().message) + "\n");
        contacts_mq[contacts_id].pop();
    }
}

void ChatWindow::open()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("contacts window open");
    writer.StartObject();
    writer.Key("id");
    writer.Uint(ID);
    writer.Key("contacts_id");
    writer.Uint(contacts_id);
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);
}

void ChatWindow::on_pushButton_clicked()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("file transfer to");
    writer.Uint(contacts_id);
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);

    QString fileName = QFileDialog::getOpenFileName(this);
    qDebug() << fileName;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    int index = fileName.lastIndexOf("/");
    QString fileName_shorter = fileName.right(fileName.length() - index - 1);
    QByteArray content = file.readAll();
    QByteArray header;
    QDataStream stream(&header, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_8);
}
