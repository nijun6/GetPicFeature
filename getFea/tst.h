#ifndef __TST__
#define __TST__
#include <stdio.h>
#include "allheaders.h"
#include <iostream>
#include <string>

class TST {
	public:
	TST();
	void getLineAndWordsLayout(Pix* pix, Boxa*& lines, Boxa*& words);
};
#endif
