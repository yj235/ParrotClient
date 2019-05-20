#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

#include <iostream>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    MyLineEdit(QWidget *parent=0);

signals:
    void focus_out(void);

protected:
    virtual void focusOutEvent(QFocusEvent *e);
};

#endif // MYLINEEDIT_H
