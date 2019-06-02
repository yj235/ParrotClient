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

    //迷... new chatwindow 后id_mq为空
    //改用manager 发送信号
    while (!id_mq[contacts_id].empty()) {
        qDebug() << "inside";
        ui->textBrowser->append(QString::fromStdString(id_mq[contacts_id].front().time));
        ui->textBrowser->append(QString::fromStdString(id_mq[contacts_id].front().message));
        id_mq[contacts_id].pop();
    }
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
    //socket->write(data.c_str(), data.length());
    socket_write(data);
    ui->textBrowser->append(time_str);
    ui->textBrowser->append(ui->textEdit->toPlainText() + "\n");
    ui->textEdit->clear();
}

void ChatWindow::showHistoryMessage()
{
    while (!id_mq[contacts_id].empty()) {
        ui->textBrowser->append(QString::fromStdString(id_mq[contacts_id].front().time));
        ui->textBrowser->append(QString::fromStdString(id_mq[contacts_id].front().message) + "\n");
        id_mq[contacts_id].pop();
    }
}
