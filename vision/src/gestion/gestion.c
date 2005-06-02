/*!
  \file   gestion.c
  \author Carlos León
  \date   Sat Mar 12 19:54:40 2005
  
  \brief  Módulo que se encarga de filtrar la salida de la red, y generar, a partir de ella, una salida coherente y útil.

          \section modulo Descripción del módulo
	       Este módulo filtra los mensajes que le llegan, eliminando el ruido generado por los módulos anteriores. Trabaja con una tabla de "elemento" -> "valor", y premia los más reforzados, de tal modo que las fluctuaciones de la red neuronal y el ocr no afecten a la salida general.

	  \section puertos Puertos
	  El módulo tiene puertos de entrada de salida:
	   <ul>
	      <li><em>entrada_red</em>: Una estructura de tipo <code>char *</code>, es decir, una cadena que representa una salida.
	      <li><em>salida_texto</em>: Una salida de tipo <code>char *</code>, que es la salida ya filtrada. Vale <code>0</code> cuando no hay salida.
	   </ul>

	   \section argumentos Argumentos
	   <ul>
	     <li><em>neutro</em>: Una entrada que corresponde a la señal que va a ser ignorada, y que en ningún caso dará salida.
	     <li><em>tolerancia</em>: El valor que debe tener un valor para ser aceptado.
	     <li><em>maximo</em>: El máximo que puede tener un valor de un elemento.
	   </ul> 
  
*/

#include "pipeline_sdk.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <stdio.h>

#define PUERTO_ENTRADA "entrada_red"
#define PUERTO_SALIDA  "salida_texto"

//! La estructura de datos del contenido propio del módulo.
typedef struct {
  char m_buffer_error[128];	/*!< El búfer que contiene la cadena de salida. */
  char *m_anterior;		/*!< La última cadena de salida, para no repetir. */
  GHashTable* m_historial;	/*!< La tabla que guarda los estados de cada elemento posible de salida, se crea en tiempo de ejcución, según van llegado elementos que no existían. */
  int m_tolerancia;		/*!< El nivel mínimo de estado que debe tener un elemento para ser aceptado. */
  int m_maximo_valor;		/*!< El máximo valor que puede tener un elemento en su estado (el mínimo es cero). */
  char *m_neutro;		/*!< El nombre del elemento que nunca saldrá por pantalla. */
} gestion_dato_t;

//! El contenido de cada elemento de la tabla de estados. Según esta estructura se decidirá que salida es la apropiada.
typedef struct {
  int m_valor;			/*!< El valor que tiene ese estado en un momento dado. */
} estado_t;

//! Estructura de datos que usamos para manejar la llamada de la función de sumar.
typedef struct {
  estado_t *m_estado; /*!< El estado que cambiamos. */
  char *m_maximo; /*!< La cadena que representa la cadena más usada. */
  int m_valor; /*!< El valor actual del estado funcional en ese momento. */
  int m_maximo_valor; /*!< El valor máximo que puede tomar el estado. */
  char *m_error; /*!< La cadena de error que podemos devolver. */
} argumento_t;

//! Función que usa la tabla hash para sumar los elementos al estado
/*! 
  \param key La clave del elemento.
  \param value El valor del elemento.
  \param user_data Un puntero a <code>struct argumento_t</code>
*/
static void gestion_sumar(gpointer key, gpointer value, gpointer user_data) {
  estado_t* estado = (estado_t*)value;
  argumento_t *argumento = (argumento_t *)user_data;
  if(argumento->m_estado == estado) {
    if(estado->m_valor < argumento->m_maximo_valor) {
      estado->m_valor++;
    }    
  }
  else if(estado->m_valor > 0) {
    estado->m_valor--;
  }  
  if(argumento->m_maximo) {
    if(estado->m_valor > argumento->m_valor) {
      argumento->m_valor = estado->m_valor;
      argumento->m_maximo = key;
    }
  }
  else {
    argumento->m_maximo = key;
    argumento->m_valor = estado->m_valor;
  }
  //sprintf(argumento->m_error, "%s[%s=%i] ", argumento->m_error, (char *)key, estado->m_valor);
}

