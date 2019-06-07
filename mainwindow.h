#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <string>
#include <unordered_map>
#include <map>
#include <algorithm>

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QDateTime>
#include <QShortcut>
#include <QMessageBox>

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
    //联系人map<name_id>
    std::unordered_map<std::string, unsigned int> name_id;
    //联系人map<name_id>
    //替换中...
    std::unordered_map<std::string, unsigned int> contacts_name_id;

    //替换为group_id_name
    //群map<name_id>
    std::unordered_map<std::string, unsigned int> group_name_id;
    //群map<id_name>
    std::unordered_map<unsigned int, std::string> groupID_name;

    //联系人vector<id>
    std::vector<unsigned int> vector_id;
    //联系人vector<id>
    //替换中
    std::vector<unsigned int> contacts_vector_id;
    //群vector<id>
    std::vector<unsigned int> group_vector_id;

    //搜索用户id
    unsigned int contacts_search_id;
    //搜索群id
    unsigned int group_search_id;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    //转发信号至manager 双击联系人列表->创建聊天窗口
    void double_clicked_on_contacts_list_view(QModelIndex);
    //转发信号至manager 双击群列表->创建聊天窗口
    void double_clicked_on_group_list_view(QModelIndex);

public slots:
    void query_contacts_list(void);
    void query_group_list(void);

private slots:
    //从manager获取信息
    //void recv_from_manager(unsigned int id, QString data);

    void on_pushButton_searchContacts_clicked();

    void on_pushButton_searchGroup_clicked();

private:
    Ui::MainWindow *ui;

    //关闭窗口快捷键
    QShortcut *shortcut_close;

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
