/*! \file pipeline.h
    \brief Cabecera del m�dulo de pipelines
    \author Carlos Le�n
    \version 0.1
 */

/*! \mainpage
  \section intro Introducci�n
  Esta parte del proyecto es una biblioteca que dota de la funcionalidad
  de la arquitectura en tuber�a, de forma que los diferentes m�dulos
  puedan interactuar de una forma aut�noma.
  \section uso Uso
  Para poder usar la biblioteca simplemente es necesario enlazarla
  En el programa que se est� compilando e incluir la librer�a con un simple
  <code>#include "pipeline.h"</code>.
  \section plataforma Plataforma
  El c�digo de esta biblioteca ha sido programado para compilar sin problemas
  (a priori) en plataformas Win32 y Unix.
  \section licencia Licencia
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#ifdef __cplusplus
extern "C" {
#endif


#define F_CICLO "ciclo"
#define F_INICIAR "iniciar"
#define F_CERRAR "cerrar"
//#define F_PROPIEDADES "propiedades"
/*#define F_SET_DATOS "set_datos"
#define F_GET_DATOS "get_datos"
#define F_ERROR "error"*/

#ifdef WIN32
#include <windows.h>
    typedef HMODULE tipo_handler_t;
#define pipeline_free_library(x) FreeLibrary((x))
#define pipeline_load_library(x) LoadLibrary((x))
#define pipeline_get_function(x, y) GetProcAddress((x), TEXT((y)))
#else
#include <dlfcn.h>
    typedef void *tipo_handler_t;
#define pipeline_free_library(x) dlclose((x))
#define pipeline_load_library(x) dlopen((x), RTLD_LAZY)
#define pipeline_get_function(x, y) dlsym((x),(y))
#endif

    /*! \brief Para el ciclo, propiedades y cerrar */
    //typedef int (*funcion_int) ();
    /*! \brief Para get_datos */
    //typedef void *(*funcion_void_a) ();
    /*! \brief Para set_datos */
    //typedef int (*funcion_int_param_void_a) (const void *);
    /*! \brief Para error */
    //typedef char *(*funcion_char_a) ();
    /*! \brief Para iniciar */
    typedef char* (*funcion_iniciar_t) (int, const char **);
    typedef char* (*funcion_cerrar_t)();
    typedef char* (*funcion_ciclo_t) (const void *, void **);
//    typedef char* (*funcion_propiedades) ();
/*! \brief La estructura de datos de cada elemento del pipe */
    struct elemento_s {
	int m_id;		/*!< El identificador del elemento, coincidir� (esperemos) con su �ndice */

	tipo_handler_t m_handler;	/*!< El interfaz con la biblioteca din�mica */
	char **m_argumentos;	/*!< Una lista de cadenas, que representan argumentos */
	int m_numero_argumentos;
    funcion_iniciar_t m_funcion_iniciar;
    funcion_cerrar_t m_funcion_cerrar;
    funcion_ciclo_t m_funcion_ciclo;
    //funcion_propiedades m_funcion_propiedades;
	//funcion_int m_funcion_ciclo;	/*!< La funci�n para el ciclo */
	//funcion_int_param_char_aa m_funcion_iniciar;	/*!< La funci�n para iniciar */
	//funcion_int m_funcion_propiedades;	/*!< La funci�n de propiedades */
	//funcion_int m_funcion_cerrar;	/*!< La funci�n para cerrar */
	//funcion_void_a m_funcion_get_datos;	/*!< La funci�n para devolver datos */
	//funcion_int_param_void_a m_funcion_set_datos;	/*!< La funci�n para establecer datos */
	//funcion_char_a m_funcion_error;	/*!< La funci�n que devuelve un error */
	//char m_iniciado;	/*!< Indica si el elemento est� iniciado o no */
    //char m_error; /*!< Inidica si ejerce de módulo de errores */
	char *m_ruta /*[64] */ ;	/*!< Una cadena que indica la ruta de la biblioteca */
	char *m_nombre /*[64] */ ;	/*!< Un nombre descriptivo, nada m�s */
	struct elemento_s **m_destino /*[20] */ ;	/*!< Una lista de punteros a otros elementos, para la conexi�n */
	int m_numero_conexiones;	/*!< El n�mero de conexiones actuales */
    };

/*! \brief Declaraci�n del tipo de los elementos para su uso */
    typedef struct elemento_s elemento_t;

/*! \brief La estructura de datos del pipeline */
    struct pipeline_s {
    //    char *m_error;
	//char m_corriendo;	/*!< Si el pipe est� activo o no, m�s que nada para informar */
	int m_numero;		/*!< El n�mero de elementos */
	//int m_error;		/*!< El �ndice del elemento al que se le env�an los mensajes de error */
	elemento_t **m_elemento /*[20] */ ;	/*!< La lista de elementos */
    };

/*! \brief Declaraci�n del tipo del pipeline para su uso */
    typedef struct pipeline_s pipeline_t;

/*! \brief Crea un pipe
    \return Un puntero al pipe creado
*/
    pipeline_t *pipeline_crear();
/*! \brief Crea un m�dulo en el pipe
    \param pipeline El pipe en el se crea
    \param nombre El nombre que aparecer� reflejado
    \param ruta La ruta a la biblioteca din�mica
    \param argumentos La lista de argumentos del pipeline
    \return El nuevo elemento
*/
    elemento_t *pipeline_nuevo(pipeline_t * pipeline, const char *nombre,
			       const char *ruta,
			       /* const char **conexiones, int numero_conexiones, */
			       const char *argumentos[],
			       int numero_argumentos/*, int error*/);
