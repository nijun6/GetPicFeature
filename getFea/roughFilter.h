#include "allheaders.h"
#include "IF_BaseScan.h"
#include "IF_PicScan.h"
#include "util.h"	
#include "allheaders.h"

class RoughFilter{
private:
	const static int filte_width = 200;
	const static int filte_height = 300;
	const static int min_variance = 2e5;
public:
	IplImage* filte(IplImage* img) {
		if (img->width < filte_width || img->height < filte_height) {
			return NULL;
		}
		if (img->depth != IPL_DEPTH_8U)
			return NULL;
		#define S(image,x,y) ((uchar*)(image->imageData + image->widthStep*(y)))[(x)]
		int histo[256] = {};
		for (int i = 0; i < img->height; i++) {
			for (int j = 0; j < img->width; j++) {
				histo[S(img,j,i)]++;
			}
		}

		if (variance(histo, 256) < min_variance) 
			return NULL;
	}

	double variance(int* a, int len) {
		int sum = 0;
		for (int i = 0; i < len; i++) {
			sum += a[i];
		}
		double ave = sum*1.0/len;
		double var = 0;
		for (int i = 0; i < len; i++) {
			var += (a[i] - ave) * (a[i] - ave);
		}
		return var / len;
	}
};