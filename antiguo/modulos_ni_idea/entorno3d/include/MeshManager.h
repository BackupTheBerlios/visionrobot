/** ------------------------------------------------------------------------------------
//
//  @author Jorge Mendoza 2004.
//
//  @brief  Se encarga de crear un nuevo mesh a partir de un archivo en formato .x
//          Si el mesh ya ha sido previamente cargado en memoria, simplemente se devuelve
//          un puntero al ya existente y se evita la sobrecarga de volver a cargarlo desde
//          un archivo.
*/
#pragma once
#pragma warning (disable: 4786)
#include <map>
#include <string>
#include <algorithm>
#include <d3dx9.h>
#include "Mesh.h"



class MeshManager  
{
public:

	MeshManager();
	virtual ~MeshManager();

public:
    
    /**
    Busca si el mesh ha sido previamente creado y esta en la lista interna de
    mesh cargados. En caso contrario lo carga desde el archivo correspondiente
    y lo inserta en la lista.

    @param strNombre Nombre del archivo del mesh con el que se identifica al mesh.
    */
    static  Mesh* NuevoMesh(const std::string & strNombre,LPDIRECT3DDEVICE9 pd3dDevice);

public:
    /**
    Tipo de la lista que guarda los mesh cargados en memoria. Se implementa como
    un map para tener un coste eficiente en las busquedas e insercion.
    */
    typedef std::map<std::string, Mesh> ListaMeshType;
   
private:
   
    /**
    Lista que contiene los mesh cargados en memoria. Solo se tiene una copia
    de cada objeto cargado desde un archivo que será compartido por los objetos
    que tienen el mismo mesh.
    */
     static ListaMeshType m_ListaMesh;
};


/* EOF */