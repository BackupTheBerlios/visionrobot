/** --------------------------------------------------------------------------------------
//
//  @author Jorge Mendoza 2004.
//
//  @brief  Implementa un plano con propiedades de transparencia, etc que represeta agua
//
*/

#pragma once

#include <d3dx9.h>
#include <string>
#include "Camara.h"

class Agua  
{
public:
	Agua();
	virtual ~Agua();

public:
    
    /**
    Crea un plano de tamaño iTam*iTam que corresponde al agua.
    
    @param pd3dDevice Device sobre el que se renderiza el agua.
    @param iTam Dimension del plano del agua:iTam*iTam.
    @param strTextura Nombre del archivo de textura.
    */
    void CrearAgua(LPDIRECT3DDEVICE9 pd3dDevice, int iTam, std::string & strTextura);

    /**
    Muestra en pantalla el agua.
    */
    virtual void Render();

private:
   
    struct VERTICE
    {   
        D3DXVECTOR3 p;
        D3DXVECTOR3 n;
        D3DCOLOR    color;
        FLOAT tu,tv;
        enum FVF{
            FVF_Flags =D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1
        };
    };

private :
    


    /**
    Buffer de los vertices asociados al agua
    */
    LPDIRECT3DVERTEXBUFFER9 m_pVB;

    /**
    Puntero al device sobre el que se mostrara el agua.
    */
    LPDIRECT3DDEVICE9 m_pd3dDevice;

    /**
    Textura del agua.
    */
    LPDIRECT3DTEXTURE9 m_pTexturaAgua;

    /**
    Matriz de transformacion del agua.
    */
    D3DXMATRIX m_matAgua;
    
  
    /**
    Vector posicion.
    */
    D3DXVECTOR3 m_vPos;

    /**
    Tamaño del agua.
    */
    float m_fSize;
};




/* EOF */