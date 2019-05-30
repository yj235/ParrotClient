#include "chatwindow.h"
#include "ui_chatwindow.h"

using namespace std;

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    //退出快捷键ctrl+enter
    shortCut_close = new QShortcut(QKeySequence(tr("ctrl+q")), this);
    connect(shortCut_close, SIGNAL(activated()), this,SLOT(close()));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::recv_data(string data)
{
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy/M/d h:m:s");
    ui->textBrowser->append(time_str);
    ui->textBrowser->append(QString::fromStdString(data) + "\n");
}

void ChatWindow::on_pushButton_clicked()
{
    qDebug() << "________";
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
