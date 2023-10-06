#pragma once
#include <vector>
#include <eigen3/Eigen/Eigen>
#include <limits>

class Model {
private:
	std::vector<Eigen::Vector3f> verts_;
	std::vector<std::vector<int> > faces_;
public:
	float lx, mx, ly, my, lz, mz;
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Eigen::Vector3f vert(int i);
	std::vector<int> face(int idx);
};
