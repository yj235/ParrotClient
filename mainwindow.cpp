#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listWidget_contacts->addItem(new QListWidgetItem("nb"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *){
    socket->disconnectFromHost();
}

void MainWindow::on_pushButton_send_clicked()
{
    KVP *p1 = new KVP("send");
    KVP *p2 = new KVP("nb", ui->lineEdit->text().toStdString());
    p1->sub = p2;
    string data;
    format(data, p1);
    delete p1;
    socket->write(data.c_str(),data.length());
}
