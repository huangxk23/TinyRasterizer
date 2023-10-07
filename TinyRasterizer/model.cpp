#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) {
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
            Triangle::insert_vertxs(v);
        }
        else if (!line.compare(0, 4, "vt  "))
        {
            iss >> trash;
            Eigen::Vector2f uv;
            iss >> uv.x();
            iss >> uv.y();
            iss >> trash;
            Triangle::insert_uvs(uv);
        }
        else if (!line.compare(0, 4, "vn  "))
        {
            iss >> trash;
            Eigen::Vector3f normal;
            iss >> normal.x();
            iss >> normal.y();
            iss >> normal.z();
            Triangle::insert_normals(normal);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::array<int,3> f, u, n;
            int idx,uv,normal;
            int i = 0;
            iss >> trash;
            while (iss >> idx >> trash >> uv >> trash >> normal) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.at(i) = idx;
                uv--;
                u.at(i) = uv;
                normal--;
                n.at(i) = normal;
                i++;
            }
            std::shared_ptr<Triangle> t = std::make_shared<Triangle>();
            t->set_vertx_idx(f);
            t->set_uv_idx(u);
            t->set_normal_idx(n);
            triangles_list.emplace_back(t);
        }
    }

    std::cerr << "# v# " << nverts() << " f# " << nfaces() << std::endl;
    std::cerr << "# vt " << nuvs() << " vn " << nnormals() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return Triangle::vertxs.size();
}

int Model::nfaces() {
    return triangles_list.size();
}

int Model::nnormals()
{
    return Triangle::normals.size();
}

int Model::nuvs()
{
    return Triangle::uvs.size();
}
