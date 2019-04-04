#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(840,500);
    this->setWindowTitle("燕大校园导航V0.01");

    //初始化起点下拉框
    for (int i = 0;i < this->graph.GetVexCount();++i) {
        ui->StartPlace_Input->addItem(this->graph[size_t(i)]);
    }
    //初始化终点下拉框
    for (int i = 0;i < this->graph.GetVexCount();++i) {
        ui->EndPlace_Input->addItem(this->graph[size_t(i)]);
    }

    this->ui->ShowPath->clear();

    //初始化成员变量
    this->western_t.graph.Init(Area::western);
    this->east_t.graph.Init(Area::east);

    //绑定线程结束信号
    this->connect(&western_t,SIGNAL(finished()),this,SLOT(threadClosed_western()));
    this->connect(&east_t,SIGNAL(finished()),this,SLOT(threadClosed_east()));
    //绑定线程开始信号
    this->connect(&western_t,SIGNAL(started()),this,SLOT(threadStart_western()));
    this->connect(&east_t,SIGNAL(started()),this,SLOT(threadStart_east()));

    //绑定点击清空按钮信号的槽函数
    this->connect(this->ui->ClearButton,&QToolButton::clicked,[this](){
        this->ui->ShowPath->clear();
        this->ui->ShowShortPathLength->clear();
        this->runMessage.clear();
        this->ui->RunMessage->clear();
        //清除缓存
        this->western_t.clearCache();
        this->east_t.clearCache();
    });

    //绑定点击显示地图按钮信号的槽函数
    this->connect(this->ui->ShowMapButton,&QToolButton::clicked,[this](){
        this->showRunMessage("正在加载地图...\n");

        QMainWindow * showMapDialog = new QMainWindow(this);
        QImage map;
        QPalette palette;
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/res/map.png")));
        showMapDialog->setPalette(palette);

        map.load(":/res/map.png") ;

        showMapDialog->resize(QSize(map.width(),map.height()));

        showMapDialog->show();

        showMapDialog->setAttribute(Qt::WA_DeleteOnClose);
        this->showRunMessage("加载地图成功!\n");
    });
    //绑定点击Debug按钮信号的槽函数
    this->connect(this->ui->DebugButton,&QToolButton::clicked,[this](){
        this->showRunMessage("正在生成Debug信息...\n");


        QDialog debugMessageDialog;
        debugMessageDialog.resize(1900,600);

        QLabel adjList(&debugMessageDialog),BFS(&debugMessageDialog),DFS(&debugMessageDialog);
        QFont fontSize;
        QString adjList_data("邻接表信息如下：\n"),BFS_data("BFS遍历结果如下：\n"),DFS_data("DFS遍历结果如下：\n");

        fontSize.setPointSize(12);
        DFS.setFont(fontSize);
        adjList.setFont(fontSize);

        BFS.setFont(fontSize);
        adjList_data.append(this->graph.printAdjList());
        BFS_data.append(this->graph.BFSTraverse());
        DFS_data.append(this->graph.DFSTraverse());

        adjList.setText(adjList_data);
        BFS.move(0,470);
        BFS.setText(BFS_data);

        DFS.move(0,530);
        DFS.setText(DFS_data);

        this->hide();
        debugMessageDialog.exec();
        this->show();


        this->showRunMessage("生成Debug信息成功!\n");
    });
    //绑定点击开始导航按钮信号的槽函数
    this->connect(this->ui->StartButton,&QToolButton::clicked,[this](){
        int shortPathLength = 0;  //最短路径长度
        QString start,end,shortPath;

        //获取开始位置
        start = this->ui->StartPlace_Input->currentText();
        end = this->ui->EndPlace_Input->currentText();
        if(start == end){
            QMessageBox::critical(this,"提示","起点不能和终点相同");
            this->showRunMessage("起始位置或初始位置出错！\n");
            return;
        }

        this->showRunMessage("开始计算最短路径...\n");

        /************************* 多线程算法 *********************************/
        if(this->graph.getArea(start) == Area::western &&
                this->graph.getArea(end) == Area::western){
            //两点都在西区

            //初始化西区的计算线程
            this->western_t.Init(start,end);
            //启动线程
            this->western_t.start();
            //阻塞，直到线程结束
            if(this->western_t.wait()){
                //获取路径信息
                //shortPath.clear();
                shortPath = this->western_t.GetPath();
                shortPathLength =  this->western_t.GetPathLength();
                //显示线程运行信息
                this->showRunMessage("西区线程:");
                this->showRunMessage(this->western_t.RunMessage);
            }


        }else if (this->graph.getArea(start) == Area::western &&
                  this->graph.getArea(end) == Area::east) {
            //起点在西区，终点在东区

            //初始化西区线程
            this->western_t.Init(start,this->western_t.graph[size_t(this->western_t.graph.GetVexCount() - 1)]);
            //初始化东区线程
            this->east_t.Init(this->east_t.graph[0],end);

            //启动西区线程
            this->western_t.start();
            //启动东区线程
            this->east_t.start();
            //阻塞，直到线程结束
            if(this->western_t.wait() && this->east_t.wait()){
                shortPath = western_t.GetPath();
                shortPath.remove(QChar('\n'), Qt::CaseInsensitive);
                shortPath.append("-> ");
                shortPath.append(east_t.GetPath());
                shortPathLength = this->western_t.GetPathLength() + this->east_t.GetPathLength() + 770;
                //qDebug() << shortPath;
                //显示线程运行信息
                this->showRunMessage("西区线程:");
                this->showRunMessage(this->western_t.RunMessage);
                this->showRunMessage("东区线程:");
                this->showRunMessage(this->east_t.RunMessage);
            }


        }else if (this->graph.getArea(start) == Area::east &&
                  this->graph.getArea(end) == Area::western) {
            //起点在东区，终点在西区

            //初始化西区线程
            this->western_t.Init(this->western_t.graph[size_t(this->western_t.graph.GetVexCount() - 1)],end);
            //初始化东区线程
            this->east_t.Init(start,this->east_t.graph[0]);

            //启动东区线程
            this->east_t.start();
            //启动西区线程
            this->western_t.start();

            //阻塞，直到线程结束
            if(this->east_t.wait() && this->western_t.wait()){
                shortPath = east_t.GetPath();
                shortPath.remove(QChar('\n'), Qt::CaseInsensitive);
                shortPath.append("-> ");
                shortPath.append(western_t.GetPath());
                shortPath.append("\n");
                shortPathLength = this->western_t.GetPathLength() + this->east_t.GetPathLength() + 770;

                //显示线程运行信息
                this->showRunMessage("西区线程:");
                this->showRunMessage(this->western_t.RunMessage);
                this->showRunMessage("东区线程:");
                this->showRunMessage(this->east_t.RunMessage);
            }


        }else if (this->graph.getArea(start) == Area::east &&
                  this->graph.getArea(end) == Area::east) {
            //两点都在东区

            //初始化东区的计算线程
            this->east_t.Init(start,end);
            //启动线程
            this->east_t.start();

            //阻塞，直到线程结束
            if(this->east_t.wait()){
                //获取路径信息
                shortPath = this->east_t.GetPath();
                shortPathLength =  this->east_t.GetPathLength();

                //显示线程运行信息
                this->showRunMessage("东区线程:");
                this->showRunMessage(this->east_t.RunMessage);
            }



        }
        this->ui->ShowShortPathLength->setText(QString::number(shortPathLength));
        //显示最短路径
        this->ui->ShowPath->setPlainText(shortPath);
    });


}

void MainWindow::showRunMessage(const QString & msg){
    runMessage.append(msg);
    this->ui->RunMessage->setText(runMessage);
}

void MainWindow::threadClosed_east(){
    runMessage.append("东区计算线程结束\n");
    this->ui->RunMessage->setText(runMessage);
}
void MainWindow::threadStart_western(){
    runMessage.append("西区计算线程开始...\n");
    this->ui->RunMessage->setText(runMessage);
}
void MainWindow::threadStart_east(){
    runMessage.append("东区计算线程开始...\n");
    this->ui->RunMessage->setText(runMessage);
}
void MainWindow::threadClosed_western(){
    runMessage.append("西区计算线程结束\n");
    this->ui->RunMessage->setText(runMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}
