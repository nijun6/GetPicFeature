#include "allheaders.h"
#include "IF_BaseScan.h"
#include "IF_PicScan.h"
#include "util.h"	
#include "highgui.h"
#include "allheaders.h"
#include "roughFilter.h"

IF_PicScan::IF_PicScan(string libPath): BaseScan(libPath) {
	cout << "libpath = " << this->libPath << endl;
}

IF_PicScan::~IF_PicScan() {

}

void cvText(IplImage* img, const char* text, int x, int y)
{
	CvFont font;

	double hscale = 0.5;
	double vscale = 0.5;
	int linewidth = 1;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX |CV_FONT_ITALIC,hscale,vscale,0,linewidth);

	CvScalar textColor =cvScalar(0,255,255);
	CvPoint textPos =cvPoint(x, y);

	cvPutText(img, text, textPos, &font,textColor);
}

int IF_PicScan::extractFeature(struct FileInfo pf) {
	IplImage *temp = cvLoadImage((const char*)(pf.fileName.c_str()));
	if (temp == NULL) {
		fprintf(stderr, "can not open the image");
		return 0;
	}
	cvimg = cvCreateImage(cvGetSize(temp), IPL_DEPTH_8U, 1);
	cvCvtColor(temp, cvimg, CV_RGB2GRAY);
	cvReleaseImage(&temp);
	
	img = Util::getPixFromIplImage(cvimg);
	tst.getLineAndWordsLayout(img, lines, words);

#ifdef DEBUG
	cvSaveImage("cvimage.jpg", cvimg);
	pixWrite("pixtest.jpg", img, IFF_JFIF_JPEG);
	Pix* pline = pixDrawBoxa(img, lines, 1, 0x00ff0000);
	pixWrite("line.jpg", pline, IFF_JFIF_JPEG);
	Pix* pword = pixDrawBoxa(img, words, 1, 0x00ff0000);
	pixWrite("word.jpg", pword, IFF_JFIF_JPEG);
	pixDestroy(&pline);
	pixDestroy(&pword);
	for (int i = 0; i < words->n; i++) {
		char str[20];
		sprintf(str, "%d", i);
		Box* b = boxaGetBox(words, i, L_CLONE);
		cvText(cvimg, str, b->x, b->y);
	}
	cvSaveImage("mardwords.jpg", cvimg);
#endif

	if (lines == NULL || words == NULL) {
		fprintf(stderr, "Can not get words and lines position");
		return 0;
	}

	fea = "filename:" + pf.fileName + "\n"
				+"wordcode:" + codeWord(words) + "\n"
				+"sentencecode:" + codeSentence(words) + "\n"
				+"linecode:" + codeLine(lines) + "\n"
				+"project_feature:" + verticalProjectFea(img, lines, 10, 20);
	
	cvReleaseImage(&cvimg);
	pixDestroy(&img);
	return 1;
}

bool IF_PicScan::dumpFeature(const FileInfo& fileinfo) {
	string name;
	Util::converPaht2Filename(name, Util::path_separtor+fileinfo.fileName+".ID");
	string idfile = libPath + Util::path_separtor + name;

	FILE* fp = fopen(idfile.c_str(), "wb");
	if (fp == NULL) {
		fprintf(stderr, "can not open the ID file");
	}
	fwrite(fileinfo.ID, sizeof(fileinfo.ID), 1, fp);
	fclose(fp);
	Util::converPaht2Filename(name, fileinfo.fileName);
	string filename = libPath + Util::path_separtor + name + ".txt";

	fp = fopen(filename.c_str(), "w");
	if (fp == NULL) {
		fprintf(stderr, "can not open the feature file");
		return false;
	}
	fprintf(fp, "%s", fea.c_str());
	fclose(fp);
	system(("java -jar PicRetrive.jar dumpfea idfile="+idfile+" featurefile=" + filename).c_str());
	return true;
}

bool IF_PicScan::searchFea(const string& fea, ScanResult& sr, const FileInfo& fileinfo) {
	FILE* fp = fopen("picfeature", "w");
	fprintf(fp, "%s", fea.c_str());
	fclose(fp);
	system("java -jar PicRetrive.jar search featurefile=picfeature lib=.\\libPic\\index");
	Util::readRes("id", "picfeature", sr.ID_Lib, sr.distance);
	return true;
}

