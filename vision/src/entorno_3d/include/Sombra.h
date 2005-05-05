#pragma once

#include <d3dx9.h>

class Sombra  
{
public:
	Sombra();
	virtual ~Sombra();

public:

    void Reset();
    void ConstruirSombra(LPD3DXMESH lpMesh, D3DXVECTOR3 vLuz);
    void Render(LPDIRECT3DDEVICE9 pd3dDevice);

private:

    /*
    Inserta un segmento en la lista de los segmentos que constituyen el contorno
    del objeto 3d cuando se ve desde la posicion de la luz que proyecta la sombra.

    */
    void InsertarSegmento( WORD* pEdges, DWORD& dwNumEdges, WORD v0, WORD v1 );

private:
    
   

private:
    // FIXME:este tamaño fijo habra que modificarlo para hacerlo mas flexible
    // en los objetos con mas poligonos.
	D3DXVECTOR3 m_pVertices[32000]; // Vertex data for rendering shadow volume
    
    /*
    Numero de vertices de la sombra calculada.
    */
    DWORD       m_dwNumVertices;



};

/* EOF*/
