#include "pipeline_sdk.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>

#define PUERTO_ENTRADA "entrada_red"
#define PUERTO_SALIDA  "salida_texto"

typedef struct {
    char m_salida[128];
    GArray* m_historial;
    int m_tolerancia;
} gestion_dato_t;


static char *gestion_ciclo(modulo_t *modulo, const char *puerto, const void *entrada)
{
  if(entrada) {
    if(!strcmp(PUERTO_ENTRADA, puerto)) {

      const char *cadena = (const char *)entrada;
      gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;

      if(strcmp(entrada, "no_gesto") != 0){
        //Si la imagen tratada por la red, es reconocida como no_gesto, no se la pasa nada al modulo de salida
        //para que esta no escriba nada, y el ultimo gesto sea el q aun tenga validez.
        g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
      }
      else{
        g_array_append_val(dato->m_historial, cadena);
        g_array_remove_index(dato->m_historial, 0);
        //Se recorre el historial, el elemento que mas se repita se escribe en m_salida
        int i;
        int cont=1; char* actual= g_array_index(dato->m_historial, char*, 0);
        for(i=1; i<dato->m_tolerancia; i++){
           if(strcmp(actual,g_array_index(dato->m_historial, char*, i)) != 0){
             cont--;
             if(!cont){
               actual=g_array_index(dato->m_historial, char*, i);
               cont=1;
             }
           }
           else{
             cont++;
           }
        }
	// Aquí había dato->m_salida = actual, supongo que querías esto
        strcpy(dato->m_salida, actual);

        g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, &dato->m_salida);
      }
    }
  }
  else {
     /*Si la red neuronal no esta enviando datos, el tampoco envia datos al modulo de salida. Para que este
     no escriba nada*/
     g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
  }
  return 0;
}

static char *gestion_iniciar(modulo_t *modulo, GHashTable *argumentos) {
   gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);  

  // Comento esto porque la estructura no tiene ningún m_anterior, tú
  // sabrás si tienes que añadir m_argumento o quitar esta línea
  //char *argumento = g_hash_table_lookup(argumentos,"anterior");
  //strcpy(dato->m_anterior, argumento);

  dato->m_tolerancia = atoi(g_hash_table_lookup(argumentos,"tolerancia"));
  dato->m_historial = g_array_new (FALSE, FALSE, sizeof (char) * 128);
  int i;
  for(i=0; i<dato->m_tolerancia; i++){
    g_array_append_val (dato->m_historial, "no_gesto");
  }

  return "iniciado";
}

static char *gestion_cerrar(modulo_t *modulo)
{
  gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  // Aquí te he añadido el "dato->", que te faltaba
  g_array_free(dato->m_historial, TRUE);
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

