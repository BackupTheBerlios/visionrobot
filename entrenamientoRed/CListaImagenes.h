//---------------------------------------------------------------------------

#include "CImagen.h"
#include <fstream.h>
#ifndef CListaImagenesH
#define CListaImagenesH
#endif

//---------------------------------------------------------------------------

const int tamLista= 300;   //Aproximacion

class ListaImagenes{

        private:
                int numImagenes;
                Imagen** lista;
        public:
                ListaImagenes();
                ~ListaImagenes();
                bool InsertarImagen(Imagen* imagen);
                int GetNumImagenes();
                Imagen* GetImagen(int pos);
                void CargarLista(AnsiString fileName);
};
