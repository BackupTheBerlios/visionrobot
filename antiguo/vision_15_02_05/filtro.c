#include "filtro.h"
#include "red_neuronal_sdk.h"
#include <stdlib.h>
#include <math.h>


filtro_t * filtro_gestos_crear()
{
    filtro_t * filtro = (filtro_t *) malloc(sizeof(filtro_t));
    filtro->m_salida =
	(red_neuronal_in_t *) malloc(sizeof(red_neuronal_in_t));
    filtro->m_salida->m_orden = 0;
    filtro->m_salida->m_tipo_orden = 0;
    filtro->m_orden_param = 0;
    filtro->m_tipo_orden = 0;
    return filtro;
}
void filtro_gestos_borrar(filtro_t ** filtro)
{
  filtro_t *p_filtro = *filtro;
  if (filtro && p_filtro) {
    free(p_filtro->m_salida);
    if(p_filtro->m_orden_param) {
      free(p_filtro->m_orden_param);
    }
    if(p_filtro->m_tipo_orden) {
      free(p_filtro->m_tipo_orden);
    }
    free(p_filtro);
    *filtro = 0;
  }
}

red_neuronal_in_t * filtro_gestos_filtrar(filtro_t * filtro, int parametro_difuminado, int reduccion)
{

    if (!filtro || !filtro->m_buffer) {
	filtro->m_salida->m_tipo_orden = 0;
	filtro->m_salida->m_orden = 0;
	return filtro->m_salida;
    }
    int h = filtro->m_buffer->m_alto;
    int w = filtro->m_buffer->m_ancho;
    color_t m_rojoInf_orden = filtro->m_orden.m_inferior.m_r;
    color_t m_rojoSup_orden = filtro->m_orden.m_superior.m_r;
    color_t m_verdeInf_orden = filtro->m_orden.m_inferior.m_v;
    color_t m_verdeSup_orden = filtro->m_orden.m_superior.m_v;
    color_t m_azulInf_orden = filtro->m_orden.m_inferior.m_a;
    color_t m_azulSup_orden = filtro->m_orden.m_superior.m_a;
    color_t m_rojoInf_param = filtro->m_parametro.m_inferior.m_r;
    color_t m_rojoSup_param = filtro->m_parametro.m_superior.m_r;
    color_t m_verdeInf_param = filtro->m_parametro.m_inferior.m_v;
    color_t m_verdeSup_param = filtro->m_parametro.m_superior.m_v;
    color_t m_azulInf_param = filtro->m_parametro.m_inferior.m_a;
    color_t m_azulSup_param = filtro->m_parametro.m_superior.m_a;
    int bytes = filtro->m_buffer->m_bytes;
    color_t *buffer = filtro->m_buffer->m_imagen;

    if(!filtro->m_tipo_orden) {
      filtro->m_tipo_orden = (color_t *) malloc(sizeof(color_t) * h * w * bytes);
    }
    if(!filtro->m_orden_param) {
      filtro->m_orden_param = (color_t *) malloc(sizeof(color_t) * h * w * bytes);
    }
    color_t *tipo_orden = filtro->m_tipo_orden;
    color_t *orden = filtro->m_orden_param;
    int cont, cont2, acX, acX2, acY, acY2, posY;
    cont = cont2 = acX = acX2 = acY = acY2 = 0;
    int y, x, j, i;    
    for (y = 0; y < h; y++) {
      for (x = 0; x < w * bytes; x++) {
	    int rojo = 0;
	    int verde = 0;
	    int azul = 0;
	    
	    int param_bytes = parametro_difuminado * bytes;
	    int param_bytes2 = (parametro_difuminado * bytes) + (bytes - 1);
	    if (y - parametro_difuminado >= 0 && y + parametro_difuminado <= h) {
	      for (i = y - parametro_difuminado; i <= y + parametro_difuminado; i++) {
		if (x - param_bytes >= 0 && x + param_bytes2 <= w * bytes) {
		  for (j = x - param_bytes; j <= x + param_bytes2; j++) {
		    rojo += buffer[(i * w * bytes) + j];
		    verde += buffer[(i * w * bytes) + j + 1];
		    azul += buffer[(i * w * bytes) + j + 2];
		    j += bytes - 1;
		  }
		}
	      }
	      }
		
		int numero = (2 * parametro_difuminado) + 1;
		numero *= numero;
	    rojo = ((int)floor(rojo / numero));
	    verde = ((int) floor(verde / numero));
	    azul = ((int) floor(azul / numero));

	    

	    /* Código de prueba, para depurar, no tiene validez
	       no borrar porque es útil tenerlo
	       Carlos */
	    /*rojo = buffer[y * w * bytes + x];
	    verde = buffer[y * w * bytes + x + 1];
	    azul = buffer[y * w * bytes + x + 2];*/
	    /* Hasta aquí */

	    while (rojo % reduccion && rojo > 1)
	      rojo--;
	    while (verde % reduccion && verde > 1)
	      verde--;
	    while (azul % reduccion && azul > 1)
	    azul--;
	     
	    posY = y * w * bytes;
	    /* Atención: los colores rojo y azul están cambiados,
	       nosotro usamos el orden RGB */
	    color_t azul_comparar = (color_t)rojo;
	    color_t rojo_comparar = (color_t)azul;
	    color_t verde_comparar = (color_t)verde;
	    if (!
		((rojo_comparar >= m_rojoInf_orden && rojo_comparar <= m_rojoSup_orden)
		 && (verde_comparar >= m_verdeInf_orden
		     && verde_comparar <= m_verdeSup_orden)
		 && (azul_comparar >= m_azulInf_orden
		     && azul_comparar <= m_azulSup_orden))) {
	      tipo_orden[posY + x] = 0;
	      tipo_orden[posY + x + 1] = 0;
	      tipo_orden[posY + x + 2] = 0;
	    }
	    
	    else {
	      tipo_orden[posY + x] = 255;
	      tipo_orden[posY + x + 1] = 255;
	      tipo_orden[posY + x + 2] = 255;
	      cont++;
	      acX += x;
	      acY += y;
	    }

	    
	    //Cambiar parametros para verde
	    if (!
		((rojo_comparar >= m_rojoInf_param && rojo_comparar <= m_rojoSup_param)
		 && (verde_comparar >= m_verdeInf_param
		     && verde_comparar <= m_verdeSup_param)
		 && (azul_comparar >= m_azulInf_param
		     && azul_comparar <= m_azulSup_param))) {
	      orden[posY + x] = 0;
	      orden[posY + x + 1] = 0;
	      orden[posY + x + 2] = 0;
	    }
	    
	    else {
	      orden[posY + x] = 255;
	      orden[posY + x + 1] = 255;
	      orden[posY + x + 2] = 255;
	      cont2++;
	      acX2 += x;
	      acY2 += y;
	      }
	    x += bytes - 1;
	}
    }
    if (cont != 0) {
      int difY = ((int) floor(h / 2 - floor(acY / cont)));
      int difX =
	((int) floor(w / 2 - ((int) floor(acX / (bytes * cont)))));
      tipo_orden =
	filtro_gestos_centrar(filtro, tipo_orden, difY, difX);
    }
    if (cont2 != 0) {
      int difY2 = ((int) floor(h / 2 - ((int) floor(acY2 / cont2))));
      int difX2 =
	((int) floor(w / 2 - ((int) floor(acX2 / (bytes * cont2)))));
      orden = filtro_gestos_centrar(filtro, orden, difY2, difX2);	
      }
    filtro->m_salida->m_tipo_orden = tipo_orden;
    filtro->m_salida->m_orden = orden;
    filtro->m_salida->m_ancho = w;
    filtro->m_salida->m_alto = h;
    filtro->m_salida->m_bytes = bytes;
    return filtro->m_salida;
}

