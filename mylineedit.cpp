#include "mylineedit.h"

using namespace std;

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
    emit focus_out();
}
