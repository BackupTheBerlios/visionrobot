//---------------------------------------------------------------------------

#include <math.h>
#ifndef CImagenH
#define CImagenH
#endif

//---------------------------------------------------------------------------


typedef enum {ORDEN,PARAM} tipoFoto;     //Ampliable

class Imagen{

        private:
                AnsiString nombre;
                TPicture* dibujo;
                TPicture* Filtro(TPicture* dibujo, TPicture* dibujo2);
                TPicture* Centrar(TPicture* dibujo, int difY, int difX);

        public:
                Imagen();
                ~Imagen();
                AnsiString GetNombre();
                TPicture* GetDibujo();
                void Leer(AnsiString fileName);
};
