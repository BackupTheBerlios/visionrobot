/** -----------------------------------------------------------------------------
// @author Jorge Mendoza 2004.
// 
// @brief   Ventana principal de la aplicaci�n. Dentro se controla el flujo principal
//          de la aplicaci�n en el bucle de mensajes de la ventana. Tambi�n se 
//          inicializa todo el apartado de Direct3d.
*/


#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <list>
#include "Objeto.h"
#include "Texto.h"
#include "Camara.h"
#include "Escena.h"



class VentanaPrincipal  
{
public:
	
    /**
    Constructor de la ventana.
    */
    VentanaPrincipal();

    /**
    Destructor de la ventana.
    */
    virtual ~VentanaPrincipal();
    
    /**
    Se inicializa la ventana. Esta funci�n es la que realmente
    crea la ventana y la muestra, por lo que es necesario llamarla
    tras llamar al constructor.
    */
    int Inicializar(int show);
    
    /**
    Crea un nuevo Texto que se mostrar� por pantalla.

     
    @param x Desplazamiento horizontal.
    @param y Desplazamiento vertical
    @param texto Texto que se mostrar�.
    @param centrado Determina si el texto estar� centrado en la pantalla.Si esta a
            false, se utilizara el desplazamiento horizontal especificado, en caso
            contrario se ignora.
    */
    void EscribirTexto(int x, int y, std::string texto, bool centrado);
    
    /**
    Es el bucle principal de la aplicaci�n.
    @return Condicion de salida de la aplicaci�n.
    */
    int BucleMensajes(int accion);
   
    
public:
    /**
    Atributos globales de D3d.
    */
    static LPDIRECT3D9             g_pD3D      ; 
    static LPDIRECT3DDEVICE9       g_pd3dDevice; 
   
   
    static const int ANCHO_VENTANA;
    static const int ALTO_VENTANA;


   
private:

    /**
    Numero de milisegundos que transcurrir� entre llamadas a la logica/fisica
    de la apliacion. Este parametro fijara el tempo de la apliacion con independencia
    del framerate.

    NOTA: Si se hiciera variable, se podria modificar en tiempo de ejecucion para
    conseguir efectos tipo camara lenta/rapida.
    */
    static const DWORD TIEMPO_TICK;

private:    
    /**
   Objeto que engloba todos los elementos de la escena, desde camaras, luces,
   objetos,etc.
   */
   Escena * m_pEscena;
  
    /**
    Handle de la ventana.
    */
    HWND hWnd;
    
     
  
    
    /**
    Dice si la ventana esta en modo fullscreen.
    */
    bool m_PantallaCompleta;

    /**
    Contiene el tiempo desde el ultimo render. 
    */
    DWORD m_dwTiempoUltimoRender;

    /**
    Contador de tiempo para controlar la ejecucion de la apliacion.
    */
    DWORD m_dwTiempo1;

    /**
    Contador de tiempo para controlar la ejecucion de la apliacion.
    */
    DWORD m_dwTiempo2;

    /**
    Contador del tiempo transcurrido desde el ultimo frame.
    */
    DWORD  m_dwTiempoFrame;

    /**
    HACK:QUITAR ESTO ES BASURA.
    Contiene el tiempo desde el ultimo render. 
    */
    DWORD m_dwTiempoUltimoDisparo;

    /**
    Contiene el numero de frames del segundo actual.
    */
    int m_iFrames;

    /**
    Determina si la aplicacion esta en pausa. 
    Cierto: la apliacion esta en pausa.
    Falso: la apliacion esta en ejecucion.
    */
    bool m_bPausa;

    /**
    Indica si est� activada la ayuda en pantalla.
    */
    bool m_bAyuda;

    /**
    Indica si se muestran los frames en la pantalla.
    */
    bool m_bFrames;

private:
    
    /**
    Registra la ventana.
    */
    void RegistraVentana();

    /**
    Crea la ventana tras haber sido registrada.

    @param show Parametro que indica la visibilidad de la ventana (sin importancia).
    @return True si se ha creado la ventana con �xito y false en caso contrario.
    */
    bool CrearVentana(int show);

   

    /**
    M�todo que procesa los mensajes de la ventana.(WIN32)
    */
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                                    WPARAM wParam, LPARAM lParam);

    
    /**
    Inicializa D3D.
    */
    HRESULT InicializarD3D( HWND hWnd );

   

    /**
    Libera los recursos D3D.
    */
    VOID Limpiar();
    
    /**
    Renderiza la escena en pantalla.
    */
    VOID Render();

    

    /**
    Renderiza todos los Textos que estan en m_ListaTexto.
    */
    void RenderizarTexto();

    /**
    Actualiza el mensaje de informacion*/
    void ActualizarInformacion();

    /**
    Actualiza las variables que controlan las diferentes opciones seg�n la entrada
    */
    void ActualizarEstado();

    
};


/* EOF */