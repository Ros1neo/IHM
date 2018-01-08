#include <fstream>

#include "iomesh.h"
#include "objformat.h"
#include "mesh.h"

IOMesh::IOMesh(Mesh *mesh)
    : _mesh(mesh)
{
}

void IOMesh::load_off(const string &filename)
{
    ifstream in(filename.c_str(),std::ios::in);
    if(!in)
    {
        cerr << "File not found " << filename << endl;
        return;
    }

    string header;
    in >> header;

    if (header!="OFF" && header!="COFF")
    {
        cerr << "Wrong header = " << header << endl;
        return;
    }

    int nof_vertices, nof_faces, inull;
    int nb, id0, id1, id2;
    Vector3f v, c;

    in >> nof_vertices >> nof_faces >> inull;

    if(header == "COFF"){

        for(int i=0 ; i<nof_vertices ; ++i)
        {
            float alpha;
            in >> v.x() >> v.y() >> v.z() >> c.x() >> c.y() >> c.z() >> alpha;
            _mesh->_vertices_pos.push_back(v);
            _mesh->_vertices_color.push_back(c/255.);
        }
    }
    else { //OFF file
        for(int i=0 ; i<nof_vertices ; ++i)
        {
            in >> v.x() >> v.y() >> v.z();
            _mesh->_vertices_pos.push_back(v);
        }
    }

    for(int i=0 ; i<nof_faces ; ++i)
    {
        in >> nb >> id0 >> id1 >> id2;
        assert(nb==3);
        _mesh->_faces.push_back(Vector3i(id0, id1, id2));
    }
    in.close();
}

void IOMesh::load_obj(const string &filename)
{
    ObjMesh* pRawObjMesh = ObjMesh::LoadFromFile(filename);

    if (!pRawObjMesh)
    {
        cerr << "Mesh::load_obj: error loading file " << filename << "." << endl;
        return;
    }

    ObjMesh* pObjMesh = pRawObjMesh->createIndexedFaceSet(Obj::Options(Obj::AllAttribs|Obj::Triangulate));

    pRawObjMesh = 0;

    for (unsigned i=0 ; i<pObjMesh->positions.size() ; ++i)
    {
        // position
        _mesh->_vertices_pos.push_back(Vector3f(pObjMesh->positions[i]));
        // normal
        if(!pObjMesh->normals.empty())
            _mesh->_vertices_normal.push_back(Vector3f(pObjMesh->normals[i]));
        // texture coords
        if(!pObjMesh->texcoords.empty())
            _mesh->_vertices_texture_coords.push_back(Vector2f(pObjMesh->texcoords[i]));
    }

    // copy _faces
    for (unsigned smi=0 ; smi<pObjMesh->getNofSubMeshes() ; ++smi)
    {
        const ObjSubMesh* pSrcSubMesh = pObjMesh->getSubMesh(smi);

        _mesh->_faces.reserve(pSrcSubMesh->getNofFaces());

        for (uint fid = 0 ; fid<pSrcSubMesh->getNofFaces() ; ++fid)
        {
            ObjConstFaceHandle srcFace = pSrcSubMesh->getFace(fid);
            _mesh->_faces.push_back(Vector3i(srcFace.vPositionId(0), srcFace.vPositionId(1), srcFace.vPositionId(2)));
        }
    }
}
