#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "common.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <string>

#include <QWidget>
#include <QString>
#include <QShortcut>
#include <QDateTime>
#include <QDebug>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    unsigned int contacts_id;

    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();

    void recv_data(std::string data);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ChatWindow *ui;

    //退出快捷键
    QShortcut *shortCut_close;
};

#endif // CHATWINDOW_H
