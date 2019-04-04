#include "threadmanager.h"

#include <QStack>
#include <QDebug>

/** 线程运行的代码 */
void ThreadManager::run(){
    this->RunMessage.clear();
    this->shortPath.clear();
    this->shortPathLength = 0;
    //判断起点和终点数据是否有效
    if(this->graph[this->start_pos] == -1 || this->graph[this->end_pos] == -1){
        return;
    }
    //起点和终点不能相等
    if(this->start_pos == this->end_pos){
        return;
    }


    int endIndex = (this->graph)[end_pos];                            //终点在边数组中的下标
    QVector<int>  cache_disk(this->graph.GetVexCount());              //记录最短路径的数组
    QVector<int>  cache_path(this->graph.GetVexCount());              //记录路径信息
    int * disk = new int[this->graph.GetVexCount()];                  //记录最短路径的数组
    int * path = new int[this->graph.GetVexCount()];                  //记录路径信息

    if(this->cache.iSInCache(this->start_pos)){
        //如果在缓存中找到
        if(this->cache.popCache(this->start_pos,cache_disk,cache_path)){
            for (int i = 0;i < this->graph.GetVexCount();++i) {
                disk[i] = cache_disk[i];
                path[i] = cache_path[i];
            }
            this->RunMessage = "命中缓存\n";
            qDebug() << "命中缓存\n" ;
        }else {
            this->RunMessage = "获取缓存失败\n";
            qDebug() << "获取缓存失败\n" ;
        }

    }else {
        //使用Dijkstra算法查找最短路径
        this->graph.Dijkstra(this->start_pos,disk,path);

        cache_disk.clear();
        cache_path.clear();
        for (int i = 0;i < this->graph.GetVexCount();++i) {
            cache_disk.push_back(disk[i]);
            cache_path.push_back(path[i]);
        }
        this->RunMessage = "新增信息到缓存\n";
        qDebug() << "新增信息到缓存\n" ;
        this->cache.addToCache(start_pos,cache_disk,cache_path);
    }


    QStack<int> s;

    //获取最短路径信息
    for (int i = endIndex; i != -1; i = path[i])
    {
        s.push(i);
    }
    if (s.top() == endIndex) {
        return;
    }
    while (!s.empty())
    {
        shortPath.append((this->graph)[size_t(s.top())]);
        s.pop();
        shortPath.append(QString(s.empty() ? "\n" : "-> "));
    }

    this->shortPathLength = disk[endIndex];
}
