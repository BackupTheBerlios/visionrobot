/** --------------------------------------------------------------------------------------
//
//  @author Jorge Mendoza 2004.
//
//  @brief  Esta clase implementa billboards. Son poligonos que representan imagenes 2D que
//          se orientan hacia la camara de forma que siempre son visibles. Se usan para
//          crear diferentes objetos de geometria compleja (como vegetación), luces glow,
//          y otro tipo de efectos.
//
*/

#pragma once

#include <d3dx9.h>
#include <string>
#include "Camara.h"


class Billboard  
{
public:

	Billboard();

    

	virtual ~Billboard();

    Billboard(LPDIRECT3DDEVICE9 pd3dDevice,
                Camara* pCamara,
                D3DXVECTOR3 vPosicion, 
                const std::string& strTextura,
                float fSize,
                D3DXCOLOR clrColor
                );

public:
    
    /**
    Muestra en pantalla el billboard.
    */
    virtual void Render();

    /**
    Actualiza la posicion,rotacion del billboard conforme a la posicion/orientacion
    de la camara actual.
    */
    virtual void Actualizar();

    /**
    Traslada el billboard a la posicion determinada

    @param vPos Nueva posicion del billboard.
    */
    inline virtual void Mover(D3DXVECTOR3 & vPos){ m_vPos = vPos;}



private:
   
    struct VERTICE
    {   
        D3DXVECTOR3 p;
        D3DCOLOR    color;
        FLOAT tu,tv;
        enum FVF{
            FVF_Flags =D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1
        };
    };

private :
    

    /**
    Puntero a la camara hacia la cual se orientará el billboard.
    */
    Camara *  m_pCamara;

      /**
    Buffer de los vertices asociados al billboard
    */
    LPDIRECT3DVERTEXBUFFER9 m_pVB;

    /**
    Puntero al device sobre el que se mostrara el billboard.
    */
    LPDIRECT3DDEVICE9 m_pd3dDevice;

    /**
    Textura del billboard.
    */
    LPDIRECT3DTEXTURE9 m_pTexturaBillboard;

    /**
    Matriz de transformacion del billboard.
    */
    D3DXMATRIX m_matBillboard;
    
  
    /**
    Vector posicion.
    */
    D3DXVECTOR3 m_vPos;

    /**
    Tamaño del billboard.
    */
    float m_fSize;

};
/* EOF */