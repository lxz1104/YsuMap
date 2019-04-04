#include "adjlistgraph.h"

#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QVector>
#include <QQueue>
#include <QWidget>
#include <queue>


struct LessHeap {
    size_t adjVex;
    size_t weight;
    //实现最小堆，按升序排列，重载运算符，重定义优先级，以小为先
    friend bool operator< (LessHeap a, LessHeap b)
    {
        return a.weight > b.weight;
    }
};

/** Dijkstra算法求最短路径 */
bool AdjListGraph::Dijkstra(const QString & startVex, int * disk, int * path) {
    //判断邻接表是否以被初始化
    if(!this->IsInit){
        qDebug() << "邻接表未初始化，遍历失败！" << endl;
        return false;
    }
    bool * isFind = new bool[this->vexCount];
    LessHeap * dist_t = new LessHeap[this->vexCount];
    assert(isFind != nullptr && dist_t != nullptr);
    int startIndex = this->LocateVex_AdjList(startVex);
    if (startIndex == -1) {
        qDebug() << "未找到该顶点" << endl;
        delete[] isFind;
        delete[] dist_t;
        return false;
    }
    //创建小顶堆

    std::priority_queue<LessHeap> q;
    for (size_t i = 0; i < this->vexCount; ++i)
    {
        dist_t[i].adjVex = i;
        dist_t[i].weight = INT_MAX;
        isFind[i] = false;
        path[i] = -1;
    }
    dist_t[startIndex].weight = 0;
    q.push(dist_t[startIndex]);
    while (!q.empty())
    {
        LessHeap temp = q.top();
        size_t u = temp.adjVex;
        q.pop();
        if (isFind[u]) {
            continue;
        }
        isFind[u] = true;

        Node * p = this->vexes[u].firstEdge;

        while (p)
        {
            int tempVex = p->adjVex;
            size_t tempWeight = p->weight;
            if (!isFind[tempVex] && dist_t[tempVex].weight > dist_t[u].weight + tempWeight) {
                dist_t[tempVex].weight = dist_t[u].weight + tempWeight;
                path[tempVex] = int(u);
                q.push(dist_t[tempVex]);
            }
            p = p->nextEdge;
        }
    }
    for (size_t i = 0; i < this->vexCount; ++i)
    {
        disk[i] = int(dist_t[i].weight);
    }

    delete[] isFind;
    delete[] dist_t;
    return true;
}

/** 广度优先遍历 */
QString AdjListGraph::BFSTraverse() {
    QString data;

    //判断邻接表是否以被初始化
    if(!this->IsInit){
        qDebug() << "邻接表未初始化，遍历失败！" << endl;
        return QString("邻接表未初始化，遍历失败！");
    }
    //初始化状态数组
    for (size_t i = 0; i < this->vexCount; ++i)
    {
        visited[i] = false;
    }
    //生成队列,记录邻接表边数组下标
    QQueue<int> vexQueue;
    qDebug() << "广度优先遍历结果(BFS):";
    for (size_t i = 0; i < this->vexCount; ++i)
    {
        if (!visited[i])
        {
            //            data.append(QString(i == 0 ? "NULL" : "\0"));
            //            data.append(" -> ");
            data.append(this-> vexes[size_t(i)].vex);
            qDebug() << QString(i == 0 ? "NULL" : "\0") << " -> "
                     << this-> vexes[size_t(i)].vex;
            visited[i] = true;
            vexQueue.push_back(int(i));
            while (!vexQueue.empty())
            {
                int iVex = vexQueue.front();
                vexQueue.pop_front();
                Node * pEdge = this->vexes[size_t(iVex)].firstEdge;

                while (pEdge)
                {
                    if (!visited[pEdge->adjVex])
                    {
                        data.append(" -> ");
                        data.append(this->vexes[size_t(pEdge->adjVex)].vex);
                        qDebug() << " -> " << this->vexes[size_t(pEdge->adjVex)].vex;
                        visited[pEdge->adjVex] = true;
                        vexQueue.push_back(pEdge->adjVex);
                    }
                    pEdge = pEdge->nextEdge;
                }
            }
        }
    }
    data.append("\n");
    qDebug() << endl;
    return data;
}

