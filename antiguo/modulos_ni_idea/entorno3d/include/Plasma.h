#pragma once
#include "SistemaParticulas.h"

class Plasma : public SistemaParticulas  
{
public:
	Plasma();
    Plasma              (int iNumParticulas,
                        D3DXVECTOR3 * vPosicion,  
                        LPDIRECT3DDEVICE9 pd3dDevice);
	virtual ~Plasma();

    /*
    Actualiza los atributos de las particulas para implementar diferentes efectos.
    Cada sistema de particulas en concreto, debera implementar este metodo
    para adecuar el comportamiento de las particulas.

    @param iNumParticula Indice de la particula que se quiere actualizar.
    @param pVertices Puntero al buffer de vertices asociado a las particulas.
    */
    void ActualizarEspecifico(int iNumParticula,VERTICEPUNTO * pVertices);

    /*
    Resetea una particula, inicializando de nuevo sus valores. Este metodo
    debe ser implementado por cada sistema de particulas en concreto para
    conseguir el efecto  deseado.
    
    @param iParticula Indice de la particula que se desea resetear.
    */
    void Resetear(int iParticula);

};


