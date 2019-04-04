#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "adjlistgraph.h"
#include "cache.h"

#include <QThread>


/** 线程管理类 */
class ThreadManager : public QThread
{
    Q_OBJECT

public:
    ThreadManager(QObject * parent = nullptr):
        QThread(parent),shortPathLength(0){}

    ThreadManager(QObject * parent,Area _area):
        QThread(parent),shortPathLength(0){
        this->graph.Init(_area);
        this->graph.printAdjList();
    }

    /** 开启线程，并运行其中的代码 */
    void run();

    /** 获取最短路径 */
    QString GetPath() { return this->shortPath; }

    /** 获取最短路径长度 */
    int GetPathLength() { return this->shortPathLength; }

    /** 初始化资源 */
    void Init(QString _start,QString _end){
        this->start_pos = _start;
        this->end_pos = _end;
    }

    /** 清除缓存 */
    void clearCache(){
        this->cache.clear();
        this->RunMessage.clear();
    }

public:
    AdjListGraph graph;                   //无向图类
    QString RunMessage;                   //运行信息
private:
    QString start_pos;                    //起点
    QString end_pos;                      //终点
    QString shortPath;                    //最短路径
    int shortPathLength;                  //最短路径长度
    Cache cache;                          //缓存对象

};

#endif // THREADMANAGER_H
