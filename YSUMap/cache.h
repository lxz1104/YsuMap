#ifndef CHACHE_H
#define CHACHE_H

#include <QVector>
#include <QString>

/** 缓存结点信息 */
struct CacheNode{
    /** 起始位置 */
    QString startPlace;
    /** 保存最短距离的容器 */
    QVector<int> disk;
    /** 保存路径的容器 */
    QVector<int> path;
    /** 在缓存中的权值 */
    size_t weight;

    //重载运算符，重定义优先级，以权大为先
    friend bool operator> (CacheNode a, CacheNode b)
    {
        return a.weight > b.weight;
    }
    /** 默认构造 */
    CacheNode():weight(0){

    }
    /** 拷贝构造函数 */
    CacheNode(const CacheNode & _cacheNode){
        this->path = _cacheNode.path;
        this->disk = _cacheNode.disk;
        this->weight = _cacheNode.weight;
        this->startPlace = _cacheNode.startPlace;
    }
};

/** 缓存类 */
class Cache
{
public:
    Cache(unsigned int _maxCacheCount = 10);
    /** 添加数据到缓存区间 */
    bool addToCache(QString & _startPlace,QVector<int> & _disk,QVector<int> & _path);
    /** 从缓存中获取数据 */
    bool popCache(QString _startPlace,QVector<int> & _disk,QVector<int> & _path);
    /** 在缓存中查找数据是否存在 */
    bool iSInCache(const QString & _startPlace);
    /** 获取缓存的大小 */
    unsigned int size() { return this->currentCacheCount; }
    /** 清除缓存区间 */
    void clear();
private:
    /** 创建堆 */
    void make_heap();
    /** 调整堆 */
    void adjust_heap();
    /** 堆排序 */
    void sort();
    /** 根据起点信息获取在容器中的位置 */
    int locateCache(const QString & _startPlace);
    /** 交换两个节点 */
    void swap(CacheNode & a, CacheNode & b);
private:
    QVector<CacheNode> cacheData;                    //缓存数据容器
    unsigned int  maxCacheCount;                     //最大缓存数
    unsigned int  currentCacheCount;                 //当前缓存数量
};

#endif // CHACHE_H
