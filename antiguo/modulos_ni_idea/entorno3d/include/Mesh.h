/* ------------------------------------------------------------------------------------
//
//  @author Jorge Mendoza 2004.
//
//  @brief  Esta clase administra los mesh de la aplicación. Los carga desde un archivo
//          extrayendo la informacion de geometria, texturas, materiales,etc.
//
**/

#pragma once

#include <string>
#include <d3dx9.h>

class Mesh  
{
public:
	Mesh();
    
    /**
    Constructor que crea un objeto mesh que solo contiene su nombre. Se usará
    para crear objetos auxiliares en comparaciones.
    
    @param strNombre Nombre del mesh. Normalmente será el nombre del archivo.
    */
    Mesh(const std::string & strNombre);

    Mesh(const std::string & strNombre,LPDIRECT3DDEVICE9 pd3dDevice);
	
    virtual ~Mesh();

public:
    
    /**
    Se implementa el operador de igualdad para que se puedan comparar mesh entre
    si atendiendo a su nombre. Esto permitirá utilizar métodos genéricos de 
    busqueda utilizado por ejemplo por clases STL.

    @param mesh Puntero al mesh con el que se realizara la comparacion.
    @return Verdadero si el nombre de los mesh coincide y falso en otro caso.
    */
    bool operator ==  (Mesh mesh);

public:

    /**
    Devuelve el puntero al Mesh.
    
    @return Puntero al Mesh.
    */
    LPD3DXMESH GetMesh();

    /**
    Devuelve el puntero al buffer con los materiales del mesh.

    @return Buffer con los materiales del mesh.
    */
    D3DMATERIAL9* GetMateriales();

    /**
    Devuelve el numero de materiales del mesh.

    @return Numero de materiales del mesh.
    */
    DWORD GetNumeroMateriales();

    /**
    Devuelve las textura  del mesh contenidas en un buffer..

    
    @return Buffer con las texturas del mesh.
    */
     LPDIRECT3DTEXTURE9* GetTextura();

     /**
     Devuelve el nombre del mesh. En principio dicho nombre coincide con el
     del archivo desde el que se carga.

     @return Cadena con el nombre del mesh.
     */
     std::string GetNombre();

   

private:
    
    /**
    Puntero al mesh almacenado en memoria.
    */
    LPD3DXMESH              g_pMesh          ; 
    
    /**
    Buffer para almacenar los materiales del objeto.
    */
    LPD3DXBUFFER pD3DXMtrlBuffer;

    /**
    Buffer que contiene los materiales del objeto.
    */
    D3DMATERIAL9*           g_pMeshMaterials ; 
    
    /**
    Buffer que contiene las texturas del objeto.
    */
    LPDIRECT3DTEXTURE9*     g_pMeshTextures  ; 
    
    /**
    Numero de materiales que contiene el objeto.
    */
    DWORD                   g_dwNumMaterials ;  

    /**
    Nombre del archivo que contiene al mesh y por el cual se identifica al objeto.
    */
    std::string m_strNombre;

};

/* EOF */
