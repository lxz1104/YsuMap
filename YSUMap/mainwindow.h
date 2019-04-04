#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "adjlistgraph.h"
#include "threadmanager.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    //显示运行信息
    void showRunMessage(const QString & msg);
private:
    Ui::MainWindow *ui;                 //ui对象
    AdjListGraph graph;                 //无向网对象
    ThreadManager east_t;               //东区线程对象
    ThreadManager western_t;            //西区线程对象
    QString runMessage;                 //记录运行信息
private slots:
    void threadClosed_western();
    void threadClosed_east();
    void threadStart_western();
    void threadStart_east();
};

#endif // MAINWINDOW_H
