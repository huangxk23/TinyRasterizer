#pragma once

#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <cmath>

constexpr double MY_PI = 3.1415926;

//define the transformations that perform on the space
//as for perspective projection, model transformation -> camera transformation -> perspective projection -> view port transformation
//as for orthographic projection, model transformation -> orthographics transformation -> view port transformation

class transformation
{
private:
	Eigen::Matrix4f model_transformation;
	Eigen::Matrix4f camera_transformation;
	Eigen::Matrix4f pers_projection;
	Eigen::Matrix4f ortho_projection;
	Eigen::Matrix4f view_port_transformation;
	
public:
	
	transformation();
	~transformation();
	void set_model_transformation(int angle);
	void set_camera_transformation(const Eigen::Vector3f & eye_pos);
	void set_pers_projection(const float & n,const float & f,const float & aspect_ratio,const float & fov);
	void set_ortho_projection(const Eigen::Vector2f & lr,const Eigen::Vector2f &tb,const Eigen::Vector2f &nf);
	void set_view_port_transformation(const int & width,const int & height);
	Eigen::Vector3f perform_orthographic_projection(const Eigen::Vector3f& vertx);
	std::pair<Eigen::Vector4f,Eigen::Vector3f> perform_perspective_projection(const Eigen::Vector3f& vertx);

};