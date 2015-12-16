#include "IF_PicScan.h"

FileInfo &getFileInfo(char* filename, FileInfo& fileInfo) {
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "can not open the file");
		exit(-1);
	}
	fileInfo.fileName = filename;
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fileInfo.plainContent = new unsigned char[len];
	fileInfo.length = len;
	fseek(fp, 0, SEEK_SET);
	fread(fileInfo.plainContent, 1024*8, len/1024/8+1, fp);
	fclose(fp);
	int id[32];
	srand(time(0));
	for (int i = 0; i < 32; i++) {
		id[i] = rand();
	}
	memcpy(fileInfo.ID, id, 128);
	fileInfo.plainType = 1;
	return fileInfo;
}

int main(int argc, char** argv) {
	FileInfo fileinfo;
	IF_PicScan feaGet(".\\picLib");
	feaGet.extractFeature(getFileInfo("filename", fileinfo));

	return 0;
}
