#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "model.h"
#include "rasterizer.h"
#include "texture.h"

int height = 700, width = 700;
int angle = 0;
float move = 0;

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
enum class projection_type
{
	orthographics,perspective
};

int main()
{	
	char obj_path[] = "./model/african_head.obj";
	Model m(obj_path);
	//std::cout << m.lx << " " << m.mx << std::endl; std::cout << m.ly << " " << m.my << std::endl; std::cout << m.lz << " " << m.mz << std::endl;

	std::string texture_path = "./model/african_head_diffuse.jpg";
	texture tex(texture_path);
	std::cout << "Model Loading Complete!" << std::endl;
	//for (auto uv : Triangle::uvs) std::cout << uv << std::endl;
	//for (auto normal : Triangle::normals) std::cout << normal << std::endl;
	//return 0;
	rasterizer r(height, width,&m,&tex);
	
	projection_type t = projection_type::perspective;

	//parameters of perspective projection:
	float n, f,aspect_ratio,fov;
	n = -1, f = -3,aspect_ratio = 1,fov = 90;
	Eigen::Vector3f camera_pos;
	camera_pos = { 0,0,2 };

	//parameters of orthographics projection:
	Eigen::Vector2f lr, tb, fn;
	lr = { -1,1 }, tb = { -1.5,1.5 }, fn = { -1,1 };
	
	while (1)
	{
		r.clear();
		switch (t)
		{
		case projection_type::orthographics:
			r.set_model_transformation(angle);
			r.set_orthographics_transformation(lr,tb,fn);
			r.set_view_port_transformation();
			r.render_wire_frame_orthographics_projection();
			
			break;
		case projection_type::perspective:
			r.set_model_transformation(angle);
			r.set_camera_transformation(camera_pos.z());
			r.set_perspective_transformation(n,f,aspect_ratio,fov);
			r.set_view_port_transformation();
			//r.render_wire_frame_perspective_projection();
			r.render();
			break;
		}
		//initialize the opencv Mat
		cv::Mat img = cv::Mat::ones(height, width, CV_8UC3);

		write_Mat(img, r.get_frame_buf());

		cv::imshow("display window", img);
		int k = cv::waitKey(0);
		if (k == 'a')
			angle -= 20;
		else if (k == 'd')
			angle += 20;
		else if (k == 'q')
			break;
		
	}

	return 0;
}


