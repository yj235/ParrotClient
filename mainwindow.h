#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <string>
#include <unordered_map>
#include <map>

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QDateTime>
#include <QShortcut>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    //联系人列表
    QStringList contacts_list;
    //联系人列表模型
    QStringListModel *contacts_list_model;
    //群列表
    QStringList group_list;
    //群列表模型
    QStringListModel *group_list_model;

    //联系人name_id;
    std::map<std::string, unsigned int> name_id;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    //转发信号 双击联系人列表 转发至manager
   void double_clicked_on_contacts_list_view(QModelIndex);

private slots:
    //从manager获取信息
    void recv_from_manager(unsigned int id, QString data);
    //发送信息
    void on_pushButton_send_clicked();
    //联系人ListView单击(信号)->焦点至文本编辑器(槽)
    //void contacts_clicked(QModelIndex);
    //void group_clicked(QModelIndex);

    //获取联系人列表
    void on_pushButton_getGroupList_clicked();

private:
    Ui::MainWindow *ui;

    //关闭窗口快捷键
    QShortcut *shortcut_close;

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
