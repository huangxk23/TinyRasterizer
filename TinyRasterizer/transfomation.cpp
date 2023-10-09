#include "transformation.h"

transformation::transformation() {};
transformation::~transformation() {};

//set a model transformation rotation aroung axis y
void transformation::set_model_transformation(int angle)
{
	model_transformation = Eigen::Matrix4f::Identity();

	double radian = angle / 180.0f * MY_PI;

	model_transformation << cos(radian), 0, sin(radian), 0,
		0, 1, 0, 0,
		-sin(radian), 0, cos(radian), 0,
		0, 0, 0, 1;
	
}

void transformation::set_camera_transformation(const Eigen::Vector3f & eye_pos)
{
	camera_transformation = Eigen::Matrix4f::Identity();
	
	camera_transformation << 1, 0, 0, -eye_pos.x(),
		0, 1, 0, -eye_pos.y(),
		0, 0, 1, -eye_pos.z(),
		0, 0, 0, 1;
}

void transformation::set_pers_projection(const float & n,const float & f,const float & aspect_ratio,const float & fov)
{
	float l, r, t, b;
	double radian = fov / 2.0f / 180.0f * MY_PI;
	t = std::abs(n) * tan(radian);
	b = -t;
	r = t * aspect_ratio;
	l = -r;
	Eigen::Matrix4f M1, M2, M3,M4;
	M1 << n, 0, 0, 0,
		0, n, 0, 0,
		0, 0, n + f, -n*f,
		0, 0, 1, 0;

	M2 << 1, 0, 0, -(l + r) / 2,
		0, 1, 0, -(t + b) / 2,
		0, 0, 1, -(n + f) / 2,
		0, 0, 0, 1;

	M3 << 2.0f / (r - l), 0, 0, 0,
		0, 2.0f / (t - b), 0, 0,
		0, 0, 2.0f / (n - f), 0,
		0, 0, 0, 1;


	pers_projection = M3 * M2 * M1;
}

void transformation::set_ortho_projection(const Eigen::Vector2f & lr,const Eigen::Vector2f & tb,const Eigen::Vector2f & nf)
{
	//of course we can directly write the orthographics projection matrix but
	Eigen::Matrix4f M1, M2;
	M1 << 2.0f / (lr.y() - lr.x()), 0, 0, 0,
		0, 2.0f / (tb.y() - tb.x()), 0, 0,
		0, 0, 2.0f / (nf.y() - nf.x()), 0,
		0, 0, 0, 1;
	M2 << 1, 0, 0, -(lr.x() + lr.y()) / 2.0f,
		0, 1, 0, -(tb.x() + tb.y()) / 2.0f,
		0, 0, 1, -(nf.x() + nf.y()) / 2.0f,
		0, 0, 0, 1;
	
	ortho_projection = M1 * M2;
}

void transformation::set_view_port_transformation(const int & width, const int& height)
{
	view_port_transformation << width / 2.0f, 0, 0, (width - 1.0f) / 2.0f,
		0, height / 2.0f, 0, (height - 1.0f) / 2.0f,
		0, 0, 1, 0,
		0, 0, 0, 1;
}

Eigen::Vector3f transformation::perform_orthographic_projection(const Eigen::Vector3f& vertx)
{
	Eigen::Vector4f homo_coords(vertx.x(), vertx.y(), vertx.z(), 1.0f);

	//std::cout << homo_coords.x() << " " << homo_coords.y() << std::endl;

	//std::cout << model_transformation << std::endl;
	Eigen::Vector4f canonical_coords = ortho_projection * model_transformation * homo_coords;

	//std::cout << canonical_coords.x() << " "<< canonical_coords.y() << std::endl;

	Eigen::Vector4f screen_coords = view_port_transformation * canonical_coords;

	return {screen_coords.x(),screen_coords.y(),screen_coords.z()};
}

//return value : (screen_coords.x(),screen_coords.y(),screen_coords.z(),w) w is used for linear interpolation correction

std::pair<Eigen::Vector4f,Eigen::Vector3f> transformation::perform_perspective_projection(const Eigen::Vector3f& vertx)
{

	Eigen::Vector4f homo_coords(vertx.x(), vertx.y(), vertx.z(), 1.0f);

	Eigen::Vector4f camera_coords = camera_transformation * model_transformation * homo_coords;

	Eigen::Vector4f canonical_coords = pers_projection * camera_coords;

	//homogenius division
	float w = canonical_coords.w();
	canonical_coords = canonical_coords / w;

	Eigen::Vector4f screen_coords = view_port_transformation * canonical_coords;

	return { { screen_coords.x(),screen_coords.y(),screen_coords.z(),w},{camera_coords.x(),camera_coords.y(),camera_coords.z()} };

	
}