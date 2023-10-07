#pragma once
#include <vector>
#include <eigen3/Eigen/Eigen>
#include <limits>

class Model {
private:
	std::vector<std::vector<int> > faces_vertx_idx_;
	std::vector<std::vector<int> > faces_uv_idx_;
	std::vector<std::vector<int> > faces_normals_idx_;

	std::vector<Eigen::Vector3f> verts_;
	std::vector<Eigen::Vector2f> uvs_;
	std::vector<Eigen::Vector3f> normals_;

public:
	float lx, mx, ly, my, lz, mz;
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	
	std::vector<int> get_vertx_idx(int idx);
	std::vector<int> get_uv_idx(int idx);
	std::vector<int> get_normal_idx(int idx);

	Eigen::Vector3f get_vertx(int idx);
	Eigen::Vector2f get_uvs(int idx);
	Eigen::Vector3f get_normals(int idx);
};
