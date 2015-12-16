#ifndef __utility__
#define __utility__
#include <string>
#include <algorithm>
#include "allheaders.h"
#include "cv.h"
#include "FreeImage.h"
#include <math.h>
#include <vector>
#include <map>

using namespace std;

const int scale_factor_line_layout = 10;

namespace Util {

	/*
	fuction: convert FreeImage format to Iplimage format
	*/
	bool FIBITMAP2IplImage(FIBITMAP *dib, IplImage* &iplImg);

	/*
	功能从内存中读取Iplimage的图像数据
	input：
	bytes: memory pointer
	length: memory size

	out:
	src :  out image
	return：true if sucess else false
	*/
	bool readImageFromMemory(unsigned char* bytes, int length, IplImage* &src);

	/*
		code an integer to a string contains only alphabet
	*/
	string codeToText(int n);

	/*
		fuction: conver an text picture with skew angle to an picture with 0 skew angle
		input: origin text picture with skew angle[-10,10]
		output: text picture with skew angle 0
	*/
	Pix* skewCorrect(Pix* origin);

	Pix* getRange(Pix* pix, Box* box);

	Pix* getPixFromIplImage( IplImage* cvimg );

	bool readRes(string idfile, string disfile, unsigned char* id, float& dis);
}
#endif
