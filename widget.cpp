#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    socket = new QTcpSocket();
    socket->connectToHost("192.168.196.168", 8080);

    connect(socket, SIGNAL(readyRead()), this, SLOT(f()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::f(void){
    char message[64] = {0};
    socket->read(message, sizeof(message));
    ui->label->setText(message);
}

void Widget::on_pushButton_clicked()
{
    socket->write(ui->lineEdit->text().toUtf8().data());
}
