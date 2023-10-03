#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

//initialize a frame_buf to store the rgb value for img
//transform the frame_buf into opencv::Mat
//display the opencv::Mat

int height, width;
std::vector<Eigen::Vector3f> frame_buf;

int main()
{
	std::cin >> height >> width;

	frame_buf.resize(height * width);
	std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f(0, 0, 0));

	//initialize the rgb value of the image
	//1/3 red ,1/3 green ,1/3 blue
	int step = height / 3;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int idx = i * width + j;
			if (i < step)
				frame_buf[idx] = Eigen::Vector3f(255, 0, 0);
			else if (i < 2 * step)
				frame_buf[idx] = Eigen::Vector3f(0, 255, 0);
			else
				frame_buf[idx] = Eigen::Vector3f(0, 0, 255);

		}
	}

	//initialize the opencv Mat
	cv::Mat img = cv::Mat::ones(height, width, CV_8UC3);

	int channels = img.channels(), rows = img.rows, cols = img.cols * channels;
	//the Mat may be store in a one dimension formation
	if (img.isContinuous())
	{
		cols = rows * cols;
		rows = 1;
	}

	//transform the value in frame_buf into Mat
	uchar* p;
	int idx = 0;
	for (int i = 0; i < rows; ++i)
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
	//std::cout << img << std::endl;
	cv::imshow("display window", img);
	int k = cv::waitKey(0);
	if (k == 'q')
		return 0;
}


