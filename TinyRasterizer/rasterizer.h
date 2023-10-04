#pragma once

#include <vector>
#include <eigen3/Eigen/Eigen>
#include <limits>
#include <iostream>

class rasterizer
{
private :
	int height, width;
	std::vector<Eigen::Vector3f> frame_buf;
	std::vector<float> depth_buf;
	Eigen::Vector3f color;

public:
	rasterizer(int h, int w) : height(h), width(w) 
	{
		frame_buf.resize(static_cast<long long>(height) * static_cast<long long>(width));
		depth_buf.resize(static_cast<long long>(height) * static_cast<long long>(width));
		color = { 255,255,255 };
	};
	void clear();
	std::vector<Eigen::Vector3f> get_frame_buf() { return frame_buf; };
	int get_height() { return height; };
	int get_width() { return width; };
	void draw_line(const Eigen::Vector2i & start,const Eigen::Vector2i & end);
	void draw_line_01(const Eigen::Vector2i &x0, const Eigen::Vector2i &x1,bool flag);
	void draw_line_1inf(const Eigen::Vector2i &x0, const Eigen::Vector2i &x1,bool flag);
	void set_pixel(const Eigen::Vector2i& coords, const Eigen::Vector3f & color);

};