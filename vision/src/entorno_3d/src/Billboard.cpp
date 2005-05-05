

#include "Billboard.h"



Billboard::Billboard()
{

}
Billboard::Billboard(LPDIRECT3DDEVICE9 pd3dDevice,
                 Camara* pCamara,
                D3DXVECTOR3 vPosicion, 
                const std::string& strTextura,
                float fSize,
                D3DXCOLOR clrColor
                )
{
    

    m_pd3dDevice = pd3dDevice;
    m_pCamara = pCamara;
    m_vPos = vPosicion;

    D3DXMatrixIdentity(&m_matBillboard); 
    m_fSize = fSize;
    


    // Se carga la textura.
    D3DXCreateTextureFromFile( m_pd3dDevice, strTextura.c_str(), &m_pTexturaBillboard );
    

    m_pd3dDevice->CreateVertexBuffer( 4*sizeof(VERTICE),
                                      D3DUSAGE_WRITEONLY, VERTICE::FVF_Flags,
                                      D3DPOOL_MANAGED, &m_pVB, NULL );
    VERTICE* pVertices;
    m_pVB->Lock( 0, 0, (void**)&pVertices, 0 ) ; 
        
    
    pVertices[0].p = D3DXVECTOR3(  0, 1.0 * m_fSize, 0.0f );
	pVertices[1].p = D3DXVECTOR3(  0,  0, 0.0f );
	pVertices[2].p = D3DXVECTOR3( 1.0 * m_fSize, 1.0 * m_fSize, 0.0f );
	pVertices[3].p = D3DXVECTOR3( 1.0 * m_fSize,  0, 0.0f);
	
   
   
    pVertices[0].color = clrColor;
	pVertices[1].color = clrColor;;
	pVertices[2].color = clrColor;;
	pVertices[3].color = clrColor;;

    pVertices[0].tu = 0;
	pVertices[1].tu = 0;
	pVertices[2].tu = 1.0;
	pVertices[3].tu = 1.0;

    pVertices[0].tv = 0;
	pVertices[1].tv = 1.0;
	pVertices[2].tv = 0;
	pVertices[3].tv = 1.0;

    m_pVB->Unlock();

}

Billboard::~Billboard()
{

}

void Billboard::Render()

{
     
    // Se configura los estados del render para usar los billboards.
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE);

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        
   
        
     // Establece la matriz necesaria para que el billboard permanezca orientado
     // siempre hacia la camara.
     m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matBillboard);
    
     m_pd3dDevice->SetTexture(0, m_pTexturaBillboard);
    
    // Establece el buffer con la geometría del billboard.
    m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(VERTICE) );
    m_pd3dDevice->SetFVF( VERTICE::FVF_Flags );

    
    m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
					
    // Se recupera el estado anterior para seguir renderizando otros elementos
    // de la escena.
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

}


void Billboard::Actualizar()
{
        // Para actualizar el billboard de forma eficiente, lo que 
        // se hace en vez de establecer una matriz de transformacion,
        // es recalcular las coordenadas de los vertices para que
        // formen un plano alineado con la camara.
        D3DMATRIX mat;
        m_pd3dDevice->GetTransform(D3DTS_VIEW,&mat);
        D3DXVECTOR3 rightVect = D3DXVECTOR3(mat._11,mat._21,mat._31);
        D3DXVECTOR3 upVect = D3DXVECTOR3 (mat._12,mat._22,mat._32);
        D3DXVec3Normalize(&rightVect, &rightVect);
        D3DXVec3Normalize(&upVect, &upVect);
        
        rightVect = rightVect * m_fSize * 0.5f;
        upVect = upVect * m_fSize * 0.5f;
    
    
        VERTICE* pVertices;
        m_pVB->Lock( 0, 0, (void**)&pVertices, 0 ) ; 
        
        pVertices[0].p = m_vPos - rightVect;
	    pVertices[1].p = m_vPos + upVect;
	    pVertices[2].p = m_vPos - upVect;
	    pVertices[3].p = m_vPos + rightVect ;
	    
        m_pVB->Unlock();

}

/* EOF */
