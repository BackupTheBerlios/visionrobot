/** --------------------------------------------------------------------------------------
//
//  @authorJorge Mendoza 2004.
//
//  @brief  Esta clase implementa billboards2D. A diferencia de los normales, estos poligonos
//          se muestran siempre en pantalla encima de la escena. Se usan para crear menus,
//          huds, mirillas, barras de vida, etc.
//
*/
#pragma once

#include "Billboard.h"

class Billboard2D : public Billboard  
{
public:

	Billboard2D();
    
    Billboard2D(LPDIRECT3DDEVICE9 pd3dDevice,
                D3DXVECTOR3 vPosicion, 
                const std::string& strTextura,
                float fAncho, float fAlto,
                float fAnchoPantalla, float fAltoPantalla,
                D3DXCOLOR clrColor);

	virtual ~Billboard2D();

public:
    
    /**
    Renderiza el billboard en pantalla.
    */
    void Render();

    /**
    Actualiza las propiedades del billboard como son posicion, rotacion,etc.
    */
    void Actualizar();

    /**
    Mueve la imagen a la posicion determinada teniendo en cuenta que ésta representa 
    la posicion de la esquina superior izquierda de la imagen.

    @param x Coordenada horizontal.
    @param y Coordenada vertical.
    */
    void Mover(float x, float y);

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
     
private:

    

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
    Ancho de la imagen
    */
    float m_fAncho;

    /**
    Alto de la imagen
    */
    float m_fAlto;

      /**
    Ancho de la pantalla
    */
    float m_fAnchoPantalla;

    /**
    Alto de la pantalla
    */
    float m_fAltoPantalla;
};

/* EOF */
