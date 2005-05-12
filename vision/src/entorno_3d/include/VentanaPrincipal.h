/** -----------------------------------------------------------------------------
// @author Jorge Mendoza 2004.
// 
// @brief   Ventana principal de la aplicación. Dentro se controla el flujo principal
//          de la aplicación en el bucle de mensajes de la ventana. También se 
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
#include "robot_sdk.h"
#include <glib.h>

//#define MODO_DEBUG

class VentanaPrincipal  
{
public:
	
    /**
    Constructor de la ventana.
    */
    VentanaPrincipal();

	/**
	Constructor de la ventana. 
	@param configuracion Puntero a un Hashmap que contiene los datos de configuracion.
	*/

    /**
    Destructor de la ventana.
    */
    virtual ~VentanaPrincipal(){Limpiar();};
    
    /**
    Se inicializa la ventana. Esta función es la que realmente
    crea la ventana y la muestra, por lo que es necesario llamarla
    tras llamar al constructor.
    */
    int Inicializar(int show);
    
    /**
    Crea un nuevo Texto que se mostrará por pantalla.

     
    @param x Desplazamiento horizontal.
    @param y Desplazamiento vertical
    @param texto Texto que se mostrará.
    @param centrado Determina si el texto estará centrado en la pantalla.Si esta a
            false, se utilizara el desplazamiento horizontal especificado, en caso
            contrario se ignora.
    */
    void EscribirTexto(int x, int y, std::string texto, bool centrado);
    
    /**
    Es el bucle principal de la aplicación.
    @return Condicion de salida de la aplicación.
    */
    int BucleMensajes(int accion);
   
	/**
	Actualiza la Entrada según las ordenes recibidas. Las ordenes de componene
	de dos cadenas de texto, la primera indica la acción a realizar y la
	segunda el grado de intensidad. Por ejemplo, se puede recibir una orden
	de rotar hacia la izquierda, con un grado de intensidad alto (giro rápido).
	*/
	void ProcesarComandos(const robot_in_t *comando);

		/**
	Inicializa un timer asincrono que controlara la ejecución de la aplicacion. Cada
	vez que el timer provoque un evento, se llamara a ciclo para acutalizar el estado
	de la simulacion. Esto implica que si se están realizando otro tipo de tareas
	ajenas a este modulo, se truncarán para hacer el renderizado y cuando se acabe
	se proseguira desde el punto en el que se interrumpío.
	*/
	void InicializarTimer();

	/**
	Método auxiliar para parsear la cadena de texto que se pasa como opciones
	de configuracion al construir la ventana
	*/
	void ParsearConfiguracion(GHashTable * configuracion);


	/**
    Libera los recursos D3D.
    */
    VOID Limpiar();
    

	
public:
    /**
    Atributos globales de D3d.
    */
    static LPDIRECT3D9             g_pD3D      ; 
    static LPDIRECT3DDEVICE9       g_pd3dDevice; 
   
   
    static  int ANCHO_VENTANA;
    static  int ALTO_VENTANA;


   
private:

    /**
    Numero de milisegundos que transcurrirá entre llamadas a la logica/fisica
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
    Indica si está activada la ayuda en pantalla.
    */
    bool m_bAyuda;

    /**
    Indica si se muestran los frames en la pantalla.
    */
    bool m_bFrames;

	/** 
	Indica si se activan las sombras dinamicas.
	*/
	bool m_bSombras;

	/** 
	Indica el delay del timer. Será el tiempo que separe la ejecución de un ciclo
	de la aplicacion.
	*/
	DWORD m_DelayTimer;

	/** 
	Indica si se usara la aceleracion 3d en el programa.
	*/
	bool m_Hardware3d;

	/**
	Almacena las ultimas ordenes recibidas desde el exterior.
	*/
	robot_in_t  m_OrdenesRobot;

private:
    
    /**
    Registra la ventana.
    */
    void RegistraVentana();

    /**
    Crea la ventana tras haber sido registrada.

    @param show Parametro que indica la visibilidad de la ventana (sin importancia).
    @return True si se ha creado la ventana con éxito y false en caso contrario.
    */
    bool CrearVentana(int show);

   

    /**
    Método que procesa los mensajes de la ventana.(WIN32)
    */
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                                    WPARAM wParam, LPARAM lParam);

    
    /**
    Inicializa D3D.
    */
    HRESULT InicializarD3D( HWND hWnd );

   

    
    
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
    Actualiza las variables que controlan las diferentes opciones según la entrada
    */
    void ActualizarEstado();

    



};


/* EOF */