#include "IF_BaseScan.h"
#include "tst.h"
#include "cv.h"

class IF_PicScan : public BaseScan {
	TST tst;
	IplImage* cvimg;
	Pix* img;
	Boxa* lines;
	Boxa* words;
public:
	IF_PicScan(string libPath);
    int extractFeature(struct FileInfo pf);
    ScanResult matchFeature(struct FileInfo pf);

private:
	string codeLine(Boxa* lines);
	string codeWord(Boxa* words);
	string verticalProjectFea(Pix* pix, Boxa* reginos, int scale_factor, int word_len);
};