
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FILTRO_GESTOS_VC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FILTRO_GESTOS_VC_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef FILTRO_GESTOS_VC_EXPORTS
#define FILTRO_GESTOS_VC_API __declspec(dllexport)
#else
#define FILTRO_GESTOS_VC_API __declspec(dllimport)
#endif

// This class is exported from the filtro_gestos_vc.dll
class FILTRO_GESTOS_VC_API CFiltro_gestos_vc {
public:
	CFiltro_gestos_vc(void);
	// TODO: add your methods here.
};

extern FILTRO_GESTOS_VC_API int nFiltro_gestos_vc;

FILTRO_GESTOS_VC_API int fnFiltro_gestos_vc(void);

