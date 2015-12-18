#ifndef __IF_PicScan__
#define __IF_PicScan__
#include "IF_BaseScan.h"
#include "tst.h"
#include "cv.h"

const int max_img_width = 10000;
const int max_img_height = 10000;

const float max_punctuation_height = 0.3;
const float min_word_width = 0.6;
const float max_word_gap = 0.2;

class IF_PicScan : public BaseScan {
	TST tst;
	IplImage* cvimg;
	Pix* img;
	Boxa* lines;
	Boxa* words;

public:
	string fea;

	IF_PicScan(string libPath);
    int extractFeature(struct FileInfo pf);
    ScanResult matchFeature(struct FileInfo pf);
	bool searchFea(const string& fea, ScanResult& sr, const FileInfo& fileinfo);
	~IF_PicScan();
	bool dumpFeature(const FileInfo& fileinfo);
private:
	IplImage* roughFilter(IplImage* img);
	string codeWord(Boxa* words);
	string codeSentence(Boxa* words);
	string codeLine(Boxa* lines);
	string verticalProjectFea(Pix* pix, Boxa* reginos, int scale_factor, int word_height);

	int averWordsGap(Boxa* words);
};

#endif