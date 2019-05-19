#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "KVP.h"
#include "format.h"

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_send_clicked();

private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
