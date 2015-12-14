#ifndef _IF_BASETIKA_H_
#define _IF_BASETIKA_H_

#include "IF_Common.h"

class BaseTika{
public:
    /**
        功能：
            构造函数
        参数：
            无
        返回值：
            无
    **/
    BaseTika(){};
    
    /**
        功能：
            析构函数
        参数：
            无
        返回值：
            无
    **/
    virtual ~BaseTika(){};
    
    /**
    功能：
        文件的格式识别
    参数：
        fileBuff[in]，文件的原始内容
        length[in]，fileBuff长度
    返回值：
        格式编码（-1，未知格式或文件损坏）
    **/
    virtual FILE_TYPE formatRecognize(const unsigned char *fileBuff, const int length)=0;
    
    /**
    功能：
        抽取文本类型的文档中的内容
    参数：
        pf[in]，待匹配的文件信息
    返回值：
        空，抽取失败（格式有问题或者加密文件等）
    **/
    virtual vector<struct FileInfo> extractPlainContent(struct FileInfo)=0;
    
private:
    /**
    
    **/
};

#endif