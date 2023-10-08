#pragma once

#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <string>

class texture
{
private:
	cv::Mat texture_img;
	int width, height;
public:
	texture(const std::string & path)
	{
		texture_img = cv::imread(path);
		width = texture_img.rows;
		height = texture_img.cols;
	}
	Eigen::Vector3f  get_color(float u,float v)
	{
		//std::cout << width << " " << height << std::endl;
		//std::cout << u << " " << v << std::endl;
		auto u_img = static_cast<int>(u * width) % width;
		auto v_img = static_cast<int>((1-v) * height) % height;
		//std::cout << u_img << " " << v_img << std::endl;

		//in opencv function at(): the first index is y while the second index is x; 
		auto color = texture_img.at<cv::Vec3b>(v_img, u_img);
		return Eigen::Vector3f(color[2], color[1], color[0]);
	}
};