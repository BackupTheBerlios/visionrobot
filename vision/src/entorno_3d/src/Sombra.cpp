
#include "Sombra.h"


Sombra::Sombra()
{

}

Sombra::~Sombra()
{

}


void Sombra::Reset()
{
    this->m_dwNumVertices= 0;
}


void Sombra::ConstruirSombra(LPD3DXMESH pMesh, D3DXVECTOR3 vLight)
{
    // Note: the MeshVertex format depends on the FVF of the mesh
	struct MeshVertex { D3DXVECTOR3 p, n;
                            DWORD diffuse;
                         float tu,tv;   };

    MeshVertex *pVertices;
    WORD       *pIndices;

    // Lock the geometry buffers
    pMesh->LockVertexBuffer( 0L, (LPVOID*)&pVertices );
    pMesh->LockIndexBuffer( 0L, (LPVOID*)&pIndices );
    DWORD dwNumFaces    = pMesh->GetNumFaces();

    // Allocate a temporary edge list
    WORD *pEdges = new WORD[dwNumFaces*6];

    if( pEdges == NULL )
    {
        pMesh->UnlockVertexBuffer();
        pMesh->UnlockIndexBuffer();
        return ;
    }
	
    DWORD dwNumEdges = 0;

    // For each face
    for( DWORD i = 0; i < dwNumFaces; ++i )
    {
        WORD wFace0 = pIndices[3*i+0];
        WORD wFace1 = pIndices[3*i+1];
        WORD wFace2 = pIndices[3*i+2];

        D3DXVECTOR3 v0 = pVertices[wFace0].p;
        D3DXVECTOR3 v1 = pVertices[wFace1].p;
        D3DXVECTOR3 v2 = pVertices[wFace2].p;

        // Transform vertices or transform light?
        D3DXVECTOR3 vCross1(v2-v1);
        D3DXVECTOR3 vCross2(v1-v0);
        D3DXVECTOR3 vNormal;
        D3DXVec3Cross( &vNormal, &vCross1, &vCross2 );

        if( D3DXVec3Dot( &vNormal, &vLight ) >= 0.0f )
        {
            InsertarSegmento( pEdges, dwNumEdges, wFace0, wFace1 );
            InsertarSegmento( pEdges, dwNumEdges, wFace1, wFace2 );
            InsertarSegmento( pEdges, dwNumEdges, wFace2, wFace0 );
        }
    }

    // Se construyen las caras de la sombra extrudando los segmentos en la dirección
    // de la luz y una longitud 10 veces la del vector luz.
    for( i = 0; i < dwNumEdges; ++i )
    {
        D3DXVECTOR3 v1 = pVertices[pEdges[2*i+0]].p;
        D3DXVECTOR3 v2 = pVertices[pEdges[2*i+1]].p;
        D3DXVECTOR3 v3 = v1 - vLight/10;
        D3DXVECTOR3 v4 = v2 - vLight/10;

        // Add a quad (two triangles) to the vertex list
        m_pVertices[m_dwNumVertices++] = v1;
        m_pVertices[m_dwNumVertices++] = v2;
        m_pVertices[m_dwNumVertices++] = v3;

        m_pVertices[m_dwNumVertices++] = v2;
        m_pVertices[m_dwNumVertices++] = v4;
        m_pVertices[m_dwNumVertices++] = v3;
    }

    // Delete the temporary edge list
    delete[] pEdges;

    // Unlock the geometry buffers
    pMesh->UnlockVertexBuffer();
    pMesh->UnlockIndexBuffer();

   
    
}

void Sombra::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
    pd3dDevice->SetFVF( D3DFVF_XYZ );

    pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_dwNumVertices/3,
                                        m_pVertices, sizeof(D3DXVECTOR3) );
                                        

}


void Sombra::InsertarSegmento( WORD* pEdges, DWORD& dwNumEdges, WORD v0, WORD v1 )
{
         // Remove interior edges (which appear in the list twice)
    for( DWORD i = 0; i < dwNumEdges; ++i )
    {
        if( ( pEdges[2*i+0] == v0 && pEdges[2*i+1] == v1 ) ||
            ( pEdges[2*i+0] == v1 && pEdges[2*i+1] == v0 ) )
        {
            if( dwNumEdges > 1 )
            {
                pEdges[2*i+0] = pEdges[2*(dwNumEdges-1)+0];
                pEdges[2*i+1] = pEdges[2*(dwNumEdges-1)+1];
            }

            --dwNumEdges;
            return;
        }
    }

    pEdges[2*dwNumEdges+0] = v0;
    pEdges[2*dwNumEdges+1] = v1;
    dwNumEdges++;
}

/* EOF */