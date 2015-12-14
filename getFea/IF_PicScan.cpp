#include "allheaders.h"
#include "IF_BaseScan.h"
#include "IF_PicScan.h"
#include "util.h"	

IF_PicScan::IF_PicScan(string libPath): BaseScan(libPath) {
	cout << "libpath = " << this->libPath << endl;
}

int IF_PicScan::extractFeature(struct FileInfo pf) {
	IplImage* cvimg;
	Util::readImageFromMemory(pf.plainContent, pf.length, cvimg);
	return 1;
}

ScanResult IF_PicScan::matchFeature(struct FileInfo pf) {
	ScanResult sr;
	return sr;
}

string IF_PicScan::codeLine(Boxa* lines) {
	if (lines->n <= 0) {
		return "";
	}
	Box* last = boxaGetBox(lines, 0, L_CLONE);
	vector<string> codeline;
	for (int i = 0; i < lines->n; i++) {
		Box* b = boxaGetBox(lines, i, L_CLONE);
		string code = Util::codeToText((b->x-last->x)/b->h)
					+ Util::codeToText((b->y-last->y)/b->h)
					+ Util::codeToText(b->w/b->h);
		last = b;
		codeline.push_back(code);
	}
	string lastline;
	int last_cnt = 1;
	string lineres;
	for (unsigned int i = 0; i < codeline.size(); i++) {
		if (lastline == codeline[i] && i != codeline.size() - 1) {
			last_cnt++;
		} else {
			lineres += codeline[i];
			lineres += last_cnt > 1 ? Util::codeToText(last_cnt) + " " : " ";
			lastline = codeline[i];
			last_cnt = 1;
		}
	}
	return lineres;
}

string IF_PicScan::codeWord(Boxa* words) {

}
string IF_PicScan::verticalProjectFea(Pix* pix, Boxa* reginos, int scale_factor, int word_len) {

}