#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#include "rasterizer.h"

int height = 500, width = 500;

//This function is used to convert the pixels' rgb value into opencv::Mat
void write_Mat(cv::Mat& img, std::vector<Eigen::Vector3f> frame_buf)
{
	int channels = img.channels(), rows = img.rows, cols = img.cols * channels;
	
	//the Mat may be stored in a one dimension formation
	//if (img.isContinuous())
	//{
	//	cols = rows * cols;
	//	rows = 1;
	//}

	uchar* p;
	int idx = 0;
	for (int i = rows - 1; i >= 0; --i)
	{
		p = img.ptr<uchar>(i);
		for (int j = 0; j < cols; j += 3, idx += 1)
		{
			//the value in Mat is not RGB but BGR
			p[j] = frame_buf[idx].z();
			p[j + 1] = frame_buf[idx].y();
			p[j + 2] = frame_buf[idx].x();
		}
	}
}

int main()
{	

	rasterizer r(height, width);

	while (1)
	{
		r.draw_line({ 100,100 }, { 200,350 });
		
		//initialize the opencv Mat
		cv::Mat img = cv::Mat::ones(height, width, CV_8UC3);

		write_Mat(img, r.get_frame_buf());

		cv::imshow("display window", img);
		int k = cv::waitKey(10);
		if (k == 'q')
			break;
		r.clear();
	}

	return 0;
}


