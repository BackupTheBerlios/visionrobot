

#include "Billboard2D.h"



Billboard2D::Billboard2D()
{

}

Billboard2D::Billboard2D(LPDIRECT3DDEVICE9 pd3dDevice,
                D3DXVECTOR3 vPosicion, 
                const std::string& strTextura,
                float fAncho, float fAlto,
                float fAnchoPantalla, float fAltoPantalla,
                D3DXCOLOR clrColor)
{
    m_pd3dDevice = pd3dDevice; 
    m_vPos = vPosicion;

    
    m_fAncho = fAncho;
    m_fAlto= fAlto;
    m_fAnchoPantalla = fAnchoPantalla;
    m_fAltoPantalla = fAltoPantalla;

    m_vPos = vPosicion;
    m_vPos.x -= (m_fAnchoPantalla / 2) - (m_fAncho / 2);
    m_vPos.y -= (m_fAltoPantalla / 2) - (m_fAlto / 2);

    D3DXMatrixTranslation(&m_matBillboard, (float)m_vPos.x, -(float)m_vPos.y, 0.0f);
    
    

    // Se carga la textura.
    D3DXCreateTextureFromFile( m_pd3dDevice, strTextura.c_str(), &m_pTexturaBillboard );
    

    m_pd3dDevice->CreateVertexBuffer( 4*sizeof(VERTICE),
                                      D3DUSAGE_WRITEONLY, VERTICE::FVF_Flags,
                                      D3DPOOL_MANAGED, &m_pVB, NULL );
    VERTICE* pVertices;
    m_pVB->Lock( 0, 0, (void**)&pVertices, 0 ) ; 
        
    
    // Establece la posicion de los vertices.
	pVertices[0].p.x = -(m_fAncho) / 2.0f;
	pVertices[0].p.y = -(m_fAlto) / 2.0f;
	
	pVertices[1].p.x = -(m_fAncho) / 2.0f;
	pVertices[1].p.y = m_fAlto / 2.0f;

	pVertices[2].p.x = (m_fAncho) / 2.0f;
	pVertices[2].p.y = -(m_fAlto) / 2.0f;

	pVertices[3].p.x = (m_fAncho) / 2.0f;
	pVertices[3].p.y = m_fAlto / 2.0f;
	
	pVertices[0].p.z = 1.0f;
	pVertices[1].p.z = 1.0f;
	pVertices[2].p.z = 1.0f; 
	pVertices[3].p.z = 1.0f;



    // Establece el color de los vertices.
   
    pVertices[0].color = clrColor;
	pVertices[1].color = clrColor;;
	pVertices[2].color = clrColor;;
	pVertices[3].color = clrColor;;


    // Establece las coordenadas para la textura.
   	pVertices[0].tu = 0.0f;
	pVertices[0].tv = 1.0f;

	pVertices[1].tu = 0.0f;
	pVertices[1].tv = 0.0f;

	pVertices[2].tu = 1.0f;
	pVertices[2].tv = 1.0f;

	pVertices[3].tu = 1.0f;
	pVertices[3].tv = 0.0f;

    m_pVB->Unlock();


}

Billboard2D::~Billboard2D()
{

}

void Billboard2D::Actualizar()
{
    

}

void Billboard2D::Mover( float x, float y)
{
    m_vPos.x = x;
    m_vPos.y= y;
    m_vPos.x -= (float)(m_fAnchoPantalla / 2.0f) ;
    m_vPos.y -= (float)(m_fAltoPantalla/ 2.0f) ;

    D3DXMatrixTranslation(&m_matBillboard, (float)m_vPos.x, -(float)m_vPos.y, 0.0f);
}


void Billboard2D::Render()
{


    D3DXMATRIX matOrtho; 
    D3DXMATRIX matIdentity;
    
    //Setup the orthogonal projection matrix and the default world/view matrix
    D3DXMatrixOrthoLH(&matOrtho, (float)m_fAnchoPantalla, (float)m_fAltoPantalla, 0.0f, 1.0f);
    D3DXMatrixIdentity(&matIdentity);

    m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
    m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matBillboard);
    m_pd3dDevice->SetTransform(D3DTS_VIEW, &matIdentity);

  
  
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE  );

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


    m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(VERTICE) );
    m_pd3dDevice->SetFVF( VERTICE::FVF_Flags );

	
	m_pd3dDevice->SetTexture(0, m_pTexturaBillboard);
	
	

	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE  );



}