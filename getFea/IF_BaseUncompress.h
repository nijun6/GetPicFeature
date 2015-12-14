#ifndef _IF_BASEUNCOMPRESS_H_
#define _IF_BASEUNCOMPRESS_H_

#include "IF_Common.h"

class BaseUncompress{
public:
    /**
        功能：
            构造函数
        参数：
            无
        返回值：
            无
    **/
    BaseUncompress(){};
    /**
        功能：
            析构函数
        参数：
            无
        返回值：
            无
    **/
    ~BaseUncompress(){};
    
    /**
    功能：
        将文本类型的文档生成图片
    参数：
        pf[in]，待打印的文件结构体
    返回值：
        空，生成失败；否则，生成的图片内容列表
    **/
    virtual vector<FileInfo> uncompress(struct FileInfo pf)=0;
private:
    /**
    
    **/
};

#endif