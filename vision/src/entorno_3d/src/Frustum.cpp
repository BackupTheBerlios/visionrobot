#include "Frustum.h"



Frustum::Frustum()
{

}

Frustum::~Frustum()
{
    if(m_pPlanos)
    {
        delete []m_pPlanos;
    }
}


void Frustum::CrearFrustum()
{
    // Se reserva memoria para los planos del frustum.
    m_pPlanos = new D3DXPLANE[6];

}

void Frustum::ExtraerPlanos(const D3DXMATRIX &matCombinada)
{
   m_pPlanos[IZQUIERDO].a = matCombinada._14 + matCombinada._11;
   m_pPlanos[IZQUIERDO].b = matCombinada._24 + matCombinada._21;
   m_pPlanos[IZQUIERDO].c = matCombinada._34 + matCombinada._31;
   m_pPlanos[IZQUIERDO].d = matCombinada._44 + matCombinada._41;

   m_pPlanos[DERECHO].a = matCombinada._14 - matCombinada._11;
   m_pPlanos[DERECHO].b = matCombinada._24 - matCombinada._21;
   m_pPlanos[DERECHO].c = matCombinada._34 - matCombinada._31;
   m_pPlanos[DERECHO].d = matCombinada._44 - matCombinada._41;

   m_pPlanos[INFERIOR].a = matCombinada._14 + matCombinada._12;
   m_pPlanos[INFERIOR].b = matCombinada._24 + matCombinada._22;
   m_pPlanos[INFERIOR].c = matCombinada._34 + matCombinada._32;
   m_pPlanos[INFERIOR].d = matCombinada._44 + matCombinada._42;

   m_pPlanos[SUPERIOR].a = matCombinada._14 - matCombinada._11;
   m_pPlanos[SUPERIOR].b = matCombinada._24 - matCombinada._21;
   m_pPlanos[SUPERIOR].c = matCombinada._34 - matCombinada._31;
   m_pPlanos[SUPERIOR].d = matCombinada._44 - matCombinada._41;

   m_pPlanos[CERCANO].a = matCombinada._13;
   m_pPlanos[CERCANO].b = matCombinada._23; 
   m_pPlanos[CERCANO].c = matCombinada._33; 
   m_pPlanos[CERCANO].d = matCombinada._43; 

   m_pPlanos[LEJANO].a = matCombinada._14 - matCombinada._13;
   m_pPlanos[LEJANO].b = matCombinada._24 - matCombinada._23;
   m_pPlanos[LEJANO].c = matCombinada._34 - matCombinada._33;
   m_pPlanos[LEJANO].d = matCombinada._44 - matCombinada._43;
    
   for (int i = 0 ; i < 6; i++)
   {
       D3DXPlaneNormalize(&m_pPlanos[i], &m_pPlanos[i]);
   }

}

PuntoVsPlano Frustum::CompararPunto(const D3DXVECTOR3 &vPunto,TipoPlano ePlano)
{
    float distancia = 0.0f;

    distancia = m_pPlanos[ePlano].a * vPunto.x + m_pPlanos[ePlano].b * vPunto.y +
                m_pPlanos[ePlano].c * vPunto.z + m_pPlanos[ePlano].d ;

    if(distancia < 0) return NEGATIVO;
    else if (distancia > 0) return POSITIVO;
    return EN_PLANO;

}