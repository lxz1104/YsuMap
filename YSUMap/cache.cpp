#include "cache.h"

#include <QDebug>

/** 构造函数 */
Cache::Cache(unsigned int _maxCacheCount):maxCacheCount(_maxCacheCount),currentCacheCount(0)
{

}

/** 添加数据到缓存区间 */
bool Cache::addToCache(QString & _startPlace,QVector<int> & _disk,QVector<int> & _path){
    int index = -1;
    if((index = this->locateCache(_startPlace)) != -1){
        this->cacheData[index].weight++;
        return true;
    }else {
        if(this->currentCacheCount == this->maxCacheCount){
            CacheNode temp;
            temp.startPlace = _startPlace;
            temp.disk = _disk;
            temp.path = _path;
            this->cacheData.pop_back();
            this->cacheData.push_back(temp);
        }else {
            CacheNode temp;
            temp.startPlace = _startPlace;
            temp.disk = _disk;
            temp.path = _path;
            this->cacheData.push_back(temp);
            this->currentCacheCount++;
        }

    }
    this->sort();

    return true;
}
/** 从缓存中获取数据 */
bool Cache::popCache(QString _startPlace,QVector<int> & _disk,QVector<int> & _path){
    int index = -1;
    if((index = this->locateCache(_startPlace)) == -1){
        return false;
    }else {
        _disk.clear();
        _path.clear();
        _disk = this->cacheData.at(index).disk;
        _path = this->cacheData.at(index).path;
        this->cacheData[index].weight++;
        this->sort();
        return true;
    }
}

/** 在缓存中查找数据是否存在 */
bool Cache::iSInCache(const QString & _startPlace){
    return this->locateCache(_startPlace) == -1 ? false : true;
}

/** 根据起点信息获取在容器中的位置 */
int Cache::locateCache(const QString & _startPlace){
    for (auto it = this->cacheData.begin(); it != this->cacheData.end() ;++it) {
        if((*it).startPlace == _startPlace){
            return int(it - this->cacheData.begin());
        }
    }
    return -1;
}

/** 清除缓存区间 */
void Cache::clear(){
    this->cacheData.clear();
    this->currentCacheCount = 0;
}

/** 调整堆 */
void Cache::adjust_heap(){

}
/** 堆排序 */
void Cache::sort(){
    for (int i = 0;i < int(this->currentCacheCount);++i) {
        for (int j = i + 1;j < int(this->currentCacheCount);++j) {
            if(this->cacheData.at(i).weight < this->cacheData.at(j).weight){
                this->swap(this->cacheData[i],this->cacheData[j]);
            }
        }
    }
}

/** 交换两个节点 */
void Cache::swap(CacheNode & a, CacheNode & b){
    CacheNode temp(a);
    a = b;
    b = temp;
}
/** 创建堆 */