/** 深度优先遍历 */
QString AdjListGraph::DFSTraverse() {
    QString data;
    //判断邻接表是否以被初始化
    if(!this->IsInit){
        qDebug() << "邻接表未初始化，遍历失败！\n" << endl;
        return QString("邻接表未初始化，遍历失败！\n");
    }
    //初始化状态数组
    for (size_t i = 0; i < this->vexCount; ++i)
    {
        this->visited[i] = false;
    }
    qDebug() << "深度优先遍历结果(DFS):";
    for (size_t i = 0; i < this->vexCount; ++i)
    {
        if (!this->visited[i]) {
            this->DFS(int(i),data);
        }

    }
    data.append("\n");
    qDebug() << endl;
    return data;
}

/** 深度优先遍历核心算法 */
void AdjListGraph::DFS(int index,QString & data) {
    data.append(QString(index == 0 ? "NULL" : "\0"));
    data.append(" -> ");
    data.append(this->vexes[size_t(index)].vex);
    qDebug() << QString(index == 0 ? "NULL" : "\0")
             << " -> " << this->vexes[size_t(index)].vex;
    this->visited[index] = true;

    Node * pEdge = this->vexes[size_t(index)].firstEdge;
    while (pEdge)
    {
        int j = pEdge->adjVex;
        if (!visited[j])
        {
            this->DFS(j,data);
        }
        pEdge = pEdge->nextEdge;
    }
}

/** 打印邻接表 */
QString AdjListGraph::printAdjList() {

    QString data;

    //判断邻接表是否以被初始化
    if(!this->IsInit){
        qDebug() << "邻接表未初始化，打印失败！" << endl;
        return QString("邻接表未初始化，打印失败！");
    }

    //qDebug() << "\n邻接表：" << endl;
    for (size_t i = 0; i < this->vexCount; ++i) {
        VexNode vNode = this->vexes[i];
        data.append("顶点");
        data.append(QString::number(i));
        data.append(": ");
        data.append(vNode.vex);
        //qDebug() << "顶点" << i << ": " << vNode.vex;
        Node *eNode = vNode.firstEdge;
        while (eNode) {
            data.append(" ");
            data.append("(");
            data.append(QString::number(eNode->weight));
            data.append(")");
            data.append("-> ");
            data.append(this->vexes[size_t(eNode->adjVex)].vex);
            //qDebug() << " " << "(" << eNode->weight << ")" << "-> " << this->vexes[size_t(eNode->adjVex)].vex;
            eNode = eNode->nextEdge;
        }
        data.append("\n");
    }
    return data;
}

