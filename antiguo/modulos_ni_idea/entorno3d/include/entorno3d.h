#ifndef _ENTORNO3D_H_
#define _ENTORNO3D_H_

class VentanaPrincipal;

VentanaPrincipal * ventana;

extern "C" __declspec(dllexport) int iniciar();

extern "C" __declspec(dllexport) int ciclo();

extern "C" __declspec(dllexport) int cerrar();

extern "C" __declspec(dllexport) void *get_datos();

extern "C" __declspec(dllexport) int set_datos(const void *datos);

extern "C" __declspec(dllexport) int propiedades();

extern "C" __declspec(dllexport) char *error();

#endif
