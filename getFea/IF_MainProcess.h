#ifndef _IF_MAINPROCESS_H_
#define _IF_MAINPROCESS_H_

#include "IF_BaseScan.h"
#include "IF_BaseTika.h"
#include "IF_Common.h"

class MainProcess{
public:
    /**
        功能：(一些初始化的量是否要在这里赋值)
            构造函数
        参数：
            无
        返回值：
            无
    **/
    MainProcess();
    
    /**
        功能：
            析构函数
        参数：
            无
        返回值：
            无
    **/
    ~MainProcess();
    
    /**
    功能：
        主线程向界面提供的特征提取接口
    参数：
        path[in]，待进行特征提取的文件或目录路径
    返回值：
        空
    **/
    int extractFeatures(const string path);
    
    /**
    功能：
        主线程向界面提供的特征匹配接口
    参数：
        path[in]，待进行特征匹配的文件或目录路径
    返回值：
        空
    **/
    int matchFeatures(const string path);
    
    map<string, vector<struct ScanResult>> resultMap;   //结果数据结构，需与界面确定最终的结构和信息
    map<string, struct MetaInfo> metaLib;//文件的元信息库，需与界面确定最终的结构和信息
    
    
private:
    BaseTika *tika;
    BaseScan *textScan;
    BaseScan *picScan;
    
    string metaPath;    //文件元信息库存储路径
    string textLibPath; //文本特征库存储路径
    string picLibPath;  //图片特征库存储路径
    
    int loadLibraries();//加载各个库的接口，在匹配前主线程内部调用
    int dumpLibraries();//存储各个库的接口，在所有文件提取特征完毕后主线程内部调用
};

#endif