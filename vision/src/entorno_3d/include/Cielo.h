/** --------------------------------------------------------------------------------
// 
// @author Jorge Mendoza 2004.
//
// @brief  Cielo en 3d formado por un plano curvado.
//
*/ 

#pragma once

#include <d3dx9.h>
#include <string>


class Cielo  
{
public:
	Cielo();
	virtual ~Cielo();

public:

    /**
    Crea un plano curvado que forma una boveda que constituye el cielo.

    @param pd3dDevice Device sobre el que se mostrara el cielo.
    @param strTextura Nombre del archivo con la textura del cielo.
    @param fRadioAtmosfera Determina el punto más alto de la boveda.
    @param fRadioPlaneta Determina la longitud de la boveda.
    @param iDivisiones Numero de divisiones que tendra la boveda. Cuantas más
                        tenga mas suave será su geometría (y mas coste al renderizar).
    */
    void CrearCielo(LPDIRECT3DDEVICE9 pd3dDevice,
                    std::string & strTextura,
                    float fRadioAtmosfera,
                    float fRadioPlaneta,
                    int iDivisiones);

    /**
    Renderiza la boveda del cielo.
    */
    void Render();


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
    Buffer de los vertices asociados al cielo.
    */
    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    
    /**
    Buffer de indices de vertics que crean los triangulos.
    */
    LPDIRECT3DINDEXBUFFER9 m_pIB;

    /**
    Puntero al device sobre el que se mostrara el cielo.
    */
    LPDIRECT3DDEVICE9 m_pd3dDevice;

    /**
    Textura del cielo.
    */
    LPDIRECT3DTEXTURE9 m_pTexturaCielo;

    /**
    Numero de vertices que conforman la boveda del cielo.
    */
    int m_iNumVertices;

    /**
    Numero de indices para generar los triangulos de la boveda del cielo.
    */
    int m_iNumIndices;

};
