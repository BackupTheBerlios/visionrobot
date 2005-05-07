#ifndef _PARALELO_H_
#define _PARALELO_H_

typedef enum {PARALELO_LPT1 = 0,
      PARALELO_LPT2} paralelo_puerto_t;

int paralelo_iniciar(paralelo_puerto_t puerto);

void paralelo_sube_pin(int index);

void paralelo_baja_pin(int index);

void paralelo_pin_salida(int index);

void paralelo_terminar();

#endif //_PARALELO_H_
