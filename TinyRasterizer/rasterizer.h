#pragma once

#include <vector>
#include <eigen3/Eigen/Eigen>
#include <limits>
#include <iostream>

#include "transformation.h"
#include "model.h"
#include "texture.h"

class rasterizer
{
private :
	int height, width;
	std::vector<Eigen::Vector3f> frame_buf;
	std::vector<float> depth_buf;
	Eigen::Vector3f color;
	transformation trans;
	Model * _ptr_m;
	texture *_ptr_t;

public:

	rasterizer(int h, int w,Model * m,texture * p);
	~rasterizer();
	void clear();
	std::vector<Eigen::Vector3f> get_frame_buf() { return frame_buf; };
	int get_height() { return height; };
	int get_width() { return width; };
	
	
	void draw_line(const Eigen::Vector2i & start,const Eigen::Vector2i & end);
	void draw_line_01(const Eigen::Vector2i &x0, const Eigen::Vector2i &x1,bool flag);
	void draw_line_1inf(const Eigen::Vector2i &x0, const Eigen::Vector2i &x1,bool flag);
	
	void set_pixel(const Eigen::Vector2i& coords, const Eigen::Vector3f & color);
	
	void render_wire_frame_orthographics_projection();
	void render_wire_frame_perspective_projection();
	
	void rasterize_triangles();
	
	void set_model_transformation(int angle);
	void set_camera_transformation(const float & pos);
	void set_orthographics_transformation(const Eigen::Vector2f &lr, const Eigen::Vector2f &tb,const Eigen::Vector2f &nf);
	void set_perspective_transformation(const float & n,const float & f,const float & aspect_ration,const float & fov);
	void set_view_port_transformation();


};