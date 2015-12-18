#include <string>
#include <algorithm>
#include "allheaders.h"
#include "cv.h"
#include "FreeImage.h"
#include <math.h>
#include <vector>
#include <map>
#include "util.h"

using namespace std;

namespace Util {
	/*
		code an integer to a string represents by aphabet
			input: an integer
			output: string
	*/
	string codeToText(int n) {
		string s;
		if (n < 0) {
			s += "N";
			n = -n;
		}
		string t;
		do {
			t += 'a' + n%26;
			n /= 26;
		} while (n);
		reverse(t.begin(), t.end());
		return s + t;
	}

	bool readImageFromMemory(unsigned char* bytes, int length, IplImage* &src)
	{
		FreeImage_Initialise();  
		FREE_IMAGE_FORMAT format = FIF_UNKNOWN;  
		FIBITMAP *bitmap = NULL;  
		FIBITMAP *bitmap_temp = NULL;
		FIMULTIBITMAP *bitmap_ = NULL;
		int pages = 0;
		FIMEMORY * memory = FreeImage_OpenMemory(bytes, length);  
		if (!memory) 
		{  
			FreeImage_DeInitialise();
			return false;  
		}  
		format = FreeImage_GetFileTypeFromMemory(memory);
		if (format == -1 || !memory)
		{
			FreeImage_CloseMemory(memory);
			FreeImage_DeInitialise();
			return false;
		}
		else if (format == FIF_TIFF)
		{

			/*这里用参数TIFF_JPEG解压图片，是为了防止其他图片的崩溃情况，
				但这样会导致非JPEG压缩的图片不能正常读取,所以要判断depth，
				一般depth很高，128以上就处理不了，windows下不能正常打开的图片的depth一般在128以上
				所以当depth在128以下时，需要用默认参数0从新打开图片
			*/
			bitmap_temp = FreeImage_LoadFromMemory(format, memory, TIFF_JPEG);
			short depth = FreeImage_GetBPP(bitmap_temp);
			if (depth > 64)
			{
				if(bitmap_temp)
				{
					FreeImage_Unload(bitmap_temp);
				}
				if(memory)
				{
					FreeImage_CloseMemory(memory);
				}
				FreeImage_DeInitialise();
				return false;
			}
			else
			{
				if(bitmap_temp)
				{
					FreeImage_Unload(bitmap_temp);
				}
				if(memory)
				{
					FreeImage_CloseMemory(memory);
				}
				memory = FreeImage_OpenMemory( bytes, length );  
				if (!memory) 
				{  
					FreeImage_DeInitialise();
					return false;  
				}  
				format = FreeImage_GetFileTypeFromMemory(memory);
				if(format == -1 || !memory)
				{
					FreeImage_CloseMemory(memory);
					FreeImage_DeInitialise();
					return false;
				}
				bitmap = FreeImage_LoadFromMemory(format, memory, 0);
			}
			int imageWidth = FreeImage_GetWidth(bitmap);
			int imageHeight = FreeImage_GetHeight(bitmap);
			short channels = 1;
			
			//cout << depth << endl;
			FIBITMAP * bitmap_temp = 0;
			unsigned char * data = 0;
			if(FreeImage_GetImageType(bitmap) != FIT_BITMAP)
			{
				FreeImage_FIFSupportsExportType(format, (FreeImage_GetImageType(bitmap)));
			}
			if (depth < 8) 
			{
				depth = 8;
				if((bitmap_temp = FreeImage_ConvertToGreyscale(bitmap)) != NULL) 
				{
					data = FreeImage_GetBits(bitmap_temp);
				}
			} 
			else 
			{
				assert (depth % 8 == 0);
				channels = depth / 8;
				if(channels >= 4)
				{
					if (channels > 4)    //处理不了
					{
						if(bitmap_temp)
						{
							FreeImage_Unload(bitmap_temp);
						}
						if(bitmap)
						{
							FreeImage_Unload(bitmap);
						}
						if(memory != NULL)
						{
							FreeImage_CloseMemory(memory);
						}
						FreeImage_DeInitialise();
						return false;
					}
					else
					{
						channels = 3;
					}
				}
				depth = 8;
				data = FreeImage_GetBits(bitmap);
			}
			if(data) 
			{
				CvSize size = cvSize((int) imageWidth, (int) imageHeight);
				IplImage* head = cvCreateImageHeader(size, depth, channels);
				cvSetData(head, data, head->widthStep);
				src = cvCreateImage(size, depth, (int) channels);
				cvFlip(head, src, 0);
				cvReleaseImageHeader(&head);
				if(bitmap_temp)
				{
					FreeImage_Unload(bitmap_temp);
				}
				if(bitmap)
				{
					FreeImage_Unload(bitmap);
				}
			}
		}
		else
		{
			bitmap = FreeImage_LoadFromMemory( format, memory, 0); 
			FIBITMAP2IplImage(bitmap,src);
		}
		
		if(memory != NULL)
		{
			FreeImage_CloseMemory(memory);
		}
		FreeImage_DeInitialise();
		return true;
	}

