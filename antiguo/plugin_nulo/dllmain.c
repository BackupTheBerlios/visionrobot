#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

__declspec (dllexport) int ciclo ()
{
    MessageBox (0, "Mensaje del plugin nulo.\n", "Plugin nulo", MB_ICONINFORMATION);
    return 0;
}

char buffer[128];

__declspec (dllexport) int set_datos(const void * datos) {
  char buff[128];
	sprintf(buff, "Set_Datos: %s.", (char *) datos);
	MessageBox (0, buff, "Plugin nulo", MB_ICONINFORMATION);
	strcpy(buffer, datos);
	return 0;

}

__declspec (dllexport) void * get_datos() {
	MessageBox (0, "Get_Datos.\n", "Plugin nulo", MB_ICONINFORMATION);
	return buffer;
}

__declspec (dllexport) int iniciar() {
	MessageBox (0, "Iniciar.\n", "Plugin nulo", MB_ICONINFORMATION);
	return 0;

}

__declspec (dllexport) int propiedades() {
	MessageBox (0, "Propiedades.\n", "Plugin nulo", MB_ICONINFORMATION);
	return 0;

}

__declspec (dllexport) int cerrar(){
	MessageBox (0, "Cerrar.\n", "Plugin nulo", MB_ICONINFORMATION);
	return 0;

}

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
    switch (reason)
    {
      case DLL_PROCESS_ATTACH:
        break;

      case DLL_PROCESS_DETACH:
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}
