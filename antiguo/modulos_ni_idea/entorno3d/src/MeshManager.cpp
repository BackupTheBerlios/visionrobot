
#include "MeshManager.h"


// Inicializacion de las variables estaticas.
MeshManager::ListaMeshType MeshManager::m_ListaMesh= *(new std::map<std::string, Mesh>);


MeshManager::MeshManager()
{

}

MeshManager::~MeshManager()
{

}

Mesh* MeshManager::NuevoMesh(const std::string & strNombre,LPDIRECT3DDEVICE9 pd3dDevice)
{
  
    
    Mesh* nuevoMesh = new Mesh(strNombre,pd3dDevice);
    
    std::pair<ListaMeshType::iterator, bool> ret;
    
    ret=m_ListaMesh.insert(std::make_pair(strNombre, *nuevoMesh));

    if(ret.second)
    {
        return nuevoMesh;
    }
    else
    {
        delete nuevoMesh;
        return &ret.first->second;
    }

    
}

/* EOF */