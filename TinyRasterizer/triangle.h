#pragma once

#include <eigen3/Eigen/Eigen>
#include <array>
#include <vector>

class Triangle
{
public:
	static std::vector<Eigen::Vector3f> vertxs;
	static std::vector<Eigen::Vector3f> normals;
	static std::vector<Eigen::Vector2f> uvs;
private:
	std::array<int, 3> vertx_idx;
	std::array<int, 3> normal_idx;
	std::array<int, 3> uv_idx;

public :

	//to get the vertx (x,y,z)
	Eigen::Vector3f a();
	Eigen::Vector3f b();
	Eigen::Vector3f c();

	//to get the normal of each vertx (x,y,z)
	Eigen::Vector3f normal_a();
	Eigen::Vector3f normal_b();
	Eigen::Vector3f normal_c();

	//to get the uv of each vertx (x,y)
	Eigen::Vector2f uv_a();
	Eigen::Vector2f uv_b();
	Eigen::Vector2f uv_c();

	void set_vertx_idx(const std::array<int,3> & v);
	void set_normal_idx(const std::array<int,3> & n);
	void set_uv_idx(const std::array<int,3> & u);

	static void insert_vertxs(Eigen::Vector3f& v);
	static void insert_normals(Eigen::Vector3f& n);
	static void insert_uvs(Eigen::Vector2f& uv);

};