#ifndef IOMesh_H
#define IOMesh_H

#include <string>
#include <iostream>
#include <eigen3/Eigen/Geometry>

using namespace std;
using namespace Eigen;

class Mesh;

class IOMesh
{
public:
    IOMesh(Mesh* mesh);

    void load_off(const string& filename);
    void load_obj(const string& filename);

private:
    Mesh* _mesh;
};

#endif // IOMesh_H
