#include "allheaders.h"
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;

void test_pix() {
	Pix* pix = pixRead("1070100120140416140020085.jpg");
	if (pix == 0) {
		fprintf(stderr, "error");
		return;
	}
	for (int i = 0; i < pix->h && i<30; i++) {
		for (int j = 0; j < pix->w && j < 30; j++) {
			unsigned int v;
			pixGetPixel(pix, i,j,&v);
			printf("%u ", v);
		}
		printf("\n");
	}
	printf("------------------------\n");
	Pix* bpix = pixConvertTo8(pix, 0);
	for (int i = 0; i < pix->h && i<30; i++) {
		for (int j = 0; j < pix->w && j < 30; j++) {
			unsigned int v;
			pixGetPixel(bpix, i,j,&v);
			printf("%u ", v);
		}
		printf("\n");
	}
}

int main_pixtest(int argc, char** argv) {
	test_pix();
	char file[256];
	while (true) {
		scanf("%s", file);
		Pix* pix = pixRead(file);
		if (pix == 0) {
			fprintf(stderr, "Can not open the image\n");
			return -1;
		}
		int w, h, d;
		pixGetDimensions(pix, &w, &h, &d);
		printf("w = %d, h = %d, d = %d\n", w, h, d);
		Pix* bpix = pixConvertTo8(pix, 0);
		pixWrite("test.jpg", bpix, IFF_JFIF_JPEG);
		pixDestroy(&bpix);
		pixDestroy(&pix);
	}
	return 0;
}
