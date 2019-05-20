#include "mylineedit.h"

using namespace std;

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
    cout << "focus out" << endl;
    emit focus_out();
}
