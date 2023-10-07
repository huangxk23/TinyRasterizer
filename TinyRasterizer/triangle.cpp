#include "triangle.h"

std::vector<Eigen::Vector3f> Triangle::vertxs;
std::vector<Eigen::Vector3f> Triangle::normals;
std::vector<Eigen::Vector2f> Triangle::uvs;

Eigen::Vector3f Triangle::a() { return vertxs[vertx_idx[0]]; }

Eigen::Vector3f Triangle::b() { return vertxs[vertx_idx[1]]; }

Eigen::Vector3f Triangle::c() { return vertxs[vertx_idx[2]]; }

Eigen::Vector3f Triangle::normal_a() { return normals[normal_idx[0]]; }
Eigen::Vector3f Triangle::normal_b() { return normals[normal_idx[1]]; }
Eigen::Vector3f Triangle::normal_c() { return normals[normal_idx[2]]; }


Eigen::Vector2f Triangle::uv_a() { return uvs[uv_idx[0]]; }
Eigen::Vector2f Triangle::uv_b() { return uvs[uv_idx[1]]; }
Eigen::Vector2f Triangle::uv_c() { return uvs[uv_idx[2]]; }

void Triangle::set_vertx_idx(const std::array<int,3>& v) { vertx_idx = v; }
void Triangle::set_normal_idx(const std::array<int,3> & n) { normal_idx = n; }
void Triangle::set_uv_idx(const std::array<int,3> & u) { uv_idx = u; }

void Triangle::insert_vertxs(Eigen::Vector3f& v) { vertxs.emplace_back(v); }
void Triangle::insert_normals(Eigen::Vector3f& n) { normals.emplace_back(n); }
void Triangle::insert_uvs(Eigen::Vector2f& uv) { uvs.emplace_back(uv); }