#ifndef GROUPWINDOW_H
#define GROUPWINDOW_H

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "common.h"

#include "string.h"

#include <unordered_map>

#include <QWidget>
#include <QStringList>
#include <QStringListModel>
#include <QDateTime>
#include <QCloseEvent>
#include <QShortcut>

namespace Ui {
class GroupWindow;
}

class GroupWindow : public QWidget
{
    Q_OBJECT

public:
    //群id
    unsigned group_id;
    std::unordered_map<unsigned int, std::string> id_name;
    //群成员
    QStringList member_list;
    QStringListModel *model;

    explicit GroupWindow(QWidget *parent = 0);
    //这样写不对
    //GroupWindow(unsigned int _id, QWidget *parent = 0) : id(_id){}
    ~GroupWindow();

    void init(void);
    void open(void);
    void recv_data(std::string name, std::string time, std::string message);

signals:
    void close(unsigned int);

public slots:
    void show_history_message(void);
    //void open(void);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_send_clicked();

private:
    Ui::GroupWindow *ui;

    QShortcut *shortCut_close;

    //请求群成员
    void query_member(void);
};

#endif // GROUPWINDOW_H
