
#include "Entrada.h"
#include "windows.h"



// Se incializan las variables estaticas de la clase.
Movimientos Entrada::m_Movimiento = {0,0,0,0,0,0,0,0,0,0,0,0,0};
POINT Entrada::m_posicionRatonAnterior = {0,0};
bool Entrada::m_bRatonIzquierdoPulsado = false;
bool Entrada::m_bRatonDerechoPulsado = false;
bool Entrada::m_bTeclaC = false;
bool Entrada::m_bTeclaP = false;
bool Entrada::m_bTeclaM = false;
bool Entrada::m_bTeclaD = false;
bool Entrada::m_bTeclaF = false;
bool Entrada::m_bTeclaX = false;
bool Entrada::m_bTeclaS= false;
bool Entrada::m_bTeclaF1 = false;


Entrada::Entrada()
{

}

Entrada::~Entrada()
{

}


void Entrada::Actualizar()
{
    // Este codigo hace que si se suelta el boton del raton mientras se mueve,
    // se mantenga la inercia del giro. Para evitar este comportamiento
    // sólo habría que poner a cero los valores giroX y giroY cuando no se
    // este pulsando el boton del raton.
    if(m_bRatonIzquierdoPulsado){ 
        
        POINT posicionRaton;
        GetCursorPos(&posicionRaton);

        m_Movimiento.giroX=(float)(posicionRaton.x - m_posicionRatonAnterior.x);
        m_Movimiento.giroY=(float)(posicionRaton.y - m_posicionRatonAnterior.y);

         SetCursorPos(m_posicionRatonAnterior.x, m_posicionRatonAnterior.y);
    }

    if(m_bRatonDerechoPulsado){ 
        
        POINT posicionRaton;
        GetCursorPos(&posicionRaton);

        m_Movimiento.zoom=(float)(posicionRaton.x - m_posicionRatonAnterior.x);
        
         SetCursorPos(m_posicionRatonAnterior.x, m_posicionRatonAnterior.y);
    }

    
     m_Movimiento.abajo     = GetAsyncKeyState(VK_DOWN);
     m_Movimiento.arriba    = GetAsyncKeyState(VK_UP);
     m_Movimiento.derecha   = GetAsyncKeyState(VK_RIGHT);
     m_Movimiento.izquierda = GetAsyncKeyState(VK_LEFT);
     m_Movimiento.zoomMas   = GetAsyncKeyState(VK_ADD);
     m_Movimiento.zoomMenos = GetAsyncKeyState(VK_SUBTRACT);
     m_Movimiento.disparo   = GetAsyncKeyState(VK_SPACE);

   
     // Se actualizan las teclas con funcion de interruptor. 
     // Solo se activara el flag de la tecla cuando se detecta la pulsación
     // y se evita que se quede activado durante el periodo que se mantiene
     // presionada la tecla.
     m_Movimiento.c = (GetAsyncKeyState('C') && ! m_bTeclaC);
     m_bTeclaC = GetAsyncKeyState('C');

     m_Movimiento.pausa = (GetAsyncKeyState('P') && ! m_bTeclaP);
     m_bTeclaP = GetAsyncKeyState('P');

     m_Movimiento.m = (GetAsyncKeyState('M') && ! m_bTeclaM);
     m_bTeclaM = GetAsyncKeyState('M');

     m_Movimiento.d = (GetAsyncKeyState('D') && ! m_bTeclaD);
     m_bTeclaD = GetAsyncKeyState('D');

     m_Movimiento.f1 = (GetAsyncKeyState(VK_F1) && ! m_bTeclaF1);
     m_bTeclaF1 = GetAsyncKeyState(VK_F1);

     m_Movimiento.f = (GetAsyncKeyState('F') && ! m_bTeclaF);
     m_bTeclaF = GetAsyncKeyState('F');

     m_Movimiento.x = (GetAsyncKeyState('X') && ! m_bTeclaX);
     m_bTeclaX = GetAsyncKeyState('X');

      m_Movimiento.s = (GetAsyncKeyState('S') && ! m_bTeclaS);
     m_bTeclaS = GetAsyncKeyState('S');
     



}

void Entrada::SetRatonIzquierdoPulsado(bool bPulsado)
{
    if( !m_bRatonIzquierdoPulsado && bPulsado)
    {
        POINT posicionRaton;
        GetCursorPos(&posicionRaton);
        m_posicionRatonAnterior=posicionRaton;
        ShowCursor(false);
        
        
    }

    if(m_bRatonIzquierdoPulsado && !bPulsado)
    {
        ShowCursor(true);
    }

    m_bRatonIzquierdoPulsado=bPulsado;
}

void Entrada::SetRatonDerechoPulsado(bool bPulsado)
{
    if( !m_bRatonDerechoPulsado && bPulsado)
    {
        POINT posicionRaton;
        GetCursorPos(&posicionRaton);
        m_posicionRatonAnterior=posicionRaton;
        ShowCursor(false);
        
        
    }

    if(m_bRatonDerechoPulsado && !bPulsado)
    {
        ShowCursor(true);
    }

    m_bRatonDerechoPulsado=bPulsado;
}