/** 初始化对象函数 */
bool AdjListGraph::Init(Area _area){

    QString currPath = QCoreApplication::applicationDirPath();
    currPath.replace("\\","/");
    QString confPath = currPath;
    this->vexFile = currPath;
    this->edgeFile = currPath;
    this->weightFile = currPath;
    confPath.append("/config.ini");
    if(this->IsInit){
        this->DeleteAdjList();
    }
    //根据地区获取不同的数据路径
    const char * vexCountFile = Area::western == _area ? "/Western/VexCount" : "/East/VexCount";
    const char * edgeCountFile = Area::western == _area ? "/Western/EdgeCount" : "/East/EdgeCount";
    const char * vexFile_path = Area::western == _area ? "/Western/VexDataPath" : "/East/VexDataPath";
    const char * edgeFile_path = Area::western == _area ? "/Western/EdgeDataPath" : "/East/EdgeDataPath";
    const char * weightFile_path = Area::western == _area ? "/Western/WeightDataPath" : "/East/WeightDataPath";

    //定义配置文件读取对象
    QSettings * configIniRead = new QSettings(confPath, QSettings::IniFormat);
    //获取顶点数
    this->vexCount = size_t(configIniRead->value(vexCountFile).toInt());
    //获取边数
    this->edgeCount = size_t(configIniRead->value(edgeCountFile).toInt());
    //获取顶点数据文件路径
    this->vexFile += configIniRead->value(vexFile_path).toString();
    //获取边数据文件路径
    this->edgeFile += configIniRead->value(edgeFile_path).toString();
    //获取权值文件路径
    this->weightFile += configIniRead->value(weightFile_path).toString();

    //异常处理

    //读入入完成后删除指针
    delete configIniRead;

    //读取顶点数据
    QFile vexData(this->vexFile);
    if(vexData.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString temp;
        for (size_t i = 0;i < this->vexCount; ++i) {
            temp = vexData.readLine();
            temp.remove(QChar('\n'), Qt::CaseInsensitive);
            this->vexes[i].vex = temp;
        }

        vexData.close();
    }else {
        qDebug() << this->vexFile;
        qDebug() << "文件读取出错";
        return false;
    }


    //读取边数据
    //临时储存边信息
    QVector<QString> edgeData_temp1;
    QVector<QString> edgeData_temp2;
    QFile edgeData(this->edgeFile);
    if(edgeData.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString temp1,temp2;

        //读取边数据
        for (size_t i = 0; i < this->edgeCount; ++i) {
            temp1 = edgeData.readLine();
            temp1.remove(QChar('\n'), Qt::CaseInsensitive);
            edgeData_temp1.push_back(temp1);

            temp1 = edgeData.readLine();
            temp1.remove(QChar('\n'), Qt::CaseInsensitive);
            edgeData_temp2.push_back(temp1);
        }
        //关闭文件
        edgeData.close();
    }else {
        qDebug() << this->edgeFile;
        qDebug() << "文件读取出错";
        return false;
    }

    //读取权值数据
    QVector<int> weightData_temp;
    QFile weightData(this->weightFile);
    if(weightData.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString temp;
        for (size_t i = 0;i < this->edgeCount ; ++i) {
            temp = weightData.readLine();
            temp.remove(QChar('\n'), Qt::CaseInsensitive);
            weightData_temp.push_back(temp.toInt());
            //qDebug() << weightData_temp[int(i)];
        }
        weightData.close();
    }else {
        qDebug() << this->weightFile;
        qDebug() << "文件读取出错";
        return false;
    }

    //建立邻接表
    for (size_t i = 0; i < this->edgeCount; ++i) {
        if (!this->addEdge(edgeData_temp1[int(i)], edgeData_temp2[int(i)], size_t(weightData_temp[int(i)]))) {
            qDebug() << "初始化地图数据出错" << endl;
            return false;
        }

    }

    visited = new bool[this->vexCount];
    //设置类初始化状态
    this->IsInit = true;
    return true;
}

