

#include "Agua.h"



Agua::Agua()
{

}

Agua::~Agua()
{

}

void Agua::CrearAgua(LPDIRECT3DDEVICE9 pd3dDevice, int iTam, std::string & strTextura)
{
    m_pd3dDevice = pd3dDevice;
   

    D3DXMatrixIdentity(&m_matAgua); 
    m_fSize = iTam;
    
    float fAltura = -1950.0f;

    // Se carga la textura.
    D3DXCreateTextureFromFile( m_pd3dDevice, strTextura.c_str(), &m_pTexturaAgua);
    

    m_pd3dDevice->CreateVertexBuffer( 4*sizeof(VERTICE),
                                      D3DUSAGE_WRITEONLY, VERTICE::FVF_Flags,
                                      D3DPOOL_MANAGED, &m_pVB, NULL );
    VERTICE* pVertices;
    m_pVB->Lock( 0, 0, (void**)&pVertices, 0 ) ; 
        
    
    pVertices[0].p = D3DXVECTOR3(  -m_fSize, fAltura, -m_fSize);
	pVertices[1].p = D3DXVECTOR3(  -m_fSize, fAltura, m_fSize  );
	pVertices[2].p = D3DXVECTOR3( m_fSize, fAltura, -m_fSize);
	pVertices[3].p = D3DXVECTOR3( m_fSize, fAltura, m_fSize);
	
    for (int i = 0; i < 4; i++)
    {
   
        pVertices[i].color = D3DXCOLOR (0.20f, 0.40f, 0.5f, 0.9f);
	    pVertices[i].n = D3DXVECTOR3 (0.0f, 1.0f, 0.0f);


    }

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

void Agua::Render()
{
    // Se configura los estados del render para usar los billboards.
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA );

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_ADDSIGNED);
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );
        
    // FIXME: Se crea un material a mano para hacer pruebas.

    D3DMATERIAL9 g_silverMetalMtrl;
     ZeroMemory( &g_silverMetalMtrl, sizeof(D3DMATERIAL9) );

    // Set the material's main color to a silver-like gray color.
    g_silverMetalMtrl.Diffuse.r = 1.0f;
    g_silverMetalMtrl.Diffuse.g = 1.0f;
    g_silverMetalMtrl.Diffuse.b = 1.0f;
    g_silverMetalMtrl.Diffuse.a = 0.8f;

    g_silverMetalMtrl.Emissive.r = 0.2f;
    g_silverMetalMtrl.Emissive.g = 0.2f;
    g_silverMetalMtrl.Emissive.b = 0.2f;
    g_silverMetalMtrl.Emissive.a = 1.0f;

   
    

    // Polished silver can reflect back pure white highlights, so set the 
    // specular to pure white.
    g_silverMetalMtrl.Specular.r = 0.70f;
    g_silverMetalMtrl.Specular.g = 0.50f;
    g_silverMetalMtrl.Specular.b = 0.50f;
    g_silverMetalMtrl.Specular.a = 1.0f;

    // Set the Power value to a small number to make the highlight's size bigger.
    g_silverMetalMtrl.Power = 0.5f;

    // Establece que la informacion del color se extraiga del material actual y no
    // de la información contenida en los vertices.
    m_pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
    
    
            
    m_pd3dDevice->SetMaterial( & g_silverMetalMtrl );
   
   
        
     // Establece la matriz necesaria para que el billboard permanezca orientado
     // siempre hacia la camara.
     m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matAgua);
    
     m_pd3dDevice->SetTexture(0, m_pTexturaAgua);

     
    
    
    // Establece el buffer con la geometría del billboard.
    m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(VERTICE) );
    m_pd3dDevice->SetFVF( VERTICE::FVF_Flags );

    
    m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
					
    // Se recupera el estado anterior para seguir renderizando otros elementos
    // de la escena.
   // m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}