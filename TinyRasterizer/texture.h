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
	Eigen::Vector3f  get_color(int u,int v)
	{
		auto u_img = u * width;
		auto v_img = (1 - v) * height;
		auto color = texture_img.at<cv::Vec3b>(v_img, u_img);
		return Eigen::Vector3f(color[2], color[1], color[0]);
	}
};