	bool FIBITMAP2IplImage(FIBITMAP *dib, IplImage* &iplImg)  
	{  
		if(!dib)  
			return false;  
		int nClrUsed = FreeImage_GetColorsUsed(dib);  
		int nBpp = FreeImage_GetBPP(dib);  
		int height = FreeImage_GetHeight(dib);  
		int width = FreeImage_GetWidth(dib);  
		RGBQUAD* pPalette = FreeImage_GetPalette(dib);  
		int nChannel = nBpp / 8;   
		if (nChannel < 1)      //有可能为0通道 2015.6.15，加入处理  
		{
			nChannel = 1;
			FIBITMAP * bitmap_temp = FreeImage_ConvertToGreyscale(dib);
			if (bitmap_temp == NULL)
			{
				FreeImage_Unload(dib); 
				return false;
			}
			iplImg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,nChannel);     
			iplImg->origin = 0;  
			for(int y=0;y<height;y++)  
			{  
				unsigned char* pLine = (unsigned char*)iplImg->imageData + y*iplImg->widthStep;  
				unsigned char* psrcLine = (unsigned char*)FreeImage_GetScanLine(bitmap_temp,height-1-y);  
				//cout << psrcLine << endl;
				for (int x = 0;x < nChannel * width /*&& x < strlen( (char *)psrcLine)*/;x++)  
				{  
					*pLine++ = *psrcLine++;  

				}  
			}  
			FreeImage_Unload(bitmap_temp); 
			FreeImage_Unload(dib);  
			//FreeImage_DeInitialise();  
			return true;  
		}
		else if(!nClrUsed && !pPalette || nChannel == 1)      //无调色板图像处理  
		{  
			if (nChannel > 4)    //通道过大，直接变小，只会在颜色有变化，但在后续二值化后无差别 2015.6.15
			{

				nChannel = 4;

			}

			iplImg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,nChannel);     
			iplImg->origin = 0;  
			for(int y=0;y<height;y++)  
			{  
				unsigned char* pLine = (unsigned char*)iplImg->imageData + y*iplImg->widthStep;  
				unsigned char* psrcLine = (unsigned char*)FreeImage_GetScanLine(dib,height-1-y);  
				//cout << psrcLine << endl;
				for (int x = 0;x < nChannel * width /*&& x < strlen( (char *)psrcLine)*/;x++)  
				{  
					*pLine++ = *psrcLine++;  

				}  
			}  

			FreeImage_Unload(dib);  
			//FreeImage_DeInitialise();  
			return true;  
		}  
		else if(pPalette)//索引图像处理  
		{ 
			if(nChannel > 4)
			{
				nChannel = 4;
			}
			iplImg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,nChannel);     
			iplImg->origin = 1;  
			unsigned char intensity;  
			unsigned char* pIntensity = &intensity; 

			for(int y=0;y<height;y++)  
			{  
				unsigned char* pLine = (unsigned char*)iplImg->imageData + y*iplImg->widthStep;  
				for (int x=0;x<width;x++)  
				{  

					FreeImage_GetPixelIndex(dib,x,y,pIntensity);  
					pLine[x*3] = pPalette[intensity].rgbBlue;  
					pLine[x*3+1] = pPalette[intensity].rgbGreen;  
					pLine[x*3+2] = pPalette[intensity].rgbRed;  
				}  
			}  

			FreeImage_Unload(dib);  
			//FreeImage_DeInitialise();  
			return true;  
		}  
		else  
		{  
			FreeImage_Unload(dib);  
			//FreeImage_DeInitialise();  
			return false;  
		}  

	}  

	Pix* getRange(Pix* pix, Box* box) {
		Pix* pr = pixCreate(box->w, box->h, pix->d);
		for (int i = 0; i < box->w; i++) {
			for (int j = 0; j < box->h; j++) {
				unsigned int v;
				pixGetPixel(pix, box->x + i, box->y + j, &v);
				pixSetPixel(pr, i, j, v);
			} 
		}
		return pr;
	}


	Pix* getPixFromIplImage( IplImage* cvimg ) {
		if (cvimg->depth != IPL_DEPTH_8U) {
			fprintf(stderr, "error the input IplImage is not IPL_DEPTH_8U\n");
			return NULL;
		}
		#define S(image,x,y) ((uchar*)(image->imageData + image->widthStep*(y)))[(x)]
		Pix* pix = pixCreate(cvimg->width, cvimg->height, 8);
		for (int x = 0; x < cvimg->width; x++) {
			for (int y = 0; y < cvimg->height; y++) {
				pixSetPixel(pix, x, y, S(cvimg, x, y));
			}
		}
		return pix;
	}

	void converPaht2Filename(string& name, const string& path) {
		name = path;
		for (int i = 0; i < name.size(); i++) {
			if (name[i] == '\\' || name[i] == ':')
				name[i] = '_';
		}
	}

	bool readRes(string idfile, string disfile, unsigned char* id, float& dis) {
		FILE* fp = fopen(idfile.c_str(), "rb");
		if (fp == NULL) {
			return false;
		}
		fread(id, 128, 0, fp);
		fclose(fp);
		fp = fopen(disfile.c_str(), "r");
		if (fp == NULL) {
			return false;
		}
		double d;
		fscanf(fp, "%lf", &d);
		dis = (float)d;
		fclose(fp);
		return true;
	}
}
