//Realizado por: Diego Sánchez Nieto
//---------------------------------------------------------------------------

#ifndef FiltroH
#define FiltroH
//#include <Graphics.hpp>
//#include <windows.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>
#else
typedef unsigned char BYTE;
#endif

//---------------------------------------------------------------------------


typedef struct t_data_out{
        BYTE* tipo_orden;
        BYTE* orden;
        int ancho;
	      int alto;
	      int bytes;
}data_out;

typedef struct t_data_in{
        BYTE* m_imagen;
        int m_alto;
        int m_ancho;
        int m_bytes;
}data_in;

class Filtro{
        private:
                //int m_Bytes;
                data_out * m_Salida;
                int m_rojoSup_orden,m_rojoInf_orden,m_verdeSup_orden,m_verdeInf_orden,m_azulSup_orden,m_azulInf_orden;
                int m_rojoSup_param,m_rojoInf_param,m_verdeSup_param,m_verdeInf_param,m_azulSup_param,m_azulInf_param;
                BYTE* Centrar(BYTE* dibujo, int difY, int difX, int h, int w, int bytes);
        public:
                Filtro();
                data_out* Filtrar(BYTE* buffer, int h, int w, int bytes);
                void SetColor(int rs,int ri,int vs,int vi,int as,int ai,int rs2,int ri2,int vs2,int vi2,int as2,int ai2);
};


//---------------------------------------------------------------------------

#endif
