#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "KVP.h"
#include "format.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <string>

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStringList contacts_list;
    QStringListModel *contacts_list_model;

public slots:
    void recv_data(QString);

private slots:
    void contacts_list_select(QModelIndex index);

    void on_pushButton_send_clicked();

private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