void filtro_gestos_set_color(filtro_t * filtro, unsigned char rs, unsigned char ri, unsigned char vs,
			      unsigned char vi, unsigned char as, unsigned char ai, unsigned char rs2, unsigned char ri2,
			     unsigned char vs2, unsigned char vi2, unsigned char as2, unsigned char ai2) {
    filtro->m_orden.m_superior.m_r = rs;
    filtro->m_orden.m_inferior.m_r = ri;
    filtro->m_orden.m_superior.m_v = vs;
    filtro->m_orden.m_inferior.m_v = vi;
    filtro->m_orden.m_superior.m_a = as;
    filtro->m_orden.m_inferior.m_a = ai;
    filtro->m_parametro.m_superior.m_r = rs2;
    filtro->m_parametro.m_inferior.m_r = ri2;
    filtro->m_parametro.m_superior.m_v = vs2;
    filtro->m_parametro.m_inferior.m_v = vi2;
    filtro->m_parametro.m_superior.m_a = as2;
    filtro->m_parametro.m_inferior.m_a = ai2;
} 

char *filtro_gestos_centrar(filtro_t * filtro, char *dibujo, int difY,
				int difX)
{
  int h = filtro->m_buffer->m_alto;
  int w = filtro->m_buffer->m_ancho;
  int bytes = filtro->m_buffer->m_bytes;
  
  if (difY < 0) {
    int y, x;
    for (y = 0; y - difY < h; y++) {
      for (x = 0; x < w * bytes; x++) {
	dibujo[(y * w * bytes) + x] =
	  dibujo[((y - difY) * w * bytes) + x];
      }
    }
    while (y < h) {
      for (x = 0; x < w * bytes; x++) {
	dibujo[(y * w * bytes) + x] = 0;
      }
      y++;
    }
  }
  if (difY > 0) {
    int y, x;
    for (y = h - 1; y - difY >= 0; y--) {
      for (x = 0; x < w * bytes; x++) {
	dibujo[(y * w * bytes) + x] =
	  dibujo[((y - difY) * w * bytes) + x];
      }
    }
    while (y >= 0) {
      for (x = 0; x < w * bytes; x++) {
	dibujo[(y * w * bytes) + x] = 0;
      }
      y--;
    }
  }
  if (difX < 0) {
    int y;
    for (y = 0; y < h; y++) {
      int x;
      for (x = 0; x - (difX * bytes) + bytes - 1 < w * bytes; x++) {
	dibujo[(y * w * bytes) + x] =
	  dibujo[(y * w * bytes) + (x - (difX * bytes))];
	dibujo[(y * w * bytes) + x + 1] =
	  dibujo[(y * w * bytes) + (x - (difX * bytes) + 1)];
	dibujo[(y * w * bytes) + x + 2] =
	  dibujo[(y * w * bytes) + (x - (difX * bytes) + 2)];
	x += bytes - 1;
      }
      while (x + 2 < w * bytes) {
	dibujo[(y * w * bytes) + x] = 0;
	dibujo[(y * w * bytes) + x + 1] = 0;
	dibujo[(y * w * bytes) + x + 2] = 0;
	x += bytes - 1;
      }
    }
  }
  /* He cambiado esto porque si no viola el segmento.
     Antes:
     if(difX > 0) {
     Carlos
  */
  if (difX > 1) {
    int y;
    for (y = 0; y < h - 1; y++) {
      int x;
      for (x = (w * bytes) - 2; x - (difX * bytes) + bytes - 1 >= 0;
	   x--) {
	dibujo[(y * w * bytes) + x] =
	  dibujo[(y * w * bytes) + (x - (difX * bytes))];
	dibujo[(y * w * bytes) + x - 1] =
	  dibujo[(y * w * bytes) + (x - (difX * bytes) - 1)];
	dibujo[(y * w * bytes) + x - 2] =
	  dibujo[(y * w * bytes) + (x - (difX * bytes) - 2)];

	x -= bytes - 1;
      }
      while (x >= 0) {
	dibujo[(y * w * bytes) + x] = 0;
	dibujo[(y * w * bytes) + x - 1] = 0;
	dibujo[(y * w * bytes) + x - 2] = 0;
	x -= bytes - 1;
      }
    }
  }
  return dibujo;
}


