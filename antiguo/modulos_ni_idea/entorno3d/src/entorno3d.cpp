#include "entorno3d.h"
#include "VentanaPrincipal.h"

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved )
{
   
   
    return TRUE;
}


extern "C" __declspec(dllexport) int iniciar(const char **argumentos)
{

    ventana=new VentanaPrincipal();
    int salida= ventana->Inicializar(1);
    
    return 0;
    

    
}

extern "C" __declspec(dllexport) int ciclo()
{
    ventana->BucleMensajes(0);
    return 0;

}

extern "C" __declspec(dllexport) int cerrar()
{

    delete ventana;
    return 0;
}





extern "C" __declspec(dllexport) int set_datos(const void *datos)
{
 
    return 0;

}

extern "C" __declspec(dllexport) void *get_datos(){ return NULL;}

extern "C" __declspec(dllexport) int propiedades(){ return 0;}

extern "C" __declspec(dllexport) char *error(){ return NULL;}