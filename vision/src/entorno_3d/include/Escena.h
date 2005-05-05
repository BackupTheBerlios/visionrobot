/** -------------------------------------------------------------------------------------
//
//  @author Jorge Mendoza 2004.
//
//  @brief  Esta clase engloba todos los elementos que constituyen una escena del programa
//          como son las cámaras, luces, objetos,etc. Se encarga de gestionar la 
//          actualizacion y renderizado de cada uno de ellos.
//
*/

#pragma once

#include <d3dx9.h>
#include "Objeto.h"
#include "Camara.h"
#include "Entrada.h"
#include "Texto.h"
#include "SistemaParticulas.h"
#include "Plasma.h"
#include "Billboard.h"
#include "Billboard2D.h"
#include "Terreno.h"
#include "Cielo.h"
#include "Agua.h"
#include "Luz.h"



class Escena  
{
public:
	Escena();
	virtual ~Escena();

    /**
    Inicializa todos los elementos de la escena como las luces, camaras,objetos,
    etc.

    @param pd3dDevice Device d3d sobre el que se mostrara la escena.
    */
    void Inicializar(LPDIRECT3DDEVICE9 pd3dDevice);

    /**
    Renderiza la escena. Recorre todos los objetos y los presenta en pantalla.
    */
    void Render();

    /**
    Actualiza las propiedades fisicas de la escena, como la velocidad, rotacion,
    direccion,etc de los objetos.

    @param bPausa Si es cierto, solo se llamara a la actualizacion de la camara
    para poder navegar por el escenario en pausa. El resto de elementos de la escena
    no se actualizaran. Si es falso, toda la escena se actualiza.
    */
    void CicloFisica(bool bPausa);

    /**
    Actualiza el valor de las variables que controlan las opciones a la hora
    de mostrar la escena. Está actualización se debe hacer cada vez que se
    actualice la entrada para evitar problemas de pérdida de pulsaciones.
    */
    void ActualizarEstado();

public:
    /** 
   Buffer que almacena un cuadrado que se usará para proyectar las sombras 
   volumetricas. Solo es necesario un cuadrado para crear todas las sombras,
   por ello se puede hacer el atributo estático.
   */
   static LPDIRECT3DVERTEXBUFFER9  m_pGranCuadradoVB;


    
private:
   /**
   Puntero al device sobre el que se muestra la escena.
   */
   LPDIRECT3DDEVICE9       m_pd3dDevice; 
   
  

    /**
    Camara de la escena.
    */
    Camara * m_Camara;

    /**
    Terreno de la escena.
    */
    Terreno * m_pTerreno;

 
    /**
    Cielo de la escena.
    */
    Cielo * m_pCielo;

    /**
    Billboardas para crear el sol
    */
    Billboard * m_pBillboard;
    Billboard * m_pBillboard2;
    Billboard * m_pBillboard3;
    
    /**
    Agua de la escena
    */
    Agua * m_pAgua;
    
    /**
    Objeto de prueba para navegar por el terreno.
    */
    Objeto *m_pAvion;

    /**
    Escenario.
    */
    Objeto *m_pEscenario;

     /**
    Sistena de particulas para FX.
    */
    SistemaParticulas * m_pSistemaParticulas;

    /**
    Luz para recrear el sol.
    */
    Luz * m_pLuz;

  
    
   
    

private:
    /**
    Indica el modo de renderizado de la escena.
    
    Se considera que el valor cierto indica que se renderizará en modo solido
    mientras que el valor de falso indica que se renderizará en modo wireframe.
    */
    static bool m_bModoRenderSolido;

    /**
    Indica si está activado el FX de fuego
    */
    static bool m_bFuego;

    /**
    Indica si está activado el renderizado de sombras en los objetos dinámicos.
    */
    static bool m_bSombra;

    


};

/* EOF */
