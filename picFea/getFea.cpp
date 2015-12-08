#include <iostream>
#include "allheaders.h"
#include "tst.h"
#include "cv.h"
#include "string.h"
#include "highgui.h"
#include "opencv2/opencv.hpp"
#include "util.h"

using namespace std;

string codeLine(Boxa* lines) {
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

string codeWords(Boxa* words) {
	if (words->n <= 0) {
		return "";
	}
	Box* last = boxaGetBox(words, 0, L_CLONE);
	string wordscode;
	int lcnt = 0;
	for (int i = 1; i < words->n; i++) {
		Box* b = boxaGetBox(words, i, L_CLONE);
		lcnt++;
		if (b->y < last->y - 0.3*last->h || i == words->n - 1) {
			wordscode += Util::codeToText(lcnt);
			lcnt = 0;
			if (b->x < last->x)
				lcnt++;
		}
		last = boxaGetBox(words, i, L_CLONE);
	}
	return wordscode;
}

void outPutLineAndWordsLayot(Boxa* lines, Boxa* words, int scale_factor){
	if (lines->n <= 0 || words->n <= 0)
		return ;
	cout << "lines:" << endl;
	for (int i = 0; i < lines->n; i++) {
		Box* b = boxaGetBox(lines, i, L_CLONE);
		printf("%d %d %d %d\n", b->x, b->y, b->w, b->h);
	}
	cout << "words:" << endl;
	for (int i = 0; i < words->n; i++) {
		Box* b = boxaGetBox(words, i, L_CLONE);
		printf("%d %d %d %d\n", b->x, b->y, b->w, b->h);
	}

	cout << "lines_code:" + codeLine(lines) << endl;
	cout << "words_code:" + codeWords(words) << endl;
}


void open(char* file, Pix*& pix, IplImage*& image) {
	image = cvLoadImage(file);
	if (pix) {
		pixDestroy(&pix);
	}
	pix = pixCreateHeader(image->width, image->height, image->depth);
	pix->data = (l_uint32*)new char[image->widthStep*image->height];
	memcpy(pix->data, image->imageData, image->widthStep*image->height);
}

void open(char* file, Pix*& pix, cv::Mat& img) {
	IplImage* image = cvLoadImage(file);
	if (image == NULL) {;
		pix = NULL;
		return ;
	}
	img = cv::imread(file);
	cout << img.size() << endl;
	cout << img.depth() << endl;
	pix = pixCreateHeader(img.size().width, img.size().height, image->depth);
	pix->data = (l_uint32*)img.data;
}

/*
 *
 * vertical project feature with scale the pic to word_len
 *
 *
 * */
string verticalProjectFeaScale(Pix* pix, Boxa* words, int scale_factor, int word_len) {
	if (words->n <= 0) {
		return "";
	}

	int max_h;
	int i = 0, j;
	while (i < words->n) {
		Box* b = boxaGetBox(words, i, L_CLONE);
		int lastx = b->x;
		max_h = b->h;
		j = i + 1;
		while (j < words->n) {
			Box* jb = boxaGetBox(words, j, L_CLONE);
			if (jb->x < lastx)
				break;
			lastx = jb->x;
			if (jb->h > max_h)
				max_h = jb->h;
			j++;
		}

		while (i < j) {

			Pix* p = pixScale()
		}
	}
}

/*
 * vertical project feature
 *
 * */
string verticalProjectFea(Pix* pix, Boxa* words, int scale_factor, int word_len) {
	if (words->n <= 0) {
		return "";
	}
	string res;
	Pix* p = pixConvertTo8(pix, 0);
	Box* b = boxaGetBox(words, 0, L_CLONE);
	int lastx = b->x;
	for (int i = 0; i < words->n; i++) {
		Box* b = boxaGetBox(words, i, L_CLONE);
		int*col = new int[b->w];
		memset(col, 0, sizeof(int)*b->w);
		int sum = 1;
		for (int x = b->x+1; x < b->x + b->w; x++) {
			for (int y = b->y+1; y < b->y + b->h; y++) {
				l_uint32 v;
				pixGetPixel(p, x, y, &v);
				//printf("x = %d, y = %d, v = %d, index = %d\n", x, y, v, x - b->x -1);
				col[x - b->x  - 1] += 255 - v;
				sum += 255 - v;
			}
		}
		int cl[100] = {};
		double step = (b->w - 2) / word_len;
		double p = step;
		int cnt = 0;
		for (int j = 0; j < b->w - 2; j++) {
			if (j > p) {
				cl[cnt] += col[j] + (col[j] - col[j-1])*(p - int(p));
				cl[cnt+1]+= col[j] + (col[j] - col[j-1])*(1 - p + int(p));
				p += step;
				cnt++;
			}
			cl[cnt] += col[j];
		}
		for (int j = 0; j < word_len; j++) {
			res += Util::codeToText(scale_factor*cl[j]/sum);
		}
		if (b->x < lastx)
			res += '\n';
		else
			res += ' ';
		lastx = b->x;
		delete[] col;
	}
	pixDestroy(&p);
	return res;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "wrong arguments\n");
		return -1;
	}
	Pix* pix = NULL;
	IplImage* image = NULL;
	//cv::Mat mat;
	//open(argv[1], pix, mat);
	//cout << pix->w << " " << pix->h << endl;
	pix = pixRead(argv[1]);
	Boxa* lines = NULL, *words = NULL;
	int scale_factor = atoi(argv[2]);
	TST tst;
	tst.getLineAndWordsLayout(pix, lines, words);
	//outPutLineAndWordsLayot(lines, words, 10);
	cout << "verticalProjectFea:" << verticalProjectFea(pix, words, scale_factor, 10) << endl;
	pixDestroy(&pix);
	boxaDestroy(&lines);
	boxaDestroy(&words);
	return 0;
}
