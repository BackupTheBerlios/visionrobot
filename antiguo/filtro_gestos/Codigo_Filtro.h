//Realizado por: Diego Sánchez Nieto
//---------------------------------------------------------------------------

#ifndef FiltroH
#define FiltroH
//#include <Graphics.hpp>
#include <windows.h>
#include <Math.h>
#endif

//---------------------------------------------------------------------------

typedef struct t_entrada{
        BYTE* m_imagen;
        int m_alto;
	int m_ancho;
}entrada;

typedef struct t_salida{
        BYTE* tipo_orden;
        BYTE* orden;
}salida;

salida* Filtro(BYTE* buffer, int h, int w);
BYTE* Centrar(BYTE* dibujo, int difY, int difX, int h, int w);
