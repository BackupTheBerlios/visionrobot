//---------------------------------------------------------------------------

#ifndef CImagenH
#define CImagenH
#include <math.h>

#ifdef WIN32
#include <windows.h>
#else
typedef unsigned char BYTE;
#endif
//#include <Graphics.hpp>

//---------------------------------------------------------------------------


typedef enum {ORDEN,PARAM} tipoFoto; 

class Imagen{

        private:
                BYTE* m_dibujo;
                int m_ancho;
                int m_alto;
        public:
                Imagen(BYTE* dibujo, int ancho, int alto);
                ~Imagen();
                BYTE* GetDibujo();
                int GetAncho();
                int GetAlto();
};

#endif
