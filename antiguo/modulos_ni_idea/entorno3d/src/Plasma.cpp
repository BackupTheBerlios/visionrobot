// Plasma.cpp: implementation of the Plasma class.
//
//////////////////////////////////////////////////////////////////////

#include "Plasma.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Plasma::Plasma()
{

}

Plasma::Plasma          (int iNumParticulas,
                        D3DXVECTOR3 * vPosicion,  
                        LPDIRECT3DDEVICE9 pd3dDevice):SistemaParticulas(iNumParticulas,
                                            vPosicion,  
                                            pd3dDevice)
{

}
Plasma::~Plasma()
{

}


void Plasma::ActualizarEspecifico(int i,VERTICEPUNTO * pVertices)
{
     m_pbParticulas[i].m_vPos = m_pbParticulas[i].m_vPos + m_pbParticulas[i].m_vVel;
     m_pbParticulas[i].m_clrDiffuse.a -=  m_pbParticulas[i].m_fFade;
        

     pVertices[i].v =  m_pbParticulas[i].m_vPos ;
     pVertices[i].color = m_pbParticulas[i].m_clrDiffuse;


}
void Plasma::Resetear(int iParticula)
{
        float fVelX,fVelY,fVelZ,fFade,fColor;
        
        fVelX = -0.1 + (rand()%20) * 0.01;
        fVelY = (rand()%100) * 0.01;
        fVelZ = -0.1 + (rand()%20) * 0.01;
        fFade = 0.04 + (rand()%5) * 0.01;
        fColor = (rand()%5) * 0.1;
        
        m_pbParticulas[ iParticula].m_vPos = *m_pvPosicion;  
        m_pbParticulas[ iParticula].m_vVel = D3DXVECTOR3 (fVelX, fVelY, fVelZ);      
        m_pbParticulas[ iParticula].m_clrDiffuse = D3DXCOLOR (fColor, fColor, 1.0 ,1.0);
        m_pbParticulas[ iParticula].m_fFade = fFade;
}