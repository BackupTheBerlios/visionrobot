#include "filtro.h"
#include "pipeline_sdk.h"
#include "ventana_imagen_sdk.h"
#include <stdlib.h>
#include <string.h>

static char buffer_error[128];

static char *filtro_ciclo(modulo_t *modulo, const pipeline_dato_t *in, pipeline_dato_t *out)
{
  filtro_t *filtro = (filtro_t*)modulo->m_dato;
  if (filtro) {
    filtro->m_buffer = (filtro_gestos_in_t *) in->m_dato;
    if(filtro->m_buffer) {
      out->m_tipo = PIPELINE_FILTRO_GESTOS;
      out->m_dato = filtro_gestos_filtrar(filtro);
      sprintf(buffer_error,
	      "IN [%ix%ix%i], OUT [%ix%ix%i]",
	      filtro->m_buffer->m_ancho, filtro->m_buffer->m_alto,
	      filtro->m_buffer->m_bytes, filtro->m_salida->m_ancho,
	      filtro->m_salida->m_alto, filtro->m_salida->m_bytes);
      return buffer_error;
    }
    return "imagen vacía";
  }
  return "fallo en el filtro";
}
static int filtro_valor(const char *tipo, int argc, const char **argv)
{
  int i;
  for (i = 0; i < argc; i += 2) {
    if (!strcmp(tipo, argv[i])) {
      return atoi(argv[i + 1]);
    }
  }
  return 0;
}
static char *filtro_iniciar(modulo_t *modulo, int argc, const char **argv)
{
  if (argc < 12) {
    return "faltan parámetros";
  }
  modulo->m_dato = filtro_gestos_crear();
  filtro_t *filtro = (filtro_t*)modulo->m_dato;
  filtro_gestos_set_color(filtro,
			      filtro_valor("orden_superior_rojo", argc, argv),
			      filtro_valor("orden_inferior_rojo", argc, argv),
			      filtro_valor("orden_superior_verde", argc, argv),
			      filtro_valor("orden_inferior_verde", argc, argv),
			      filtro_valor("orden_superior_azul", argc, argv),
			      filtro_valor("orden_inferior_azul", argc, argv),
			      filtro_valor("param_superior_rojo", argc, argv),
			      filtro_valor("param_inferior_rojo", argc, argv),
			      filtro_valor("param_superior_verde", argc, argv),
			      filtro_valor("param_inferior_verde", argc, argv),
			      filtro_valor("param_superior_azul", argc, argv),
			      filtro_valor("param_inferior_azul", argc, argv));
    return "iniciado";
}
static char *filtro_cerrar(modulo_t *modulo)
{
    filtro_gestos_borrar((filtro_t**)&modulo->m_dato);
    free(modulo);
    return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Filtro";
  modulo->m_iniciar = filtro_iniciar;
  modulo->m_cerrar = filtro_cerrar;
  modulo->m_ciclo = filtro_ciclo;
  modulo->m_dato = 0;
  return modulo;

}
