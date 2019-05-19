#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void f(void);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;

    QTcpSocket *socket;
};

#endif // WIDGET_H
