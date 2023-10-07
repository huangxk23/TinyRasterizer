#include "rasterizer.h"

//this function is used to reset the frame_buf and depth_buf
void rasterizer::clear()
{
	std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f(0, 0, 0));
	std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::max());
}



rasterizer::rasterizer(int h, int w,Model * m,texture * t) : height(h), width(w),_ptr_m(m),_ptr_t(t)
{
	frame_buf.resize(static_cast<long long>(height) * static_cast<long long>(width));
	depth_buf.resize(static_cast<long long>(height) * static_cast<long long>(width));
	color = { 255,255,255 };
	
};

rasterizer::~rasterizer() {};

//drawing a line from x0 to x1 with x0 < x1 and slope between (0,1]
//flag determine whether transforming back to slope [-1,0)
void rasterizer::draw_line_01(const Eigen::Vector2i &x0, const Eigen::Vector2i &x1, bool flag)
{
	int x = x0.x(), y = x0.y();
	float d = (x0.y() - x1.y()) * (x + 1) + (x1.x() - x0.x()) * (y + 0.5) + x0.x() * x1.y() - x1.x() * x0.y();
	
	for (; x <= x1.x(); x++)
	{
		if (flag) set_pixel({ 2 * x0.x() - x, y }, color);
		else set_pixel({ x,y }, color);
		if (d < 0)
		{
			d += (x1.x() - x0.x()) + (x0.y() - x1.y());
			y += 1;
		}
		else
			d += (x0.y() - x1.y());
	}
}

//drawing a line from x0 to x1 with x0 < x1 and slope between (1,inf)
//flag determine whether transforming back to slope (-inf,-1)
void rasterizer::draw_line_1inf(const Eigen::Vector2i &x0, const Eigen::Vector2i &x1, bool flag)
{
	int x = x0.x(), y = x0.y();
	float d = (x0.y() - x1.y()) * (x + 0.5) + (x1.x() - x0.x()) * (y + 1) + x0.x() * x1.y() - x1.x() * x0.y();

	for (; y <= x1.y(); y++)
	{
		if (flag) set_pixel({ 2 * x0.x() - x,y }, color);
		else set_pixel({x,y},color);
		if (d < 0)
			d += (x1.x() - x0.x());
		else
		{
			x += 1;
			d += (x1.x() - x0.x()) + (x0.y() - x1.y());
		}
	}
}

//implementation of mid-point algorithom
void rasterizer::draw_line(const Eigen::Vector2i &start, const Eigen::Vector2i & end)
{
	if (start.x() == end.x())
	{
		for (int y = std::min(start.y(), end.y()); y <= std::max(start.y(), end.y()); y++)
			set_pixel({ start.x(),y }, color);
		return;
	}

	if (start.y() == end.y())
	{
		for (int x = std::min(start.x(), end.x()); x <= std::max(start.x(), end.x()); x++)
			set_pixel({ x,start.y() }, color);
		return;
	}
	
	float slope = static_cast<float>((end.y() - start.y())) / static_cast<float>((end.x() - start.x()));
	//std::cout << slope << std::endl;
	if (slope > 0 && slope <= 1)
	{
		if (start.x() < end.x() && start.y() < end.y()) draw_line_01(start, end, false);
		else draw_line_01(start, end, false);
		return;
	}
	else if (slope > 1)
	{
		if (start.x() < end.x() && start.y() < end.y()) draw_line_1inf(start, end, false);
		else draw_line_1inf(end, start, false);
		return;
	}
	else if (slope < -1)
	{
		if (end.x() < start.x() && end.y() > start.y())
		{
			Eigen::Vector2i x1(2 * start.x() - end.x(), end.y());
			draw_line_1inf(start, x1, true);
		}
		else
		{
			Eigen::Vector2i x1(2 * end.x() - start.x(), start.y());
			draw_line_1inf(end, x1, true);
		}
		return;
	}
	else
	{
		if (end.x() < start.x() && end.y() > start.y())
		{
			Eigen::Vector2i x1(2 * start.x() - end.x(), end.y());
			draw_line_01(start, x1, true);
		}
		else
		{
			Eigen::Vector2i x1(2 * end.x() - start.x(), start.y());
			draw_line_01(end, x1, true);
		}
		return;
	}

}


