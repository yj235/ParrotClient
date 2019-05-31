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
        ui->textBrowser->append(QString::fromStdString(id_mq[contacts_id].front()));
        id_mq[contacts_id].pop();
    }
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::recv_data(string data)
{
    //时间要改
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy/M/d h:m:s");
    ui->textBrowser->append(time_str);
    ui->textBrowser->append(QString::fromStdString(data) + "\n");
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    emit close(this->contacts_id);

}

void ChatWindow::on_pushButton_send_clicked()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("send");
    writer.StartObject();
    writer.Key("id");
    writer.Uint(contacts_id);
    writer.Key("data");
    writer.String(ui->textEdit->toPlainText().toStdString().c_str());
    ui->textEdit->clear();
    writer.EndObject();
    writer.EndObject();
    string data(sb.GetString());
    socket->write(data.c_str(), data.length());
}

void ChatWindow::showHistoryMessage()
{
    while (!id_mq[contacts_id].empty()) {
        ui->textBrowser->append(QString::fromStdString(id_mq[contacts_id].front()));
        id_mq[contacts_id].pop();
    }
}
