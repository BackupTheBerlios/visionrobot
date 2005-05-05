

#include "Mesh.h"



Mesh::Mesh()
{

}

Mesh::Mesh(const std::string & strNombre)
{
    m_strNombre = strNombre;
}

Mesh::Mesh(const std::string & strNombre,LPDIRECT3DDEVICE9 pd3dDevice)
{

    m_strNombre=strNombre;

   // Carga el objeto del archivo especificado.
    if( FAILED( D3DXLoadMeshFromX( strNombre.c_str(), D3DXMESH_SYSTEMMEM, 
            pd3dDevice, NULL, &pD3DXMtrlBuffer, NULL,
			&g_dwNumMaterials, &g_pMesh ) ) )
    {
                MessageBox(NULL, "No se puede encontrar el archivo .x",
			           "Error al cargar Mesh", MB_OK);
                
           
     }
    

    // Se extraen los materiales y texturas del objeto.
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
    g_pMeshTextures  = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];

    for( DWORD i=0; i<g_dwNumMaterials; i++ )
    {
        // Copia el material.
        g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Establece el color difuso del material.
        g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

        g_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL && 
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0 )
        {
            // Se crea la  textura
            if( FAILED(D3DXCreateTextureFromFile( pd3dDevice, 
                                                d3dxMaterials[i].pTextureFilename, 
                                                &g_pMeshTextures[i] ) ) )
            {
               
                MessageBox(NULL, "No se puede hallar el archivo de textura.",
                    "Error al cargar textura", MB_OK);
               
            }
        }
    }

    // Se libera el buffer.
    pD3DXMtrlBuffer->Release();

    // Clonar el mesh para insertarle en el FVF las normales y poder calcularlas
    // para su uso en la iluminacion.
    LPD3DXMESH  lpTempMesh = NULL;

    
    

    g_pMesh->CloneMeshFVF(0,  D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE |D3DFVF_TEX1 , 
                           pd3dDevice, &lpTempMesh);
   
    
      

    D3DXComputeNormals( lpTempMesh,NULL);

    g_pMesh=lpTempMesh;

}

Mesh::~Mesh()
{

}


bool Mesh::operator ==(Mesh mesh)
{
   
    return (m_strNombre == mesh.GetNombre());
}

std::string Mesh::GetNombre()
{
    return m_strNombre;
    
}

LPD3DXMESH Mesh::GetMesh()
{
    return g_pMesh;
}


D3DMATERIAL9* Mesh::GetMateriales()
{
    return g_pMeshMaterials;
}

 
DWORD Mesh::GetNumeroMateriales()
{
    return g_dwNumMaterials;

}

LPDIRECT3DTEXTURE9* Mesh::GetTextura()
{
    return g_pMeshTextures;
}


/* EOF */