ScanResult IF_PicScan::matchFeature(struct FileInfo pf) {
	ScanResult sr;
	IplImage *temp = cvLoadImage((const char*)(pf.fileName.c_str()));
	if (temp == NULL) {
		fprintf(stderr, "can not open the image");
		sr.distance = Util::INF;
		return sr;
	}
	cvimg = cvCreateImage(cvGetSize(temp), IPL_DEPTH_8U, 1);
	cvCvtColor(temp, cvimg, CV_RGB2GRAY);
	cvReleaseImage(&temp);

	img = Util::getPixFromIplImage(cvimg);
	IplImage* pimg = roughFilter(cvimg);
	if (pimg == NULL) {
		sr.distance = Util::INF;
		return sr;
	}

	extractFeature(pf);
	searchFea(fea, sr, pf);

	cvReleaseImage(&cvimg);
	cvReleaseImage(&pimg);
	pixDestroy(&img);
	return sr;
}

IplImage* IF_PicScan::roughFilter(IplImage* img) {
	RoughFilter rf;
	return rf.filte(img);
}

/*
	code the word positions
*/
string IF_PicScan::codeWord(Boxa* words) {
	if (words->n <= 0) {
		return "";
	}
	Box* last = boxaGetBox(words, 0, L_CLONE);
	string wordscode;
	int lcnt = 0;
	for (int i = 1; i < words->n; i++) {
		Box* b = boxaGetBox(words, i, L_CLONE);
		lcnt++;
		if (b->x > last->x + last->h*(1.3) 
			|| b->y < last->y - last->h * 0.3 
			|| b->x < last->x 
			|| i == words->n - 1) {
			wordscode += Util::codeToText(lcnt);
			lcnt = 0;
			if (b->x < last->x)
				lcnt++;
		}
		last = boxaGetBox(words, i, L_CLONE);
	}
	return wordscode;
}

int IF_PicScan::averWordsGap(Boxa* words) {
	Box* last = boxaGetBox(words, 0, L_CLONE);
	int ave_words_gap = 0;
	int gap_cnt = 0;

	//suppose the first word is a normal word not a punctuation
	int word_height = last->h;
	for (int i = 1; i < words->n; i++) {
		Box* b = boxaGetBox(words, i, L_CLONE);
		if (last == NULL) {
			last = b;
			continue;
		}

		if (b->x < last->x) {
			last = b;
			continue;
		}

		if (b->h < max_punctuation_height*word_height) {
			last = NULL;
			continue;
		}

		if (last->w > min_word_width*last->h || b->w > min_word_width*b->h) {
			ave_words_gap += b->x - last->w;
			gap_cnt++;
		}
	}
	return ave_words_gap/gap_cnt;
}

/*
	code the sentence 
*/
string IF_PicScan::codeSentence(Boxa* words) {
	if (words->n == 0) {
		return "";
	}
	string res;

	Box* last = boxaGetBox(words, 0, L_CLONE);
	int cnt = 1;
	for (int i = 1; i < words->n; i++) {
		Box* b = boxaGetBox(words, i, L_CLONE);
		if (b->x < last->x  || b->x - last->x - last->w > b->h*max_word_gap || i == words->n-1) {
			res += Util::codeToText(cnt);
			cnt = 1;
		} else {
			cnt++;
		}
		last = b;
	}
	return res;
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

/*
	get the projection feature of the regions
	input: 
		Pix* pix        input image
		Boxa* regions   regions to extract feature
		int scale_factor [1,25]
		int word_height  :destination reginon height

	return: the code represents the projection fea
*/
string IF_PicScan::verticalProjectFea(Pix* pix, Boxa* reginos, int scale_factor, int word_height) {
	if (reginos->n == 0) {
		return "";
	}
	string res;
	for (int i = 0; i < reginos->n; i++) {
		Box* b = boxaGetBox(reginos, i, L_CLONE);
		Pix* region = Util::getRange(pix, b);
		float scale = 1.0*word_height/region->h;
		Pix* pscl = pixScale(region, scale, scale);
		int col_project[max_img_width] = {};
		unsigned int v;
		for (int y = 0; y < pscl->h; y++) {
			for (int x = 0; x < pscl->w; x++) {
				pixGetPixel(pscl, x, y, &v);
				col_project[x] += 255 - v;
			}
		}
		
		string tmp = Util::codeToText(col_project[0]/255);
		int last = col_project[0];
		for (int c = 1; c < pscl->w; c++) {
			if (last == 0)
				last++;
			double t = 1.0*col_project[c]/last;
			if (t > 1.0)
				t = 1.0/t;
			tmp += Util::codeToText(scale_factor*t);
			last = col_project[c];
		}
		pixDestroy(&pscl);
		if (i != reginos->n - i)
			res += tmp + " ";
	}

	return res;
}