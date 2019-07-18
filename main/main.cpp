/*
bmp文件转yuv420文件程序
*/

#include <opencv2/opencv.hpp>  
#include <stdio.h>  
#include <fstream>  
#include <windows.h>  
using namespace std;
using namespace cv;

void bmp2yuv420(Mat bmp, char *file_name);
void yuv4202bmp(const char* yuvname, int nHeight, int nWidth);

#define BOOT_CMD 1  

int main(int argc, char* argv[])
{
#ifdef BOOT_CMD  
	if (argc == 3)
	{
		if (!strcmp(&argv[1][strlen(argv[1] - 4)], ".bmp"))
		{
			Mat bmp = imread(argv[1]);
			int yuvRows = bmp.rows * 3 / 2;
			int yuvCols = bmp.cols;
			int yuvSize = yuvRows*yuvCols;
			Mat yuvmat(yuvRows, yuvCols, CV_8UC1);
			const int max_path = 200;
			char filename[max_path] = "";
			strcat(filename, "d:\\bmp2yuv420\\");
			strcat(filename, argv[2]);
			bmp2yuv420(bmp, argv[2]);
			printf("convert bmp to yuv succ\n");
		}
	}

	if (argc == 1)
	{
		char file_in[100];
		char file_out[100];
		printf("input bmp file name: ");
		scanf("%s", file_in);
		printf("input yuv file name: ");
		scanf("%s", file_out);
		Mat bmp = imread(file_in);
		bmp2yuv420(bmp, file_out);
		printf("convert bmp to yuv succ\n");
	}
#else  

	Mat bmp = imread("bmp.bmp");
	imshow("bmp file", bmp);
	bmp2yuv420(bmp, "yuv.yuv");
#endif  
	//等待按键  
	waitKey();
	system("pause");

	return 1;
}

void bmp2yuv420(Mat bmp, char *file_name)
{
	if (bmp.empty()) return;
	int nHeight = bmp.rows;
	int nWidth = bmp.cols;

	//Mat rmat(nHeight, nWidth, CV_8UC1);  
	//Mat gmat(nHeight, nWidth, CV_8UC1);  
	//Mat bmat(nHeight, nWidth, CV_8UC1);  

	Mat ymat(nHeight, nWidth, CV_8UC1);
	Mat umat(nHeight / 2, nWidth / 2, CV_8UC1);
	Mat vmat(nHeight / 2, nWidth / 2, CV_8UC1);
	Mat yuvmat(nHeight, nWidth, CV_8UC3);

	//unsigned char r,g,b;  
	unsigned char y, u, v;

	cvtColor(bmp, yuvmat, CV_RGB2YCrCb);

	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			//b = bmp.at<Vec3b>(i,j)[0];  
			//g = bmp.at<Vec3b>(i,j)[1];  
			//r = bmp.at<Vec3b>(i,j)[2];  

			//rmat.at<unsigned char>(i,j) = r;  
			//gmat.at<unsigned char>(i,j) = g;  
			//bmat.at<unsigned char>(i,j) = b;  

			y = yuvmat.at<Vec3b>(i, j)[0];
			ymat.at<unsigned char>(Point(j, i)) = y;
			if ((i % 2 == 0) && (j % 2 == 0))
			{
				u = yuvmat.at<Vec3b>(i, j)[1];
				v = yuvmat.at<Vec3b>(i, j)[2];
				umat.at<unsigned char>(Point(j / 2, i / 2)) = u;
				vmat.at<unsigned char>(Point(j / 2, i / 2)) = v;
			}
		}
	}

	//显式  
	//imshow("bmp2yuv420_rgb", bmp);  
	//imshow("bmp2yuv420_r", rmat);  
	//imshow("bmp2yuv420_g", gmat);  
	//imshow("bmp2yuv420_b", gmat);  

	//保存  
	//imwrite("bmp2yuv420_rgb.bmp",bmp);  
	//imwrite("bmp2yuv420_r.bmp",rmat);  
	//imwrite("bmp2yuv420_g.bmp",gmat);  
	//imwrite("bmp2yuv420_b.bmp",bmat);  

	//显式  
	//imshow("bmp2yuv420_y", ymat);  
	//imshow("bmp2yuv420_u", umat);  
	//imshow("bmp2yuv420_v", vmat);  

	//保存  
	//imwrite("bmp2yuv420_y.bmp", ymat);  
	//imwrite("bmp2yuv420_u.bmp", umat);  
	//imwrite("bmp2yuv420_v.bmp", vmat);  

	int ySize = nWidth * nHeight;
	int uSize = ySize / 4;
	int vSize = ySize / 4;

	//const char* yuvname = "d:\yuv.yuv";  
	const char* yuvname = file_name;
	fstream bitstreamFile(yuvname, ios::binary | ios::out);
	if (bitstreamFile.fail()) printf("open fail\n");

	bitstreamFile.write((char*)(ymat.data), ySize);
	if (bitstreamFile.fail() || bitstreamFile.eof()) printf("wirte fail\n");
	bitstreamFile.write((char*)(umat.data), uSize);
	if (bitstreamFile.fail() || bitstreamFile.eof()) printf("wirte fail\n");
	bitstreamFile.write((char*)(vmat.data), vSize);
	if (bitstreamFile.fail() || bitstreamFile.eof()) printf("wirte fail\n");
	printf("write yuv file succ\n");
}