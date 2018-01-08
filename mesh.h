// Copyright (C) 2008-2011 Gael Guennebaud <gael.guennebaud@inria.fr>

#ifndef _Mesh_h_
#define _Mesh_h_

#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigen>

template<class T>
class TypedVBO;

using namespace std;
using namespace Eigen;

class Mesh
{
public:

    friend class IOMesh;

    Mesh(const string& filename);
    ~Mesh();

    virtual void draw_geometry(int prg_id);

private:
    void make_unitary();
    void compute_normals();

protected:
    bool _initialized;

    // faces
    vector<Vector3i> _faces;
    // vertices positions
    TypedVBO<Vector3f>* _vbo_pos;
    vector<Vector3f> _vertices_pos;
    // vertices normals
    TypedVBO<Vector3f>* _vbo_normal;
    vector<Vector3f> _vertices_normal;
    // vertices texture coords
    TypedVBO<Vector2f>* _vbo_texture_coords;
    vector<Vector2f> _vertices_texture_coords;
    // vertices texture coords
    TypedVBO<Vector3f>* _vbo_color;
    vector<Vector3f> _vertices_color;

    mutable unsigned int _indexBufferId;  ///< the id of the BufferObject storing the faces indices
    mutable unsigned int _vertexArrayId;  ///< the id of the VertexArray object

public:
    static Matrix4f mat_obj;
    static Matrix4f mat_proj;
    static Matrix4f mat_view;
};



#endif // MESH_H
