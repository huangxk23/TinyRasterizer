#pragma once
#include <vector>
#include <eigen3/Eigen/Eigen>
#include <limits>
#include <memory>

#include "triangle.h"

class Model {

public:
	float lx, mx, ly, my, lz, mz;
	std::vector<std::shared_ptr<Triangle>> triangles_list;
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	int nnormals();
	int nuvs();
};
