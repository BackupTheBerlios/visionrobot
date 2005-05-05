/** -------------------------------------------------------------------------------------
//
//  @author Jorge Mendoza 2004.
//
//  @brief  Esta clase implementa un sistema de partículas genérico para crear efectos
//          como fuego, humo, etc. 
//
*/

#pragma once
#include <d3dx9.h>



class SistemaParticulas  
{
public:
	SistemaParticulas();
    
    /**
    Constructor de la clase.
    @param iNumParticulas Numero de particulas que contendra el sistema.
    @param vPosicion Posicion del emisor de particulas.
    @param pd3dDevice Device sobre el que se renderiza el sistema de particulas.
    */
    SistemaParticulas   (int iNumParticulas,
                        D3DXVECTOR3 * vPosicion,  
                        LPDIRECT3DDEVICE9 pd3dDevice);

	virtual ~SistemaParticulas();

public :
typedef struct 
    {
        D3DXVECTOR3 m_vPos;      
        D3DXVECTOR3 m_vVel;      

        D3DXVECTOR3 m_vPos0;      
        D3DXVECTOR3 m_vVel0;      
        FLOAT       m_fTime0;     

        D3DXCOLOR   m_clrDiffuse; 
        FLOAT       m_fFade;      

}PARTICULATYPE ;

struct VERTICEPUNTO
{
    D3DXVECTOR3 v;
    D3DCOLOR    color;
    enum FVF{
        FVF_flags =D3DFVF_XYZ | D3DFVF_DIFFUSE
    };
};


public:

    /**
    Renderiza el sistema de particulas.
    */
    void Render();

    /**
    Inicializa el sistema de particulas.
    */
    void Inicializar();

    /**
    Actualiza la posicion, velocidad, color ,etc de las particulas.
    Recorre todas las particulas y llama al metodo virtual ActualizarEspecifico()
    que dependiendo del sistema de particulas concreto, actualizara los valores
    para conseguir diferentes efectos.
    */
    void Actualizar();

    /**
    Actualiza los atributos de las particulas para implementar diferentes efectos.
    Cada sistema de particulas en concreto, debera implementar este metodo
    para adecuar el comportamiento de las particulas.

    @param iNumParticula Indice de la particula que se quiere actualizar.
    @param pVertices Puntero al buffer de vertices asociado a las particulas.
    */
    virtual void ActualizarEspecifico(int iNumParticula,VERTICEPUNTO * pVertices);

    /**
    Resetea una particula, inicializando de nuevo sus valores. Este metodo
    debe ser implementado por cada sistema de particulas en concreto para
    conseguir el efecto  deseado.
    
    @param iParticula Indice de la particula que se desea resetear.
    */
    virtual void Resetear(int iParticula);
    




protected:
    
    /**
    Puntero al buffer que almacena las particulas.
    */
    PARTICULATYPE * m_pbParticulas;

    /**
    Numero de particulas del sistema.
    */
    int m_iNumParticulas;

    /**
    Posicion del emisor del sistema de particulas.
    */
    D3DXVECTOR3 * m_pvPosicion;

    /**
    Buffer de los vertices asociados a cada particula.
    */
    LPDIRECT3DVERTEXBUFFER9 m_pVB;

    /**
    Puntero al device sobre el que se mostrara el sistema de particulas.
    */
    LPDIRECT3DDEVICE9 m_pd3dDevice;

    /**
    Textura de las particulas.
    */
    LPDIRECT3DTEXTURE9 m_pTexturaParticula;

private:

    // Función de ayuda para pasar un valor float a dword.
    inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

  

};




/* EOF */
