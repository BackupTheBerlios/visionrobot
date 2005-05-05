/** ---------------------------------------------------------------------------
//
// @author Jorge Mendoza 2004.
//
// @brief   Clase estatica que procesa la entrada del raton y teclado ,
//          rellenando una estructura  que contiene las acciones asociadas
//          a dicha entrada. 
//
*/
#pragma once
#include <windows.h>

typedef struct
{
        bool arriba;
        bool abajo;
        bool izquierda;
        bool derecha;
        bool zoomMas;
        bool zoomMenos;
        bool disparo;
        float giroY;
        float giroX;
        float zoom;
        bool c;
        bool pausa;
        bool m;
        bool d;
        bool f;
        bool x;
        bool s;
        bool f1;
		float rotacion;
            
} Movimientos;
    

class Entrada  
{
public:


public:
    
    Entrada();
	virtual ~Entrada();
    
    /**
    Almacena las ordenes de movimiento y angulos de giro.
    */
    static Movimientos m_Movimiento;
    
    /**
    Actualiza la variable m_Movimiento a partir de la informacion de la entrada.
    Normalmente se recogerá del teclado y ratón.
    */
    static void Actualizar();

    /**
    Actualiza el estado de la variable estatica que informa acerca de la pulsacion
    del boton izquierdo del raton.

    @param bPulsado Estado del boton izquierdo del raton.
    */
    static void SetRatonIzquierdoPulsado(bool bPulsado);

    /**
    Actualiza el estado de la variable estatica que informa acerca de la pulsacion
    del boton derecho del raton.

    @param bPulsado Estado del boton derecho del raton.
    */
    static void SetRatonDerechoPulsado(bool bPulsado);

	/** 
	Resetea todos los valores de m_Movimiento. Se debe llamar para actualizar
	los comandos.
	*/
	static void Resetear();

private:

    /**
    Posicion del cursor del raton en la comprobacion anterior.
    */
    static POINT m_posicionRatonAnterior;

    /**
    Determina si el boton izquierdo del raton esta pulsado.
    */
    static bool m_bRatonIzquierdoPulsado;

    /**
    Determina si el boton derecho del raton esta pulsado.
    */
    static bool m_bRatonDerechoPulsado;

    /**
    Variables que controlan si una tecla estaba previamente pulsada para
    implementar la funcion de interruptor.
    */
    static bool m_bTeclaC;
    static bool m_bTeclaP;
    static bool m_bTeclaM;
    static bool m_bTeclaD;
    static bool m_bTeclaF;
    static bool m_bTeclaF1;
    static bool m_bTeclaX;
    static bool m_bTeclaS;

    
};

/* EOF */
