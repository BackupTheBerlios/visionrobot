//---------------------------------------------------------------------------

#include <windows.h>
#include "Codigo_Filtro.h"

//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#pragma argsused
TPicture* buffer;
TPicture* buffer2;//Provisional

__declspec (dllexport) int ciclo ()
{
    if(buffer==NULL || buffer2==NULL)return -1;
    else {
      buffer= Filtro(buffer,buffer2);
      return 0;
    }
}

__declspec (dllexport) int set_datos(const void * datos) {
        if(datos==NULL)
          return -1;
        else {
          buffer=(TPicture*)datos;
          return 0;
        }
}

__declspec (dllexport) void * get_datos() {
	return buffer;
}

__declspec (dllexport) int iniciar(const void * datos) {
	MessageBox (0, "Iniciando.\n", "Filtro Imagen", MB_ICONINFORMATION);
        buffer2=(TPicture*)datos;
	return 0;

}

__declspec (dllexport) int propiedades() {
	MessageBox (0, "Propiedades.\n", "Filtro Imagen", MB_ICONINFORMATION);
	return 0;

}

__declspec (dllexport) int cerrar(){
	if(buffer!=NULL) delete buffer;
	return 0;

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fwdreason, LPVOID lpvReserved)
{
        return 1;
}
//---------------------------------------------------------------------------
 