void rasterizer::set_pixel(const Eigen::Vector2i& coords, const Eigen::Vector3f& color)
{
	int idx = coords.y() * width + coords.x();
	if(idx < height * width && idx >= 0) 
		frame_buf[idx] = color;
}

void rasterizer::render_wire_frame_orthographics_projection()
{	
	for (auto triangle : _ptr_m -> triangles_list)
	{
		Eigen::Vector3f vertx1 = triangle -> a();
		Eigen::Vector3f vertx2 = triangle -> b();
		Eigen::Vector3f vertx3 = triangle -> c();

		//std::cout << vertx1.x() << " " << vertx1.y() << std::endl;
		//std::cout << vertx2.x() << " " << vertx2.y() << std::endl;
		//std::cout << vertx3.x() << " " << vertx3.y() << std::endl;

		Eigen::Vector3f s1 = trans.perform_orthographic_projection(vertx1);
		Eigen::Vector3f s2 = trans.perform_orthographic_projection(vertx2);
		Eigen::Vector3f s3 = trans.perform_orthographic_projection(vertx3);

		Eigen::Vector2i p1((int)s1.x(), (int)s1.y());
		Eigen::Vector2i p2((int)s2.x(), (int)s2.y());
		Eigen::Vector2i p3((int)s3.x(), (int)s3.y());
		//std::cout << p1.x() << " " << p1.y() << std::endl;
		//std::cout << p2.x() << " " << p2.y() << std::endl;
		//std::cout << p3.x() << " " << p3.y() << std::endl;

		draw_line(p1, p2);
		draw_line(p2, p3);
		draw_line(p3, p1);
	}
}

void rasterizer::render_wire_frame_perspective_projection()
{
	   
	

	for (auto triangle : _ptr_m -> triangles_list)
	{
		
		Eigen::Vector3f vertx1 = triangle->a();
		Eigen::Vector3f vertx2 = triangle->b();
		Eigen::Vector3f vertx3 = triangle->c();

		//std::cout << vertx1.x() << " " << vertx1.y() << std::endl;
		//std::cout << vertx2.x() << " " << vertx2.y() << std::endl;
		//std::cout << vertx3.x() << " " << vertx3.y() << std::endl;

		Eigen::Vector3f s1 = trans.perform_perspective_projection(vertx1);
		Eigen::Vector3f s2 = trans.perform_perspective_projection(vertx2);
		Eigen::Vector3f s3 = trans.perform_perspective_projection(vertx3);

		//std::cout << s1.z() << " " << s2.z() << " " << s3.z() << std::endl;

		Eigen::Vector2i p1((int)s1.x(), (int)s1.y());
		Eigen::Vector2i p2((int)s2.x(), (int)s2.y());
		Eigen::Vector2i p3((int)s3.x(), (int)s3.y());

		
		//std::cout << p1.x() << " " << p1.y() << std::endl;
		//std::cout << p2.x() << " " << p2.y() << std::endl;
		//std::cout << p3.x() << " " << p3.y() << std::endl;

		draw_line(p1, p2);
		draw_line(p2, p3);
		draw_line(p3, p1);
	}
	
	
}

void rasterizer::rasterize_triangles()
{
	
}

void rasterizer::set_model_transformation(int angle)
{
	trans.set_model_transformation(angle);
}

void rasterizer::set_camera_transformation(const float & pos)
{
	trans.set_camera_transformation(pos);
}

void rasterizer::set_orthographics_transformation(const Eigen::Vector2f & lr,const Eigen::Vector2f & tb,const Eigen::Vector2f & nf)
{
	trans.set_ortho_projection(lr,tb,nf);
}

void rasterizer::set_perspective_transformation(const float & n,const float & f,const float & aspect_ratio,const float & fov)
{
	trans.set_pers_projection(n,f,aspect_ratio,fov);
}

void rasterizer::set_view_port_transformation()
{
	trans.set_view_port_transformation(width,height);
}



