#ifndef ADJLISTGRAPH_H
#define ADJLISTGRAPH_H

#include <array>
#include <QString>
#include <QObject>

/** 定义最大顶点数 */
const int MAX_VERTEX = 30;

/** 区域枚举 */
enum Area {
    //西区
    western,
    //东区
    east
};

/** 无向图类 */
class AdjListGraph;

/**
 * 边/弧的结点类
 */
class Node {
private:
    int adjVex;                     //该边指向这条边邻接点的下标
    size_t weight;            //权重
    Node * nextEdge;                //指向下一条边结点的指针
public:
    //默认构造
    Node() :
        adjVex(0), weight(0), nextEdge(nullptr) {}
    //带参构造
    Node(int _adjVex, size_t _weight, Node * _nextEdge) :
        adjVex(_adjVex), weight(_weight), nextEdge(_nextEdge) {}
    friend class AdjListGraph;
    friend class VexNode;
};

/**
 * 顶点结点类
 */
class VexNode {
private:
    QString vex;                 //顶点值(在顶点数组中的值)
    Node * firstEdge;            //指向第一条边的指针
public:
    VexNode() :
        firstEdge(nullptr) {}
    VexNode(QString & _vex, Node * _firstEdge) :
        vex(_vex), firstEdge(_firstEdge) {}
    friend class AdjListGraph;
};

/**
 * 无向网类
 */
class AdjListGraph:public QObject
{
    Q_OBJECT

public:
    /** 删除无用自生成函数 */
    AdjListGraph operator=(AdjListGraph &) = delete;
    AdjListGraph(const AdjListGraph &) = delete;
    /** 构造函数 */
    AdjListGraph(QWidget * parent = nullptr);
    /** 析构函数 */
    ~AdjListGraph();

    /** 获取顶点数 */
    int GetVexCount() { return int(this->vexCount); }
    /** 获取边数 */
    int GetEdgeCount() { return int(this->edgeCount); }
    /** 打印邻接表 */
    QString printAdjList();
    /** 深度优先遍历 */
    QString DFSTraverse();
    /** 广度优先遍历 */
    QString BFSTraverse();
    /** 销毁邻接表 */
    void DeleteAdjList();

    /** 初始化无向网 */
    bool Init(Area _area);

    /**
     * Dijkstra算法求最短路径
     * @param startVex 包含起点位置的string对象
     * @param disk 存储起点到各顶点的最短距离
     * @param path 存储起点到各顶点的路径信息
     * @return 获取成功，返回true；获取异常，返回false
     */
    bool Dijkstra(const QString & startVex, int * disk, int * path);

    /**
     * 重载[]操作符，通过下标获取顶点名
     * @param _area 地区枚举类型（东区或西区）
     */
    void setArea(Area _area) { this->area = _area; }

    /** 获取长度(顶点和邻接点的距离) */
    int GetWeight(size_t index) {
        return this->IsInit == true ? int(this->vexes.at(index).firstEdge->weight) : -1;
    }

    /**
     * 重载[]操作符，通过下标获取顶点名
     * @param index 顶点数组下标
     * @return 存在，返回顶点string对象；不存在，返回空string对象
     */
    QString operator[](size_t index) {
        return this->IsInit == true ? this->vexes.at(index).vex : QString("此对象未初始化，请初始化后重试");
    }

    /**
     * 重载[]操作符，通过顶点名获取顶点在顶点数组中的下标
     * @param vex_find 含有顶点名的string对象
     * @return 存在，返回顶点在顶点数组中的下标；不存在，返回-1
     */
    int operator[](QString & vex_find) {
        return this->IsInit == true ? this->LocateVex_AdjList(vex_find) : -1;
    }

    /**
     * 判断某地点在东区还是西区
     * @param vex_find 要定位的地点
     * @return Area::western，西区；Area::east，东区
     */
    Area getArea(QString & vex_find){
        QString flag("燕宏桥");
        return (*this)[vex_find] < (*this)[flag] ? Area::western : Area::east;
    }

private:
    /**
     * 返回某个顶点在顶点数组中的下标
     * @param vex 要定位的顶点
     * @return 存在，返回下标（从0开始）；不存在，返回-1
     */
    int LocateVex_AdjList(const QString & vex);

    /**
     * 为图添加边
     * @param vex1 图顶点
     * @param vex2 图顶点的邻接点
     * @param wight 权值
     * @return 添加成功，返回下标true；添加失败，返回false
     */
    bool addEdge(QString & vex1, QString & vex2, size_t wight);

    /**
     * 深度优先遍历核心算法
     * @param index 要遍历的顶点下标
     */
    void DFS(int index,QString & data);
private:
    std::array<VexNode, MAX_VERTEX>  vexes;           //邻接表数组
    size_t vexCount;								  //顶点数量
    size_t edgeCount;							      //图的边数
    bool * visited;                                   //访问状态数组：遍历时用来判断顶点是否已被遍历
    Area area;							              //标识所在地区
    QString vexFile;						          //顶点数据文件
    QString edgeFile;                                 //边数据文件
    QString	weightFile;						          //权值数据文件
    bool IsInit;                                      //判断当前数据是否被初始化
};


#endif // ADJLISTGRAPH_H
