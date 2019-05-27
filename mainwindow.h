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
#include <QDateTime>

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
    void recv_from_manager(QString name, QString data);

private slots:
    void contacts_list_select(QModelIndex index);
    void on_pushButton_send_clicked();

private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
