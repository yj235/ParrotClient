#ifndef GROUPWINDOW_H
#define GROUPWINDOW_H

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "common.h"

#include "string.h"

#include <QWidget>
#include <QStringList>
#include <QStringListModel>

namespace Ui {
class GroupWindow;
}

class GroupWindow : public QWidget
{
    Q_OBJECT

public:
    unsigned id;
    QStringList member_list;
    QStringListModel *model;

    explicit GroupWindow(QWidget *parent = 0);
    //这样写不对
    //GroupWindow(unsigned int _id, QWidget *parent = 0) : id(_id){}
    ~GroupWindow();

    void init(void);

private:
    Ui::GroupWindow *ui;

    void query_member(void);
};

#endif // GROUPWINDOW_H