/*! \brief Borra un pipe, y establece el puntero a cero
    \param pipeline El pipe que se borra
    \return Devuelve 0
*/
    int pipeline_vaciar(pipeline_t ** pipeline);
/*! \brief Borra un m�dulo
    \param pipeline El pipe
    \param id El id del m�dulo
    \return Devuelve 0
*/
    int pipeline_borrar(pipeline_t * pipeline, int id);
/*! \brief Guarda un pipeline en XML
    \param pipeline El pipe
    \param archivo La ruta en la que se guarda
    \return Devuelve 0
*/
    int pipeline_guardar(const pipeline_t * pipeline, const char *archivo);
    /*! \brief Carga un pipeline
       \param ruta La ruta del archivo XML    
       \return El nuevo pipeline
     */
    pipeline_t *pipeline_cargar(const char *ruta);
    /*! \brief Carga los s�mbolos de la nueva biblioteca. Si no se puede abrir, las funciones se pondr�n a 0
       \param pipeline El pipeline que contiene los m�dulos
       \param i El id del elemento
     */
    void pipeline_cambiar_biblioteca(pipeline_t * pipeline, int i);
    /*! \brief Conecta dos m�dulos. No se permiten varias conexiones entre dos mismos elementos.
       \param pipeline El pipeline que contiene los m�dulos
       \param origen El �ndice del elemento origen
       \param destino El �ndice del elemento destino
       \return 0 si todo va bien, -1 en caso de error
     */
    int pipeline_conectar(pipeline_t * pipeline, int origen, int destino);
    /*! \brief Desconecta dos m�dulos
       \param pipeline El pipeline que contiene los m�dulos
       \param origen El �ndice del elemento origen
       \param destino El �ndice del elemento destino
       \return 0 si todo va bien, -1 en caso de error
     */
    int pipeline_desconectar(pipeline_t * pipeline, int origen,
			     int destino);

    /*! \brief Cierra la comunicaci�n con la biblioteca, llamando antes a "cerrar"
       \param pipeline El pipeline que contiene los m�dulos
       \param i El �ndice del elemento
       \return -1 en caso de error, 0 si todo va bien
     */
  //  char * pipeline_cerrar_biblioteca(pipeline_t * pipeline, int i);
    /*! \brief Cierra todas las bibliotecas. Llama secuencialmente a pipeline_cerrar_biblioteca.
       \param pipeline El pipeline cuyas bibliotecas cerramos
       \return -1 en caso de error, 0 si todo va bien
     */
    char * pipeline_cerrar_todas(pipeline_t * pipeline);
    /*! \brief Env�a un error al m�dulo de salida de errores del pipe
       \param pipeline El pipeline en el que trabajamos
       \param elemento El elemento que env�a el error
       \param error Una cadena de error, se supone que el m�dulo sabr� tratarla
       \return -1 en caso de error, 0 si todo va bien
     */
    /*int pipeline_error(const pipeline_t * pipeline, int elemento,
		       const char *error);*/
    /*! \brief Un ciclo del pipe
       \param pipeline El pipe
       \return -1 en caso de error, 0 si todo va bien
     */
    char* pipeline_ciclo(const pipeline_t * pipeline);
    /*! \brief Inicia un m�dulo del pipe. Despu�s de llamar a la funci�n de inicio, se llamar� a <code>error()</code>
       \param pipeline El pipe en el que trabajamos
       \param elemento El elemento que iniciamos
       \return -1 en caso de error, 0 si todo va bien
     */
    //char * pipeline_iniciar(const pipeline_t * pipeline, int elemento);
    /*! \brief Detiene un m�dulo del pipe. Despu�s de llamar a la funci�n de parada, se llamar� a <code>error()</code>
       \param pipeline El pipe en el que trabajamos
       \param elemento El elemento que paramos
       \return -1 en caso de error, 0 si todo va bien
     */
//    char * pipeline_parar(const pipeline_t * pipeline, int elemento);
    /*! \brief Abre la ventana de propiedades. Despu�s de llamar a la funci�n de propiedades, el pipeline captar� el mensaje de informaci�n y/o error.
       \param pipeline El pipe en el que trabajamos
       \param elemento El elemento cuya ventana queremos abrir
       \return 0 si todo va bien, -1 si algo falla
     */
 /*   char * pipeline_abrir_propiedades(const pipeline_t * pipeline,
				   int elemento);*/
    /*! \brief Inicia todas las bibliotecas, llamando a iniciar
       \param pipeline El pipe en el que trabajamos
       \return 0 si todo va bien, -1 si algo falla
     */
    char * pipeline_iniciar_todas(pipeline_t * pipeline);

    /*! \brief Borra la lista de argumentos
       \param pipeline El pipe en el que trabajamos
       \param i El elemento cuyos argumentos queremos borrar
     */
    void pipeline_borrar_argumentos(const pipeline_t * pipeline, int i);
    /*! \brief Establece la lista de argumentos
       \param pipeline El pipe en el que trabajamos
       \param i El elemento cuyos argumentos queremos establecer
       \param argumentos Los argumentos que establecemos
       \param numero_argumentos El tamaño del vector
     */
    void pipeline_establecer_argumentos(const pipeline_t * pipeline, int i,
					const char **argumentos,
					int numero_argumentos);
                    
    int pipeline_set_nombre(const pipeline_t *pipeline, int i, const char *nombre);
    int pipeline_set_ruta(pipeline_t *pipeline, int i, const char *ruta);                    
    elemento_t *pipeline_get_elemento_por_nombre(const pipeline_t *p, const char* n);
    
#ifdef __cplusplus
}
#endif
#endif