/** 构造函数 */
AdjListGraph::AdjListGraph(QWidget * parent):QObject(parent){
    QString currPath = QCoreApplication::applicationDirPath();
    currPath.replace("\\","/");
    QString confPath = currPath;

    this->vexFile = currPath;
    this->edgeFile = currPath;
    this->weightFile = currPath;
    confPath.append("/config.ini");

    //定义配置文件读取对象
    QSettings * configIniRead = new QSettings(confPath, QSettings::IniFormat);
    //获取顶点数
    this->vexCount = size_t(configIniRead->value("/Size/VexCount").toInt());
    //获取边数
    this->edgeCount = size_t(configIniRead->value("/Size/EdgeCount").toInt());
    //获取顶点数据文件路径
    this->vexFile += configIniRead->value("/Data/VexDataPath").toString();
    //获取边数据文件路径
    this->edgeFile += configIniRead->value("/Data/EdgeDataPath").toString();
    //获取权值文件路径
    this->weightFile += configIniRead->value("/Data/WeightDataPath").toString();

    //异常处理

    //读入入完成后删除指针
    delete configIniRead;

    //读取顶点数据
    QFile vexData(this->vexFile);
    if(vexData.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString temp;
        for (size_t i = 0;i < this->vexCount; ++i) {
            temp = vexData.readLine();
            temp.remove(QChar('\n'), Qt::CaseInsensitive);
            this->vexes[i].vex = temp;
        }

        vexData.close();
    }else {
        qDebug() << this->vexFile;
        qDebug() << "文件读取出错";
        //throw std::out_of_range("Read file Failure!");
        exit(0);
    }


    //读取边数据
    //临时储存边信息
    QVector<QString> edgeData_temp1;
    QVector<QString> edgeData_temp2;
    QFile edgeData(this->edgeFile);
    if(edgeData.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString temp1,temp2;

        //读取边数据
        for (size_t i = 0; i < this->edgeCount; ++i) {
            temp1 = edgeData.readLine();
            temp1.remove(QChar('\n'), Qt::CaseInsensitive);
            edgeData_temp1.push_back(temp1);

            temp1 = edgeData.readLine();
            temp1.remove(QChar('\n'), Qt::CaseInsensitive);
            edgeData_temp2.push_back(temp1);
        }
        //关闭文件
        edgeData.close();
    }else {
        qDebug() << this->edgeFile;
        qDebug() << "文件读取出错";
        throw std::out_of_range("Read file Failure!");
    }

    //读取权值数据
    QVector<int> weightData_temp;
    QFile weightData(this->weightFile);
    if(weightData.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString temp;
        for (size_t i = 0;i < this->edgeCount ; ++i) {
            temp = weightData.readLine();
            temp.remove(QChar('\n'), Qt::CaseInsensitive);
            weightData_temp.push_back(temp.toInt());
            //qDebug() << weightData_temp[int(i)];
        }
        weightData.close();
    }else {
        qDebug() << this->weightFile;
        qDebug() << "文件读取出错";
        throw std::out_of_range("Read file Failure!");
    }

    //建立邻接表
    for (size_t i = 0; i < this->edgeCount; ++i) {
        if (!this->addEdge(edgeData_temp1[int(i)], edgeData_temp2[int(i)], size_t(weightData_temp[int(i)]))) {
            qDebug() << "初始化地图数据出错" << endl;
            throw std::out_of_range("Init AdjList error");
        }

    }
    //设置类初始化状态
    this->IsInit = true;
    visited = new bool[this->vexCount];

}

/** 添加图顶点 */
bool AdjListGraph::addEdge(QString & vex1, QString & vex2,size_t wight) {
    //获得两个顶点在顶点数组中的坐标
    int x = this->LocateVex_AdjList(vex1);
    int y = this->LocateVex_AdjList(vex2);
    if (x == -1 || y == -1) {
        //cout << "数据有误，创建图失败！" << endl;
        return false;
    }

    //构造新结点
    Node * edgeNode = new Node(x, wight, this->vexes[size_t(y)].firstEdge);
    assert(edgeNode != nullptr);
    this->vexes[size_t(y)].firstEdge = edgeNode;

    //对称性
    edgeNode = new Node(y,wight, this->vexes[size_t(x)].firstEdge);
    this->vexes[size_t(x)].firstEdge = edgeNode;
    return true;
}

/** 返回某个顶点在顶点数组中的下标 **/
int AdjListGraph::LocateVex_AdjList(const QString & vex) {
    //使用正向迭代器遍历
    for (auto it = this->vexes.begin(); it != this->vexes.end(); ++it) {
        if (it->vex == vex) {

            return int(it - this->vexes.begin());
        }
    }
    return -1;
}

/** 销毁邻接表 */
void AdjListGraph::DeleteAdjList(){
    if(!this->IsInit){
        return;
    }
    //销毁堆
    for (size_t i = 0; i < this->vexCount; ++i)
    {
        Node* pEdge = this->vexes[i].firstEdge;
        while (pEdge)
        {
            Node* q = pEdge;
            pEdge = pEdge->nextEdge;
            if (!q) {
                delete q;
            }
        }
        this->vexes[i].firstEdge = nullptr;
    }

    if(visited){
        delete [] visited;
        visited = nullptr;
    }
    //重置成员变量
    this->edgeCount = 0;
    this->vexCount = 0;

    //重置类初始化状态
    this->IsInit = false;
}

/** 析构函数 */
AdjListGraph::~AdjListGraph(){
    //销毁邻接表，回收资源
    if(this->IsInit){
        this->DeleteAdjList();
    }
}

