
#include "Texto.h"


// Inicializacion de los atributos estáticos. 
LPDIRECT3DDEVICE9 Texto::g_pd3dDevice=NULL;
LPD3DXFONT Texto:: m_Fuente=NULL; 
RECT Texto::m_LimitesFuente={0,0,0,0};
int Texto::m_Rojo = 255;
int Texto::m_Verde = 255;
int Texto::m_Azul  =255;

Texto::Texto()
{
   
}

Texto::~Texto()
{

}



void Texto::SetFuente(LPDIRECT3DDEVICE9 d3dDevice, int anchoVentana, int altoVentana)
{
/*
    Texto::m_LimitesFuente.right=anchoVentana;
    Texto::m_LimitesFuente.bottom=altoVentana;

    
    //Inicializa las fuentes para mostrar texto. 
    LOGFONT fuenteLog= {16,0,0,0,FW_NORMAL,false,false,false,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH,"lcdd"};
    D3DXCreateFontIndirect(d3dDevice,&fuenteLog,&m_Fuente);
*/
   
}

void Texto::SetColor(int r, int g, int b)
{
    m_Rojo=r;
    m_Verde=g;
    m_Azul=b;
}
void Texto::DibujarTexto(int x, int y, std::string texto,bool bCentrado)
{
/*
  m_Fuente->Begin();
  
  // Se inicializa el rectangulo en el que se contendra el texto.
  // Si el texto no es centrado, la esquina superior-izquierda determina el principio
  // del texto. Si el texto es centrado, sólo se tiene en cuenta el limite superior
  // para determinar la altura del texto en la pantalla.
  RECT rect=m_LimitesFuente;

  rect.top += y; 
  
  if(bCentrado)
  {
      m_Fuente->DrawText(texto.c_str(),-1,&rect,DT_CENTER,D3DCOLOR_RGBA(m_Rojo,m_Verde,m_Azul,255));
  }
  else
  {
      rect.left+=x;
      m_Fuente->DrawText(texto.c_str(),-1,&rect,DT_LEFT,D3DCOLOR_RGBA(m_Rojo,m_Verde,m_Azul,255));
  }
  
   m_Fuente->End();
*/
}





/* EOF */