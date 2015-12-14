#ifndef _IF_BASESCAN_H_
#define _IF_BASESCAN_H_

#include <iostream>
#include "IF_Common.h"
using namespace std;

class BaseScan{
public: 
    /**
        功能：
            构造函数
        参数：
            无
        返回值：
            无
    **/
    BaseScan(string libPath){ this->libPath=libPath; };
    
    /**
        功能：
            析构函数
        参数：
            无
        返回值：
            无
    **/
    virtual ~BaseScan(){};
    
    /**
        功能：
        提取特征
        参数：
        pf[in]，待提取的文件信息
    返回值：
        0，识别；1，成功
    **/
    virtual int extractFeature(struct FileInfo pf)=0;
    
    /**
    功能：
        匹配特征
    参数：
        pf[in]，待匹配的文件信息
    返回值：
        空，没有匹配成功；否则，返回结果数据结构
    **/
    virtual ScanResult matchFeature(struct FileInfo pf)=0;
    
protected:
    string libPath;
};

#endif