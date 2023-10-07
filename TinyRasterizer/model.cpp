#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) : verts_(), faces_vertx_idx_() {
    lx = ly = lz = std::numeric_limits<float>::max();
    mx = my = mz = std::numeric_limits<float>::lowest();

    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Eigen::Vector3f v;
            iss >> v.x();
            iss >> v.y();
            iss >> v.z();
            lx = std::min(lx, v.x()); ly = std::min(ly, v.y()); lz = std::min(lz, v.z());
            mx = std::max(mx, v.x()); my = std::max(my, v.y()); mz = std::max(mz,v.z());
            verts_.push_back(v);
        }
        else if (!line.compare(0, 4, "vt  "))
        {
            iss >> trash;
            Eigen::Vector2f uv;
            iss >> uv.x();
            iss >> uv.y();
            iss >> trash;
            uvs_.emplace_back(uv);
        }
        else if (!line.compare(0, 4, "vn  "))
        {
            iss >> trash;
            Eigen::Vector3f normal;
            iss >> normal.x();
            iss >> normal.y();
            iss >> normal.z();

            normals_.emplace_back(normal);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f, u, n;
            int idx,uv,normal;
            iss >> trash;
            while (iss >> idx >> trash >> uv >> trash >> normal) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.emplace_back(idx);
                uv--;
                u.emplace_back(uv);
                normal--;
                n.emplace_back(normal);
            }
            faces_vertx_idx_.emplace_back(f);
            faces_uv_idx_.emplace_back(u);
            faces_normals_idx_.emplace_back(n);
        }
    }

    std::cerr << "# v# " << verts_.size() << " f# " << faces_vertx_idx_.size() << std::endl;
    std::cerr << "# vt " << uvs_.size() << " vn " << normals_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_vertx_idx_.size();
}

//give the index of a triangle and return the index of its three vertexs
std::vector<int> Model::get_vertx_idx(int idx) {
    return faces_vertx_idx_[idx];
}

//give the index of a triangle and return the index of its three vertexs' uv
std::vector<int> Model::get_uv_idx(int idx)
{
    return faces_uv_idx_[idx];
}

std::vector<int> Model::get_normal_idx(int idx)
{
    return faces_normals_idx_[idx];
}

//give the index of vertex and return the vertex value (x,y,z)
Eigen::Vector3f Model::get_vertx(int idx) {
    return verts_[idx];
}

//give the index of uv and return the uv value (u,v)
Eigen::Vector2f Model::get_uvs(int idx)
{
    return uvs_[idx];
}

//give the index of normal and return the normals value (x,y,z)
Eigen::Vector3f Model::get_normals(int idx)
{
    return normals_[idx];
}