// camara.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "dll_pipeline.h"
#include "Captura.h"

#include <stdio.h>

#define PON_ERROR(x) sprintf(cadena_error, (x));hay_error = 1;

char cadena_error[64];
char hay_error = 0;

typedef struct t_data_in{
        BYTE* m_imagen;
        int m_alto;
        int m_ancho;
		int m_bytes;
}data_in;

data_in datos_salida;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



Captura * c;

int ciclo () {
  c->CogerFrame();  
  int tam = c->GetFrame(&datos_salida.m_imagen);
  datos_salida.m_bytes = (tam / datos_salida.m_alto) / datos_salida.m_ancho;
  sprintf(cadena_error, "%i bytes (%ix%i, %i bytes por punto).\n",
			tam, datos_salida.m_ancho, datos_salida.m_alto, datos_salida.m_bytes);
  hay_error = 1;  
  return 0;
}

int set_datos(const void * datos) {
   return 0;
}

void * get_datos() {	
	if(datos_salida.m_imagen == 0) {
		PON_ERROR("Imagen nula.\n");
	}

	return &datos_salida;
}

int iniciar() {	
	PON_ERROR("Iniciando camara...\n");
	datos_salida.m_alto = 240;
	datos_salida.m_ancho = 320;
	c = new Captura();
    c->Iniciar(0, 0, datos_salida.m_ancho, datos_salida.m_alto);   
    return 0;
} 

int propiedades() {
	PON_ERROR("Propiedades...\n");
    return 0;
}

int cerrar() {
	if(c) delete c;
	c = 0;
    return 0;
}

char * error() {	
	char * dev = hay_error ? cadena_error : 0;
	hay_error = 0;
	return dev;
 }
