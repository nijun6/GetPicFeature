#include "allheaders.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "wrong arguments\n");
		return -1;
	} 
	Pix* pix = pixRead(argv[1]);
	if (pix == 0) {
		fprintf(stderr, "Can not open the image\n");
		return -1;
	}
	int w, h, d;
	pixGetDimensions(pix, &w, &h, &d);
	printf("w = %d, h = %d, d = %d\n", w, h, d);
	Pix* bpix = pixConvertTo8(pix, 160);
	pixWrite(strrchr(argv[1], '/')+1, bpix, IFF_JFIF_JPEG);
	pixDestroy(bpix);
	return 0;
}
