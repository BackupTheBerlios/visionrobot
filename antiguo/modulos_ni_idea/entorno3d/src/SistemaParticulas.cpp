
#include "SistemaParticulas.h"
#include <string>


SistemaParticulas::SistemaParticulas()
{

}


SistemaParticulas::SistemaParticulas(int iNumParticulas,
                                     D3DXVECTOR3 * vPosicion,
                                     LPDIRECT3DDEVICE9 pd3dDevice
                                     )
{

    m_iNumParticulas = iNumParticulas;
    m_pvPosicion = vPosicion;
    m_pd3dDevice = pd3dDevice;
    
    
}


SistemaParticulas::~SistemaParticulas()
{

}



void SistemaParticulas::Render()
{
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    m_pd3dDevice->SetTransform(D3DTS_WORLD, &matIdentity);
    // Set the render states for using point sprites
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE  );

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    
 
    m_pd3dDevice->SetTexture(0, m_pTexturaParticula);
    m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE);
    m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(5.0f) );
    m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(5.00f) );
    m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.00f) );
    m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.00f) );
    m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.00f) );

    // Set up the vertex buffer to be rendered
    m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(VERTICEPUNTO) );
    m_pd3dDevice->SetFVF( VERTICEPUNTO::FVF_flags );

    
    m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, 0, m_iNumParticulas );
					
    m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

void SistemaParticulas::Inicializar()
{

    // Carga la textura de las particulas.
    D3DXCreateTextureFromFile( m_pd3dDevice,
                                std::string("particle2.bmp").c_str(), 
                                &m_pTexturaParticula  ) ;

    // Crea el buffer de vertices.
    m_pd3dDevice->CreateVertexBuffer   (m_iNumParticulas*sizeof(VERTICEPUNTO),
                                        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,
                                        VERTICEPUNTO::FVF_flags,
                                        D3DPOOL_DEFAULT, &m_pVB, NULL );

    
    // Inicializa el buffer de particulas.
    m_pbParticulas = new PARTICULATYPE [m_iNumParticulas];

    for (int i = 0; i < m_iNumParticulas; i++)
    {
        
        
        Resetear(i);
        
        
    }

    
}


void SistemaParticulas::Actualizar()
{

    VERTICEPUNTO* pVertices;
    
    m_pVB->Lock( 0, 0, (void**)&pVertices, NULL );

    for (int i=0; i<m_iNumParticulas ; i++)

    {

       
           ActualizarEspecifico(i, pVertices);
       
               
         if(m_pbParticulas[i].m_clrDiffuse.a <= 0.0) Resetear(i);

    }

    m_pVB->Unlock();

}


void SistemaParticulas::ActualizarEspecifico(int i,VERTICEPUNTO * pVertices)
{
     m_pbParticulas[i].m_vPos = m_pbParticulas[i].m_vPos + m_pbParticulas[i].m_vVel;
     m_pbParticulas[i].m_clrDiffuse.a -=  m_pbParticulas[i].m_fFade;
        

     pVertices[i].v =  m_pbParticulas[i].m_vPos ;
     pVertices[i].color = m_pbParticulas[i].m_clrDiffuse;


}
void SistemaParticulas::Resetear(int iParticula)
{
        float fVelX,fVelY,fVelZ,fFade,fColor;
        
        fVelX = -0.1 + (rand()%20) * 0.01;
        fVelY = (rand()% 50) * 0.01;
        fVelZ = -0.1 + (rand()%20) * 0.01;
        fFade = 0.03 + (rand()%5) * 0.01;
        fColor = (rand()%5) * 0.1;
        
        m_pbParticulas[ iParticula].m_vPos = *m_pvPosicion;  
        m_pbParticulas[ iParticula].m_vVel = D3DXVECTOR3 (fVelX, fVelY, fVelZ);      
        m_pbParticulas[ iParticula].m_clrDiffuse = D3DXCOLOR (0.8+ fColor,0.2+fColor ,fColor ,1.0);
        m_pbParticulas[ iParticula].m_fFade = fFade;
}