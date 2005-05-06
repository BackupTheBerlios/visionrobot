/*! \file entorno_3d.c
    \brief Módulo de entorno 3D para la presentación de la salida de "Visión por computador"

    \author Jorge Mendoza
    \version 1.0
*/  
#include "VentanaPrincipal.h"    
#include "pipeline_sdk.h"
#include "robot_sdk.h"
#include <string.h>

/*! \brief El puerto de entrada de la orden, recibe un <code>char *</code> */
#define PUERTO_ENTRADA "entrada_robot"

// Variable global que guarda el puntero a la ventana de la aplicacion.
VentanaPrincipal * ventana;

static char *entorno_3d_ciclo(modulo_t *modulo, const char *puerto, const void *dato){
  if(!strcmp(PUERTO_ENTRADA, puerto)) 
  {
	  if(dato) {
		ventana->ProcesarComandos(static_cast<const robot_in_t *>(dato));
	  }
  }
  return 0;
}

static char *entorno_3d_iniciar(modulo_t *modulo, GHashTable *argumentos) {
	
  // Se crea la ventana principal y se le pasa los parámetros de configuracion.
  ventana = new VentanaPrincipal();
  ventana->ParsearConfiguracion(argumentos);
  ventana->Inicializar(1);

  return "iniciado";
}
static char *entorno_3d_cerrar(modulo_t *modulo)
{
  
  
   
  ventana->Limpiar();
 
  free(modulo);
  return "cerrado";
}
/*
#ifdef _MSC_VER
	 __declspec(dllexport)
	 #endif*/
modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Entorno 3D";
  modulo->m_iniciar = entorno_3d_iniciar;
  modulo->m_cerrar = entorno_3d_cerrar;
  modulo->m_ciclo = entorno_3d_ciclo;
  modulo->m_dato = 0;
  return modulo;

}


