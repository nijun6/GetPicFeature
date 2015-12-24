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

