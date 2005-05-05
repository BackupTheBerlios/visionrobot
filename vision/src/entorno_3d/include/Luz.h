/** --------------------------------------------------------------------------------------
//
//  @author Jorge Mendoza 2004.
//
//  @brief  Clase que agrupa elementos para crear un punto luminoso con efecto
//          glow y con la posibilidad de proyectar lens flares.
//
*/
#pragma once

#include <d3dx9.h>
#include <string>
#include "Billboard.h"
#include "Billboard2D.h"
#include "Camara.h"





class Luz  
{
public:
	Luz();
	virtual ~Luz();

public:

    /**
    Crea una luz a partir del archivo determinado.

    @param pd3dDevice Device sobre el que se renderiza.
    @param strArchivo Nombre del archivo con los datos para crear la luz.
    @param vPos Posicion inicial de la luz.
    @param pCamara Puntero a la camara de la escena para orientar los billboardas de la luz.
    */
    void CrearLuz(  LPDIRECT3DDEVICE9 pd3dDevice,
                    std::string &strArchivo, 
                    D3DXVECTOR3 & vPos,
                    Camara * pCamara);

    /**
    Renderiza la luz.
    */
    void Render();

    /**
    Actualiza los distintos componentes de la luz según el estado de la escena.
    */
    void Actualizar();

    /**
    Establece la posicion de la luz.
    */
    void SetPosicion(D3DXVECTOR3 & vPos);

private:
   
    /**
    Puntero al device sobre el que se muestra la luz.
    */
    LPDIRECT3DDEVICE9       m_pd3dDevice; 
   
    /**
    Posicion de la luz.
    */
    D3DXVECTOR3 m_vPos;
    
    /**
    Numero de Billboards que componen el punto de luz.
    */
    int m_iNumBillboards;

    /**
    Numero de Billboardas2D que forman los flares secundarios en caso de que esta luz
    produzca dicho efecto.
    */
    int m_iNumBillboards2D;

    /**
    Cierto si esta luz produce lens flare y falso en caso contrario.
    */
    bool m_bLensFlares;

    /**
    Puntero a una lista de Billboards que forman el punto luminoso.
    */
    Billboard ** m_pBillboards;

    /**
    Puntero a una lista de Billboardas2D que constituyen los lens flares.
    */
    Billboard2D ** m_pBillboards2D;

    /**
    Determina si la luz esta dentro de la pantalla para dibujar los lens flares.
    */
    bool m_bLuzEnPantalla;



};


/* EOF */
