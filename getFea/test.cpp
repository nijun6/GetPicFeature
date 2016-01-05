#ifdef WIN32

#include "IF_PicScan.h"
#include "algorithm"
#include "io.h"

bool GetAllFilesList(string file_path,
	vector<string> &files_list, 
	vector<string> &folders_list);

void getFiles( string path, vector<string>& files );

bool getFileInfo(char* filename, FileInfo& fileInfo);

void test333() {
	Pix* p = pixCreate(100,100,8);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++)
			pixSetPixel(p, i,j,128);
	}
	pixWrite("pixcreate.jpg", p, IFF_JFIF_JPEG);
}

void testExtract() {
	IF_PicScan feaGet(".\\picLib");
	vector<string> fileslist;
	getFiles("D:\\NJ\\pictures\\libpic", fileslist);
	sort(fileslist.begin(), fileslist.end());
	for (int i = 0; i < fileslist.size(); i++) {
		FileInfo fi;
		if (getFileInfo((char*)fileslist[i].c_str(), fi)) {
			feaGet.extractFeature(fi);
			feaGet.dumpFeature(fi);
		}
	}
}

void testSearch() {
	IF_PicScan feaGet(".\\picLib");
	vector<string> fileslist;
	getFiles("D:\\NJ\\pictures\\libpic", fileslist);
	sort(fileslist.begin(), fileslist.end());
	for (int i = 0; i < fileslist.size(); i++) {
		FileInfo fi;
		if (getFileInfo((char*)fileslist[i].c_str(), fi)) {
			feaGet.extractFeature(fi);
			ScanResult sr;
			feaGet.searchFea(feaGet.fea, sr, fi);
			cout << "search result: " << sr.distance << endl;
		}
	}
}

int main(int argc, char** argv) {
	//test333();
	testExtract();
	//testSearch();

	return 0;
}

bool getFileInfo(char* filename, FileInfo& fileInfo) {
	string file_name = filename;
	for (int i = 0; i < file_name.size(); i++) {
		if (file_name[i] == ' ')
			file_name[i] = '-';
	}
	rename(filename, file_name.c_str());
	filename = (char*)file_name.c_str();
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "can not open the file");
		return false;
	}
	fileInfo.fileName = filename;
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	if (len < 128) {
		fprintf(stderr, "the file is too small\n");
		return false;
	}
	fileInfo.plainContent = new unsigned char[len];
	fileInfo.length = len;
	fseek(fp, 0, SEEK_SET);
	fread(fileInfo.plainContent, 1024*8, len/1024/8+1, fp);
	fclose(fp);
	int id[32];
	for (int i = 0; i < 32; i++) {
		id[i] = (int)(fileInfo.plainContent + i*4);
	}
	memcpy(fileInfo.ID, id, 128);
	fileInfo.plainType = 1;
	return true;
}

void getFiles( string path, vector<string>& files )
{
	//文件句柄
	long   hFile   =   0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include "util.h"

using namespace std;

int readFileList(char *basePath, vector<string>& files)
{
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir=opendir(basePath)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr=readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
			continue;
		else if(ptr->d_type == 8) {    ///file
			string dir = basePath;
			files.push_back(dir+"/"+ptr->d_name);
		}
		else if(ptr->d_type == 10) {    ///link file
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
		}
		else if(ptr->d_type == 4)    ///dir
		{
			memset(base,'\0',sizeof(base));
			strcpy(base,basePath);
			strcat(base,"/");
			strcat(base,ptr->d_name);
			vector<string> subfiles;
			readFileList(base, subfiles);
			for (int i = 0; i < subfiles.size(); i++)
				files.push_back(subfiles[i]);
		}
	}
	closedir(dir);
	return 1;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "too less parameters\n");
		return -1;
	}
	IplImage* cvimg = cvLoadImage(argv[1]);
	IplImage* temp = cvCreateImage(cvimg->width, cvimg->height, IPL_);
	//Pix* pix = Util::getPixFromImage(cvimg);

	return 0;
}

#endif
