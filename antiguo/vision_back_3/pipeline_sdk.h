#ifndef _PIPELINE_SDK_H_
#define _PIPELINE_SDK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32
#define DLLEXPORT __declspec (dllexport)
#else
#define DLLEXPORT
#endif


typedef struct  {
    DLLEXPORT char* m_nombre;
    DLLEXPORT char* (*m_iniciar) (int argc, const char **argv);
    DLLEXPORT char* (*m_ciclo) (const void *in, void **out);
    DLLEXPORT char* (*m_cerrar) ();
} modulo_t;

typedef modulo_t *(*funcion_get_modulo)();
modulo_t * get_modulo();

void __attribute__ ((constructor)) init(void);
void __attribute__ ((destructor)) fini(void);

#ifdef __cplusplus
}
#endif

#endif //_PIPELINE_SDK_H_
