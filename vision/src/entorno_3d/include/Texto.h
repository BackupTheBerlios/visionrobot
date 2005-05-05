/** ------------------------------------------------------------------------------
// @author Jorge Mendoza 2004.
//
// @brief   Clase para representar texto usando D3D.
//          El texto podrá ser ubicado por el usuario en cualquier posicion
//          de la pantalla. También se puede espeficificar su color RGB.
//          El texto se muestra llamando al método DibujarTexto() dentro de las
//          instrucciones de BeginScene()/EndScene() del metodo principal Render().
*/


#pragma once

#include <windows.h>
#include <d3dx9.h>
#include <string>




class Texto  
{
public:
	
    Texto();

    virtual ~Texto();
 

    /**
    Establece el color del texto. Todo el texto que se escriba a partir de ahora
    mostrará este color. Valores validos: 0-255
    
    @param r Componente roja.
    @param g Componente verde.
    @param b Componente azul.
    */
    static void SetColor(int r, int g, int b);

    /**
    Dibuja el texto.
    
    @param x Desplazamiento horizontal.
    @param y Desplazamiento vertical
    @param texto Texto que se mostrará.
    @param bCentrado Determina si el texto estará centrado en la pantalla.Si esta a
            false, se utilizara el desplazamiento horizontal especificado, en caso
            contrario se ignora.
    */
    static void DibujarTexto(int x, int y, std::string texto, bool bCentrado);
   
   
    

    /**
    Establece la fuente que se usara para mostrar los textos.
    Es un metodo estático ya que todos los textos comparten la misma fuente. 
    Se debe llamar a este metodo para inicializar los atributos estáticos antes
    de intentar escribir ningún texto.

    @param d3dDevice Device sobre el que se mostrara el texto.
    @param anchoVentana Ancho de la región sobre la que se podrá mostrar texto.
    @param altoVentana Alto de la región sobre la que se podrá mostrar texto.

    */
    static void SetFuente(LPDIRECT3DDEVICE9 d3dDevice, int anchoVentana, 
                            int altoVentana);
public:
	/**
    Componente roja del color del texto.[0-255];
    */
    static int m_Rojo;

    /**
    Componente verde del color del texto.[0-255];
    */
    static int m_Verde;

    /**
    Componente Azul del color del texto.[0-255];
    */
    static int m_Azul;
  
private:
    
    /**
    Device sobre el que se escribe el texto.
    */
    static LPDIRECT3DDEVICE9       g_pd3dDevice; 

    /**
    Atributo que guarda el tipo de fuente con el que se mostrara el texto.
    Todos los objetos Texto comparten la fuente.
    */
    static LPD3DXFONT m_Fuente; 

    /**
    Rectangulo que representa la superficie en la que es posible representar texto.
    En principio se corresponderá con las dimensiones de la ventana, de forma
    que se pueda escribir en cualquier parte de ella.
    */
    static RECT m_LimitesFuente;

 
    


    
};


/* EOF */
