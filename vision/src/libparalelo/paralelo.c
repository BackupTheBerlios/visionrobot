#include "paralelo.h"

#include <glib.h>


#ifdef G_OS_WIN32
#include <windows.h>
#define LPT1 0x378
#define LPT2 0x278
typedef void (__stdcall *oupfuncPtr)(short portaddr, short datum);
typedef short (__stdcall *inpfuncPtr)(short portaddr);
static short puerto_paralelo = LPT1;
static HINSTANCE hLib;
static oupfuncPtr oup32;
static inpfuncPtr inp32;
#else
#include <parapin.h>
#endif

typedef enum {P_UP = 0, P_DOWN} estado_t;

int paralelo_iniciar(paralelo_puerto_t puerto) {
	int p = puerto == PARALELO_LPT1 ? LPT1 : LPT2;
#ifdef G_OS_WIN32    
    puerto_paralelo = p;
    hLib = LoadLibrary("inpout32.dll");
    oup32 = (oupfuncPtr) GetProcAddress(hLib, "Out32");
    inp32 = (inpfuncPtr) GetProcAddress(hLib, "Inp32");
	oup32(puerto_paralelo, 0x00);
    return 0;
#else
    return pin_init_user(p);
#endif
}

#ifdef G_OS_WIN32   
static void paralelo_set(estado_t e, int index) {
	short biteado = 0x01 << (index - 2);
	short antes = inp32(puerto_paralelo);
	short resultado = e == P_UP ? antes | biteado : antes & ~biteado;
    oup32(puerto_paralelo, resultado);
}
#endif

void paralelo_sube_pin(int index) {
#ifdef G_OS_WIN32
	paralelo_set(P_UP, index);
#else
  set_pin(LP_PIN[index]);
#endif
}

void paralelo_baja_pin(int index) {
#ifdef G_OS_WIN32
    paralelo_set(P_DOWN, index);
#else
  clear_pin(LP_PIN[index]);
#endif
}

void paralelo_pin_salida(int index) {
#ifdef G_OS_WIN32
#else
  pin_output_mode(LP_PIN[index]);
#endif
}

void paralelo_terminar() {
    #ifdef G_OS_WIN32
    FreeLibrary(hLib);  
    #endif  
}
