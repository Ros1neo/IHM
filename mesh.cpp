// Copyright (C) 2008-2011 Gael Guennebaud <gael.guennebaud@inria.fr>
// Nous avons implémenté les fonctions :
// computesNormales, compute_curvatures, fit.

#include <iostream>
#include <limits>

#include <QCoreApplication>
#include <QProgressDialog>
#include <QElapsedTimer>
#include <eigen3/Eigen/Geometry>

#include "mesh.h"
#include "iomesh.h"
#include "shader.h"
#include "vbo.h"

using namespace Eigen;
using namespace std;

Matrix4f Mesh::mat_obj;
Matrix4f Mesh::mat_view;
Matrix4f Mesh::mat_proj;

template<typename T>
void deleteVBO(TypedVBO<T>* vbo)
{
    if(vbo)
        delete(vbo);
}

Mesh::Mesh(const string& filename)
    : _initialized(false),
      _vbo_pos(0),
      _vbo_normal(0),
      _vbo_texture_coords(0),
      _vbo_color(0)
{
    // load geometry
    IOMesh iomesh(this);
    string ext = filename.substr(filename.size()-3,3);
    if(ext=="off" || ext=="OFF")
        iomesh.load_off(filename);
    else if(ext=="obj" || ext=="OBJ")
        iomesh.load_obj(filename);
    else
        cerr << "Mesh: extension \'" << ext << "\' not supported." << endl;

    // make unitary
    make_unitary();

    // compute normals
    compute_normals();

    // create VBOs
    deleteVBO(_vbo_pos);
    deleteVBO(_vbo_normal);
    deleteVBO(_vbo_texture_coords);
    deleteVBO(_vbo_color);
    _vbo_pos = new TypedVBO<Vector3f>(_vertices_pos, "vtx_position");
    _vbo_normal = new TypedVBO<Vector3f>(_vertices_normal, "vtx_normal");
    _vbo_texture_coords = new TypedVBO<Vector2f>(_vertices_texture_coords, "vtx_texture_coord");
    _vbo_color = new TypedVBO<Vector3f>(_vertices_color, "vtx_color");
}

Mesh::~Mesh()
{
    if(_initialized)
        glDeleteBuffers(1,&_indexBufferId);

    // delete VBOs
    deleteVBO(_vbo_pos);
    deleteVBO(_vbo_normal);
    deleteVBO(_vbo_texture_coords);
    deleteVBO(_vbo_color);
}

void Mesh::make_unitary()
{
    Vector3f lowest, highest;
    lowest.fill(std::numeric_limits<float>::max());
    highest.fill(-std::numeric_limits<float>::max());

    for(uint i=0 ; i<_vertices_pos.size() ; ++i)
    {
        lowest  = lowest.array().min(_vertices_pos[i].array());
        highest = highest.array().max(_vertices_pos[i].array());
    }

    Eigen::Vector3f center = (lowest+highest)/2.0;
    float m = (highest-lowest).maxCoeff();
    for(uint i=0 ; i<_vertices_pos.size() ; ++i)
        _vertices_pos[i] = (_vertices_pos[i] - center) / m;
}

void Mesh::compute_normals()
{
    if(_vertices_normal.size() == _vertices_pos.size())
        return;

    _vertices_normal.clear();
    _vertices_normal.resize(_vertices_pos.size());

    for(unsigned int i=0; i<_vertices_normal.size(); i++){
        _vertices_normal[i] = Eigen::Vector3f(0., 0., 0.);
    }

    for(unsigned int i=0; i<_faces.size(); i++){
        int x = _faces[i].x();
        int y = _faces[i].y();
        int z = _faces[i].z();

        Eigen::Vector3f v1 =  (_vertices_pos[y] - _vertices_pos[x]).normalized();
        Eigen::Vector3f v2 =  (_vertices_pos[z] - _vertices_pos[x]).normalized();
        Eigen::Vector3f nf = v1.cross(v2);

        _vertices_normal[x] += nf;
        _vertices_normal[y] += nf;
        _vertices_normal[z] += nf;
    }

    Eigen::Vector3f emptyVec = Eigen::Vector3f::Zero();
    for(unsigned int i=0; i<_vertices_normal.size(); i++)
        if (_vertices_normal[i] != emptyVec)
            _vertices_normal[i] = (_vertices_normal[i]).normalized();
}

void Mesh::draw_geometry(int prg_id)
{
    // init buffers
    if(!_initialized)
    {
        _initialized = true;
        glGenBuffers(1,&_indexBufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3i)*_faces.size(), _faces[0].data(), GL_STATIC_DRAW);

        glGenVertexArrays(1,&_vertexArrayId);
        GL_TEST_ERR;
    }

    // bind the vertex array
    glBindVertexArray(_vertexArrayId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);

    // bind VBOs
    _vbo_pos->bind(prg_id);
    if(_vertices_normal.size() > 0)
        _vbo_normal->bind(prg_id);
    if(_vertices_texture_coords.size() > 0)
        _vbo_texture_coords->bind(prg_id);
    if(_vertices_color.size() > 0)
        _vbo_color->bind(prg_id);

    // send the geometry
    glDrawElements(GL_TRIANGLES, 3*_faces.size(), GL_UNSIGNED_INT, (void*)0);
    GL_TEST_ERR;

    // release the vertex array
    glBindVertexArray(0);
    GL_TEST_ERR;

    // disable VBOs
    _vbo_pos->disable();
    _vbo_normal->disable();
    _vbo_texture_coords->disable();
    _vbo_color->disable();
    GL_TEST_ERR;
}
