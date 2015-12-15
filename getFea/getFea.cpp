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
	pix = pixCreateHeader(img.size().width, img.size().height, image->depth);
	pix->data = (l_uint32*)img.data;
}

/*
 * function: get vertical project feature
 * pix: input picture
 * words: words position
 * scale_factor: the factor of the normalized
 * word_len: the size compress to of the original word
 *
 * return value: the code of all the words
 * */
string verticalProjectFea(Pix* pix, Boxa* reginos, int scale_factor, int word_len) {
	if (reginos->n <= 0) {
		return "";
	}
	string res;
	Pix* p = pixConvertTo8(pix, 0);
	Box* b = boxaGetBox(reginos, 0, L_CLONE);
	int lastx = b->x;
	for (int i = 0; i < reginos->n; i++) {
		Box* b = boxaGetBox(reginos, i, L_CLONE);
		int*col = new int[b->w];
		memset(col, 0, sizeof(int)*b->w);
		int sum = 1;
		for (int x = b->x+1; x < b->x + b->w-1; x++) {
			for (int y = b->y+1; y < b->y + b->h; y++) {
				l_uint32 v;
				pixGetPixel(p, x, y, &v);
				//printf("x = %d, y = %d, v = %d, index = %d\n", x, y, v, x - b->x -1);
				col[x - b->x  - 1] += 255 - v;
				sum += 255 - v;
			}
		}
		for (int j = 0; j < b->w-2; j++) {
			res += Util::codeToText(scale_factor*col[j]/sum);
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

/*
 *
 * */
void getCandidatePos(int** p, int size, int d, float min_scale, vector<int>& out) {
	int* pj = new int[size];
	memset(pj, 0, sizeof(int)*size);

	if (d == 1) {
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				pj[x] += p[x][y];
			}
		}
	} else if (d == 2) {
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				pj[y] += p[x][y];
			}
		}
	}
	int pre_height = 0;
	int up = 0, down = 0, up_i;
	for (int i = 0; i < size; i++) {
		if (pj[i] - pre_height >= min_scale*size) {
			up = 1;
			up_i = i;
		}
		if (pre_height - pj[i] >= min_scale*size
				&& i - up_i < min_scale*size
				&& up_i == 1) {
			down = 1;
		}
		if (i - up_i >= min_scale*size) {
			up = 0;
		}
		if (up && down) {
			out.push_back((up_i + i)/2);
			up = down = 0;
		}
		pre_height = pj[i];
	}
	delete[] pj;
}
/*
 * function: get lines number in the word
 * pix: input picture
 * word: position of the word
 * dir:direction of line want to get
 * 		1 : wants to get vertical lines number
 * 		2 : wants to get horizontal lines number
 *
 * return value: the lines number on that direction:
 * */
int getWordLine(Pix* pix, Box* word, int dir, float min_scale) {
	int size = word->h > word->w ? word->h : word->w;
	int** p = new int*[size];
	for (int i = 0; i < size; i++) {
		p[i] = new int[size];
		for (int j = 0; j < size; j++) {
			unsigned int v;
			if (i < word->w && j < word->h) {
				pixGetPixel(pix, word->x+j, word->y+i, &v);
				p[i][j] = v;
			}
		}
	}

	vector<int> candidate_word_pos;
	getCandidatePos(p, size, dir, min_scale, candidate_word_pos);

	for (int i = 0; i < size; i++) {
		delete[] p[i];
	}
	delete[] p;

	return candidate_word_pos.size();
}

string wordsFea(Pix* pix, Boxa* words, float min_scale) {
	if (pix == NULL || words == NULL || words->n <= 0)
		return "error:words is null or words->n is 0";
	Pix* bpix = pixConvertTo1(pix, 130);
	string res;
	for (int i = 0; i < words->n; i++) {
		res += Util::codeToText(getWordLine(bpix, boxaGetBox(words, i , L_CLONE), 0, min_scale));
		res += Util::codeToText(getWordLine(bpix, boxaGetBox(words, i , L_CLONE), 1, min_scale));
		res += " ";
	}
	pixDestroy(&bpix);
	return res;
}

void test() {
	Pix* p = pixRead("/home/nijun/OCR_PIC/20公文/7.png");
	Pix* bp = pixConvertTo1(p,255);
	pixWrite("/home/nijun/OCR_PIC/20公文/7b.jpg", bp, IFF_JFIF_JPEG);
	pixDestroy(&p);
	pixDestroy(&bp);
	getchar();
}

void testWriteBoxa(Pix*p, Boxa* words) {
	Pix* pw = pixDrawBoxa(p, words, 1, 0x00ff0000);
	pixWrite("withboxs.jpg", pw, IFF_JFIF_JPEG);
	printf("write over.");
	getchar();
}

int main(int argc, char** argv) {
	//test();
	if (argc < 2) {
		fprintf(stderr, "wrong arguments\n");
		return -1;
	}
	Pix* pix = NULL;
	IplImage* image = NULL;
	cv::Mat mat;
//	open(argv[1], pix, mat);
//	cout << pix->w << " " << pix->h << endl;
	pix = pixRead(argv[1]);
	Boxa* lines = NULL, *words = NULL;
	int scale_factor = atoi(argv[2]);
	TST tst;
	tst.getLineAndWordsLayout(pix, lines, words);
	testWriteBoxa(pix, words);
	//outPutLineAndWordsLayot(lines, words, 10);
	cout << "verticalProjectFea:" << verticalProjectFea(pix, words, scale_factor, 10) << endl;
	cout << "wordsFea:" << wordsFea(pix, words, 0.3) << endl;
	pixDestroy(&pix);
	boxaDestroy(&lines);
	boxaDestroy(&words);
	return 0;
}
