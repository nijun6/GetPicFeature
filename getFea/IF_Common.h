#ifndef _IF_COMMON_H_
#define _IF_COMMON_H_

#include <iostream>
using namespace std;

typedef enum
{
    FILE_TYPE_UNKONE=-1,    //未知格式
    FILE_TYPE_ENC=0,        //加密格式
    FILE_TYPE_HTML=1,       //HTML格式
    FILE_TYPE_DOC=2,        //DOC格式
    FILE_TYPE_PDF=7,        //PDF格式
    FILE_TYPE_RTF=9,        //RTF格式
    FILE_TYPE_ZIP=12,       //ZIP格式
    FILE_TYPE_RAR=13,       //RAR格式
    FILE_TYPE_BMP=14,       //BMP格式
    FILE_TYPE_JPEG=16,      //JPG格式
    FILE_TYPE_DOCX=27,      //DOCX格式
    FILE_TYPE_7Z=32,        //7Z格式
    FILE_TYPE_PNG=33,       //PNG格式
    FILE_TYPE_TIFF=34,      //TIFF格式
    FILE_TYPE_WPS_DOC=37,   //WPS格式
}FILE_TYPE;

struct FileInfo{
    unsigned char ID[128];      //原始文件的ID
    string fileName;            //提取出来的子文件名
    unsigned char *plainContent;//提取出来的内容缓冲区
    int length;                 //plainContent的长度
    int plainType;              //提取出来的格式：文本扫描只能是TXT；图片扫描只能是图片格式
};

struct ScanResult{
    unsigned char ID_File[128]; //被检测的原始文件的ID
    string fileName;            //被检测的子文件名
    unsigned char ID_Lib[128];  //匹配到的文件的ID
	float distance;
    int engine;                 //匹配模块标识，1为文本，2为图片
};

struct MetaInfo{
    unsigned char ID[128];  //原始文件的ID
    //string identify;
    //***
    //***
};

#endif