#ifndef _RED_NEURONAL_SDK_H_
#define _RED_NEURONAL_SDK_H_

#define PIPELINE_RED_NEURONAL 0x02

typedef struct {
  char *m_tipo_orden;
  char *m_orden;
  int m_ancho;
  int m_alto;
  int m_bytes;
} red_neuronal_in_t;

#endif				//_RED_NEURONAL_SDK_H_
