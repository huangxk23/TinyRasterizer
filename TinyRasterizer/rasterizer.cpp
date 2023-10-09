#include "rasterizer.h"

//this function is used to reset the frame_buf and depth_buf
void rasterizer::clear()
{
	std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f(0, 0, 0));
	std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::lowest());
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

void rasterizer::set_depth(const Eigen::Vector2i& coords, float& depth)
{
	int idx = coords.y() * width + coords.x();
	if (idx < height * width && idx >= 0)
		depth_buf[idx] = depth;
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

		Eigen::Vector4f s1 = trans.perform_perspective_projection(vertx1).first;
		Eigen::Vector4f s2 = trans.perform_perspective_projection(vertx2).first;
		Eigen::Vector4f s3 = trans.perform_perspective_projection(vertx3).first;

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

bool rasterizer::inside_triangle(int i,int j,const Eigen::Vector4f & s1,const Eigen::Vector4f & s2,const Eigen::Vector4f & s3)
{
	Eigen::Vector3f p1, p2, p3, p;
	p1 = { s1.x(),s1.y(),0 };
	p2 = { s2.x(),s2.y(),0 };
	p3 = { s3.x(),s3.y(),0 };
	p = { static_cast<float>(i),static_cast<float>(j),0 };

	Eigen::Vector3f cross1 = (p2 - p1).cross(p - p1);
	Eigen::Vector3f cross2 = (p3 - p2).cross(p - p2);
	Eigen::Vector3f cross3 = (p1 - p3).cross(p - p3);

	if ((cross1.z() > 0 && cross2.z() > 0 && cross3.z() > 0) || (cross1.z() < 0 && cross2.z() < 0 && cross3.z() < 0)) return true;
	else return false;
}

std::array<float, 3> rasterizer::compute_barycentirc(const int& i, const int& j, const Eigen::Vector4f& a, const Eigen::Vector4f& b, const Eigen::Vector4f& c)
{
	float alpha = ((j - b.y()) * (c.x() - b.x()) - (i - b.x()) * (c.y() - b.y()))
		/ ((a.y() - b.y()) * (c.x() - b.x()) - (a.x() - b.x()) * (c.y() - b.y()));

	float beta = ((j - c.y()) * (a.x() - c.x()) - (i - c.x()) * (a.y() - c.y()))
		/ ((b.y() - c.y()) * (a.x() - c.x()) - (b.x() - c.x()) *(a.y() - c.y()));
	
	float gamma = 1 - alpha - beta;

	return { alpha,beta,gamma };
}

void rasterizer::render()
{
	for (auto triangle : _ptr_m->triangles_list)
	{
		Eigen::Vector3f vertx1 = triangle->a();
		Eigen::Vector3f vertx2 = triangle->b();
		Eigen::Vector3f vertx3 = triangle->c();

		auto p1 = trans.perform_perspective_projection(vertx1);
		auto p2 = trans.perform_perspective_projection(vertx2);
		auto p3 = trans.perform_perspective_projection(vertx3);

		Eigen::Vector4f s1 = p1.first;
		Eigen::Vector4f s2 = p2.first;
		Eigen::Vector4f s3 = p3.first;

		float x_min = std::min<float>({ s1.x(),s2.x(),s3.x() });
		float x_max = std::max<float>({ s1.x(),s2.x(),s3.x() });
		
		int x_lower_bound = std::floor(x_min);
		int x_upper_bound = std::ceil(x_max);

		float y_min = std::min<float>({ s1.y(),s2.y(),s3.y() });
		float y_max = std::max<float>({ s1.y(),s2.y(),s3.y() });

		int y_lower_bound = std::floor(y_min);
		int y_upper_bound = std::ceil(y_max);

		for (int i = x_lower_bound; i <= x_upper_bound; i++)
		{
			for (int j = y_lower_bound; j <= y_upper_bound; j++)
			{
				
				if (inside_triangle(i,j,s1,s2,s3))
				{
					int idx = j * width + i;
					//interpolate the z value
					std::array<float,3> a = compute_barycentirc(i, j, s1, s2, s3);
					float alpha, beta, gamma;
					alpha = a[0], beta = a[1], gamma = a[2];

					float w = 1.0f / (alpha / s1.w() + beta / s2.w() + gamma / s3.w());
					float depth = alpha * s1.z() / s1.w() + beta * s2.z() / s2.w() + gamma * s3.z() / s3.w();
					depth *= w;

					if (depth > depth_buf[idx])
					{
						depth_buf[idx] = depth;
						
						Eigen::Vector2f uva = triangle->uv_a();
						Eigen::Vector2f uvb = triangle->uv_b();
						Eigen::Vector2f uvc = triangle->uv_c();
						
						float u = alpha * uva.x() / s1.w() + beta * uvb.x() / s2.w() + gamma * uvc.x() / s3.w();
						u *= w;

						float v = alpha * uva.y() / s1.w() + beta * uvb.y() / s2.w() + gamma * uvc.y() / s3.w();
						v *= w;
				
						Eigen::Vector3f normala = triangle->normal_a();
						Eigen::Vector3f normalb = triangle->normal_b();
						Eigen::Vector3f normalc = triangle->normal_c();

						Eigen::Vector3f normal = alpha * normala / s1.w() + beta * normalb/ s2.w() + gamma * normalc / s3.w();
						normal = normal * w;
						normal = normal.normalized();
						
						Eigen::Vector3f camera_coord = alpha * p1.second + beta * p2.second + gamma * p3.second;

						fragment_shader_data data;
						data.uv = { u,v };
						data.t = _ptr_t;
						data.normal = normal;

						auto color = blinn_phong_reflectance_shader(&data);
						
						set_pixel({i,j},color);
					}
				}
			}
		}
	}
}



void rasterizer::set_model_transformation(int angle)
{
	trans.set_model_transformation(angle);
}

void rasterizer::set_camera_transformation(const Eigen::Vector3f & eye_pos)
{
	trans.set_camera_transformation(eye_pos);
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


Eigen::Vector3f rasterizer::texture_map_shader(fragment_shader_data * d)
{
	return d->t->get_color(d->uv.x(), d->uv.y());
}

Eigen::Vector3f rasterizer::blinn_phong_reflectance_shader(fragment_shader_data* d)
{
	Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
	Eigen::Vector3f kd = Eigen::Vector3f(0.64, 0.64, 0.64);
	Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

	auto l1 = light{ {20, 20, 20}, {1000, 1000, 1000} };
	auto l2 = light{ {-20, 20, 0}, {1000, 1000, 1000} };

	std::vector<light> lights = { l1, l2 };
	Eigen::Vector3f amb_light_intensity{ 10, 10, 10 };
	Eigen::Vector3f eye_pos{ 0, 0, 0 };

	float p = 150;

	Eigen::Vector3f color = d->t->get_color(d->uv.x(), d->uv.y()) / 255.f;
	Eigen::Vector3f point = d->camera_pos;
	Eigen::Vector3f normal = d->normal;

	Eigen::Vector3f result_color = { 0, 0, 0 };
	for (auto& light : lights)
	{
		

		Eigen::Vector3f light_direction = (light.position - point).normalized();
		Eigen::Vector3f view_direction = (eye_pos - point).normalized();

		auto r_2 = (light.position - point).squaredNorm();

		auto ambient_l = ka.cwiseProduct(amb_light_intensity);

		auto diffuse_l = kd.cwiseProduct(light.intensity / r_2) * std::max(0.0f, light_direction.dot(normal));

		auto h = (light_direction + view_direction).normalized();
		auto specular_l = ks.cwiseProduct(light.intensity / r_2) * std::pow(std::max(0.0f, h.dot(normal)), p);

		result_color += ambient_l + diffuse_l + specular_l;


	}

	Eigen::Vector3f result = { result_color.x() * color.x() * 255.f,result_color.y() * color.y() * 255.f,result_color.z() * color.z() * 255.f };

	return result;
}