//! Realiza un ciclo en la gestión
/*!
  \param modulo El módulo del que hacemos el ciclo.
  \param puerto El puerto por el que le llega la información
  \param entrada El dato de entrada
  \return Una cadena que representa un mensaje de error o de información, <code>0</code> en caso de no haber salida.
*/
static char *gestion_ciclo(modulo_t *modulo, const char *puerto, const void *entrada)
{
  char *devolver = 0;
  const char *cadena = (const char *)entrada;
  gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  if(cadena && !strcmp(PUERTO_ENTRADA, puerto)) {
    argumento_t argumento;
    int maximo_valor = dato->m_maximo_valor;
    estado_t * estado = g_hash_table_lookup(dato->m_historial, entrada);
    estado_t *buscar;
    char *resultado;
    // Si esa orden ya existe, actualizamos su estado, si no, la metemos en la tabla;
    // así es más genérico, y vale para cualquier orden  
    if(!estado){      
      estado = (estado_t *)malloc(sizeof(estado_t));
      estado->m_valor = 1;
      g_hash_table_insert(dato->m_historial, /*strdup(*/(char*)cadena/*)*/, estado);
    }
    dato->m_buffer_error [0] = '\0';
    //    argumento = {estado, 0, 0, maximo_valor, dato->m_buffer_error};
    argumento.m_estado = estado;
    argumento.m_maximo = 0;
    argumento.m_valor = 0;
    argumento.m_maximo_valor = maximo_valor;
    argumento.m_error = dato->m_buffer_error;
    g_hash_table_foreach(dato->m_historial, gestion_sumar, &argumento);
    
    buscar = (estado_t *)g_hash_table_lookup(dato->m_historial, argumento.m_maximo);
    resultado = 0;    
    if(buscar->m_valor >= dato->m_tolerancia &&
       argumento.m_maximo != dato->m_anterior) {  
      if(strcmp(argumento.m_maximo, dato->m_neutro)){
	dato->m_anterior = argumento.m_maximo;
	resultado = dato->m_anterior;
      }    
      else {
	resultado = 0;
      }      
    }
    
    devolver = 0;//dato->m_buffer_error;
    g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, resultado);    
  }
  else {
     /* Si la red neuronal no esta enviando datos, el tampoco envia datos al modulo de salida. Para que este
     no escriba nada */
    g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
  }
  return devolver;
}

//! Función que borra una cadena. Se le pasa como argumento a la tabla de entradas, para que al borrarla, libere la memoria.
/*! 
  
\param a La cadena que se borra
*/
/*static void gestion_borrar_cadena(gpointer a) {  
  char *c = (char *)a;
  if(c) {
    g_free(c);
  }
}
*/
//! Función de retrollamada que libera la memoria de un estado
/*! 
  \param a El puntero que borramos
*/
static void gestion_borrar_estado(gpointer a) {  
  estado_t *c = (estado_t *)a;
  if(c) {
    g_free(c);
  }
}


//! Inicia el módulo de gestión, preparando todos los parámetros
/*!   
\param modulo El módulo (de tipo módulo de gestión) que iniciamos.

\return Una cadena que representa un mensaje de respuesta de cualquier tipo.
*/
static char *gestion_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);  
  dato->m_anterior = 0;
  dato->m_neutro = strdup(g_hash_table_lookup(argumentos,"neutro"));
  dato->m_tolerancia = atoi(g_hash_table_lookup(argumentos,"tolerancia"));
  dato->m_maximo_valor = atoi(g_hash_table_lookup(argumentos,"maximo"));
  dato->m_historial = g_hash_table_new_full(g_str_hash,
					    g_str_equal,
					    0,//gestion_borrar_cadena,
					    0/*gestion_borrar_estado*/);
  sprintf(dato->m_buffer_error, "iniciado con neutro = \"%s\", tolerancia  = %i, maximo = %i", 
	  dato->m_neutro,
	  dato->m_tolerancia,
	  dato->m_maximo_valor);
  return dato->m_buffer_error;
}

//! Cierra un módulo de gestión de salida de la red neuronal.
/*! Libera toda la memoria creada por el módulo, y el mismo módulo.
  
\param modulo El módulo (de tipo módulo de gestión) que liberamos.

\return Una cadena que representa un mensaje de respuesta de cualquier tipo.
*/
static char *gestion_cerrar(modulo_t *modulo)
{
  gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  g_hash_table_destroy(dato->m_historial);
  free(dato->m_neutro);
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Gestion";
  modulo->m_iniciar = gestion_iniciar;
  modulo->m_cerrar = gestion_cerrar;
  modulo->m_ciclo = gestion_ciclo;
  modulo->m_dato = (gestion_dato_t *)malloc(sizeof(gestion_dato_t));
  return modulo;

}

