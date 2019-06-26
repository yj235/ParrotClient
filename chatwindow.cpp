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

    ui->progressBar->setValue(0);
    //迷... new chatwindow 后contacts_mq为空
    //改用manager 发送信号
    //new chatWiindow 后 后续工作未完成
    //服务器端完成
    //while (!contacts_mq[contacts_id].empty()) {
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

//关闭并发送信号给manager
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

//发送消息
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
    QString time_str = time.toString("yyyy-M-d h:m:s");
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

//显示历史消息
//好像弃用了?
void ChatWindow::showHistoryMessage()
{
    while (!contacts_mq[contacts_id].empty()) {
        ui->textBrowser->append(QString::fromStdString(contacts_mq[contacts_id].front().time));
        ui->textBrowser->append(QString::fromStdString(contacts_mq[contacts_id].front().message) + "\n");
        contacts_mq[contacts_id].pop();
    }
}

//发送给服务器打开消息
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

//传文件
void ChatWindow::on_pushButton_fileTransfer_clicked()
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("file transfer to");
    writer.Uint(contacts_id);
    writer.EndObject();
    string data(sb.GetString());
    socket_write(data);

    startServer();
}

//建立服务器
void ChatWindow::startServer()
{
    pdebug << "start server..." << endl;
    cancel = false;
    if (server == NULL) {
        pdebug << "server is NULL now create new server" << endl;
        server = new QTcpServer;
    }
    server->listen(QHostAddress::Any, 12346);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

//新链接
void ChatWindow::newConnection()
{
    pdebug << "new connection..." << endl;
    socket = server->nextPendingConnection();
    server->close();
    if (server) {
        server->deleteLater();
        server = NULL;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(continueTransfer(qint64)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    /*QString*/ fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty()) {
        socket->disconnectFromHost();
        return;
    }
    int index = fileName.lastIndexOf("/");
    QString fileName_withoutPath = fileName.right(fileName.length() - index - 1);
    //int nameLength = fileName_withoutPath.length();
    file = new QFile(fileName);
    file->open(QIODevice::ReadOnly);
    restBytes = file->size();
    ui->progressBar->setValue(0);
    if (restBytes > INT_MAX) {
        ui->progressBar->setMaximum(100);
        div = restBytes / 100;
    } else {
        ui->progressBar->setMaximum(restBytes);
    }
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_8);
    //fileName_withoutPath.length()用qint64?(需转换)
    stream << /*fileName_withoutPath.length() <<*/ fileName_withoutPath << restBytes;
    socket->write(data);
}

void ChatWindow::continueTransfer(qint64)
{
    if (cancel) {
        return;
    }
    if (restBytes) {
        //pdebug << restBytes << endl;
        if (file && socket->state() == QAbstractSocket::ConnectedState) {
            restBytes -= socket->write(file->read(restBytes < BLOCK_SIZE ? restBytes : BLOCK_SIZE));
            ui->progressBar->setValue(ui->progressBar->maximum() - restBytes / div);
        }
        //pdebug << restBytes << endl;
    } else {
        pdebug << "send finish" << endl;
        ui->progressBar->setValue(ui->progressBar->maximum());
        fileName = "";
        restBytes = 0;
        file->close();
        if (file != NULL) {
            delete file;
            file = NULL;
        }
        socket->disconnectFromHost();
        if (server != NULL) {
            server->close();
            server->deleteLater();
            server = NULL;
        }
        div = 1.0;
    }
}

//void ChatWindow::startFileTransfer(unsigned int ip)
void ChatWindow::startFileTransfer(string ip)
{
    cancel = false;
    pdebug << "start file transfer..." << endl;
    socket = new QTcpSocket;
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    //socket->connectToHost(QHostAddress(ip), FILE_TRANSFER_PORT);
    //socket->connectToHost("192.168.0.101", 12346);
    socket->connectToHost(ip.c_str(), 12346);
}

void ChatWindow::readyRead()
{
    if (cancel) {
        return;
    }
    pdebug << "ready read..." << endl;
    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_4_8);
    if (flag) {
        flag = false;
        //int fileNameLength = 0;
        stream >> /*fileNameLength >>*/ fileName >> restBytes;
        ui->progressBar->setValue(0);
        if (restBytes > INT_MAX) {
            ui->progressBar->setMaximum(100);
            div = (double)restBytes / 100;
        } else {
            ui->progressBar->setMaximum(restBytes);
        }
        //pdebug << fileNameLength << endl;
        pdebug << fileName.toStdString() << endl;
        pdebug << restBytes << endl;
    }

    if (file == NULL){
        pdebug << "open file" << endl;
        file = new QFile(fileName);
        file->open(QIODevice::WriteOnly);
    }
    restBytes -= file->write(socket->readAll());
    pdebug << restBytes << endl;
    ui->progressBar->setValue(ui->progressBar->maximum() - restBytes / div);
    if (!restBytes) {
        pdebug << "recv finish" << endl;
        ui->progressBar->setValue(ui->progressBar->maximum());
        //fileNameLength = 0;
        fileName = "";
        restBytes = 0;
        file->close();
        if (file != NULL) {
            delete file;
            file = NULL;
        }
        socket->disconnectFromHost();
        //socket->close();
        delete socket;
        flag = true;
        div = 1.0;
    }
}

void ChatWindow::on_pushButton_cancel_clicked()
{
    restBytes = 0;
    cancel = true;
    pdebug << "cancel" << endl;
    if (NULL != file) {
        file->close();
    }
    if (NULL == server) {
        file->remove();
    }
    if (NULL != file) {
        delete file;
        file = NULL;
    }
    //socket->disconnectFromHost();
    if (NULL != socket) {
        socket->deleteLater();
        socket = NULL;
        //delete socket;
        //socket = NULL;
    }
    if (NULL != server) {
        server->close();
        delete server;
        server = NULL;
    }
    //fileNameLength = 0;
    fileName = "";
    div = 1.0;
    flag = true;
    ui->progressBar->setValue(0);
}

void ChatWindow::disconnected()
{
    pdebug << "disconnected" << endl;
    socket->deleteLater();

    restBytes = 0;
    //cancel = true;
    if (NULL != file) {
        file->close();
    }
    if (NULL == server) {
        file->remove();
    }
    if (NULL != file) {
        delete file;
        file = NULL;
    }
    //socket->disconnectFromHost();
    if (NULL != socket) {
        socket->deleteLater();
        socket = NULL;
        //delete socket;
        //socket = NULL;
    }
    if (NULL != server) {
        server->close();
        delete server;
        server = NULL;
    }
    //fileNameLength = 0;
    fileName = "";
    div = 1.0;
    flag = true;
    ui->progressBar->setValue(0);

    //if (!cancel) {
    //    cancel = true;
    //    if (restBytes) {
    //        on_pushButton_cancel_clicked();
    //    }
    //}
}

void ChatWindow::clean()
{
    if (server) {
        delete server;
        server = NULL;
    }
}
