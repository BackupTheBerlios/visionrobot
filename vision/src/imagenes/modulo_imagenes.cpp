/*! \file imagenes.c
    \brief Genera im�genes para el filtro del pipeline.
    
           \section modulo Descripci�n del m�dulo
           Este m�dulo puede generar im�genes de varios tipos:
	   <ul>
	      <li>Reproducir un video.
	      <li>Im�genes aleatorias, creando puntos diseminados de diferentes colores por la imagen
	      <li>Una imagen plana de un color fijo
	      <li>Cargar un archivo de imagen
	      <li>A trav�s de un interfaz de video, generar las imagenes
	   </ul>

	   \section puertos Puertos
	   El m�dulo no tiene puertos de entrada y disponde de dos puertos de salida:
	   <ul>
	      <li><em>salida_imagen</em>: Una estructura de tipo <code>filtro_gestos_in_imagen_t</code>, para un m�dulo que la acepte.
	   </ul>
	   Estas estructuras las crea el m�dulo de im�genes directamente, y tambi�n se encarga de destruirlas.

	   \section argumentos Argumentos
	   Los argumentos que soporta el m�dulo son:
	   <ul>
	     <li><em>video</em>: Si este atributo existe, se carga el video correspondiente.
	     <li><em>camara</em>: Si este atributo existe y es igual a 1, se considera que se toma la imagen de un dispositivo externo.
	     <li><em>alto</em>: El alto de la imagen, obligatorio.
	     <li><em>ancho</em>: El ancho de la imagen, obligatorio.
	     <li><em>bytes</em>: El n�mero de bytes por punto, obligatorio.
	     <li><em>archivo</em>: El archivo que se carga. Si no se especifica, se supone que se quiere generar una imagen de un color.
	     <li><em>rojo</em>: La componente roja de la imagen generada.
	     <li><em>verde</em>: La componente verde de la imagen generada. 
	     <li><em>azul</em>: La componente azul de la imagen generada. Si alguno de los tres colores anteriores no se especifica, se supone que se quiere generar una imagen de colores aleatorios.
	   </ul>
    \author Carlos Le�n
    \version 1.0
*/  
    
#include "pipeline_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "imagenes.h"

/*! \brief El nombre del puerto de salida de una imagen */
#define PUERTO_IMAGEN "salida_imagen"


//! Realiza un ciclo de im�genes. El pipeline se encarga de llamar a esta funci�n.
/*! 
  
\param modulo El m�dulo en el que se realiza en ciclo.
\param puerto El puerto por el que le llega la informaci�n (0 si no recibe nada).
\param value El valor que le llega a trav�s del puerto (0 si no recibe nada).

\return Una cadena que representa un mensaje de respuesta, depuraci�n, informaci�n o error. Tambi�n puede ser 0.
*/
static char *modulo_imagenes_ciclo(modulo_t* modulo, const char *puerto, const void *value){
  imagenes_frame((imagenes_t*)modulo->m_dato);
  return 0;
}
//! Funci�n que llama el pipeline y sirve para iniciar el interfaz de im�gnenes.
/*!  Crea la memoria necesaria y ajusta los par�metros pertinenetes del m�dulo, seg�n est� especificado en el XML.
  
\param modulo El m�dulo que se inicia.
\param argumentos Una <code>GHashTable</code> de GLib, que contiene los argumentos en forma de "nombre de argumento" -> valor.

\return Una cadena que representa un mensaje de respuesta, depuraci�n, informaci�n o error. Tambi�n puede ser 0.
*/
static char *modulo_imagenes_iniciar(modulo_t* modulo, GHashTable *argumentos)
{
	frame_imagen_t *dir;
  char *devolver = "iniciado";  
    imagenes_t * i = 0;



  char * rojo = (char*)g_hash_table_lookup(argumentos,"rojo");
  char * verde = (char*)g_hash_table_lookup(argumentos,"verde");
  char * azul = (char*)g_hash_table_lookup(argumentos,"azul");  
  char *video = (char*)g_hash_table_lookup(argumentos,"video");
  char *camara = (char*)g_hash_table_lookup(argumentos,"camara");
  int alto = atoi((char*)g_hash_table_lookup(argumentos,"alto"));
  int ancho = atoi((char*)g_hash_table_lookup(argumentos,"ancho"));
  int bytes = atoi((char*)g_hash_table_lookup(argumentos,"bytes"));
  char * archivo_imagen = (char*)g_hash_table_lookup(argumentos,"archivo");

  if(video) {
    i = imagenes_iniciar(VIDEO, ancho, alto, bytes, 0, 0, video, 0, 0, 0);
  }
  else if(camara){
    int cam = atoi(camara);
    i = imagenes_iniciar(CAMARA, ancho, alto, bytes, 0, cam, 0, 0, 0, 0);
  }
  else if(archivo_imagen) {
    i = imagenes_iniciar(IMAGEN_FIJA, ancho, alto, bytes, archivo_imagen, 0, 0, 0, 0, 0);
  }
  else if(rojo && verde && azul) {
    color_t i_rojo = (color_t)atoi(rojo);
    color_t i_verde = (color_t)atoi(verde);
    color_t i_azul = (color_t)atoi(azul);
    i = imagenes_iniciar(COLOR_PLANO, ancho, alto, bytes, 0, 0, 0, i_rojo, i_verde, i_azul);
  }
  else {
        i = imagenes_iniciar(ALEATORIO, ancho, alto, bytes, 0, 0, 0, 0, 0, 0);
  }
      modulo->m_dato = i;
  
  dir = imagenes_frame(i);
  g_hash_table_insert(modulo->m_tabla, PUERTO_IMAGEN, dir);

  return devolver;
}

//! Cierra un m�dulo de im�genes
/*! Libera toda la memoria creada por el m�dulo, y el mismo m�dulo.
  
\param modulo El m�dulo que cerramos

\return Una cadena que representa un mensaje de respuesta, depuraci�n, informaci�n o error. Tambi�n puede ser 0.
*/
static char *modulo_imagenes_cerrar(modulo_t* modulo)
{
  imagenes_t *i = (imagenes_t *)modulo->m_dato;
  imagenes_cerrar(&i);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Imagenes";
  modulo->m_iniciar = modulo_imagenes_iniciar;
  modulo->m_cerrar = modulo_imagenes_cerrar;
  modulo->m_ciclo = modulo_imagenes_ciclo;
  modulo->m_dato = 0;  
  return modulo;
}
