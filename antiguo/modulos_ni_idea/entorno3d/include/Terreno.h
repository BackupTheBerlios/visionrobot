/** --------------------------------------------------------------------------------
// 
// @author Jorge Mendoza 2004.
//
// @brief Terreno en 3d generado a partir de un mapa de altura.
//
*/

#pragma once

#include <d3dx9.h>
#include <string>


class Terreno  
{
public:
	Terreno();
	virtual ~Terreno();

    /**
    Crea un terreno de dimensiones iTam*iTam a partir del mapa de altura
    de nombre strMapaAltura. Se supone que está en formato raw y en escala
    de grises (i.e. un byte por pixel). El tamaño del mapa de altura debera
    ser también de iTam*iTam.

    @param strNombreMapam Nombre del archivo que contiene los datos necesarios
                          para crear el terreno. Estos datos son:
                          [archivo_mapa_altura] [archivo_textura] [archivo_textura_detalle]
                          [tamaño_mapa] [escala_mapa] [suavizado_mapa].

    @param pd3dDevice Puntero al device sobre el que se renderiza el terreno.
    */
    void CrearTerreno( std::string & strNombreMapa, LPDIRECT3DDEVICE9 pd3dDevice);

    /**
    Renderiza el terreno.
    */
    void Render();

    /**
    Devuelve la altura del terreno en las coordenadas XZ especificadas.

    @param fX Coordenada X.
    @param fZ Coordenada Z.
    @return Altura del terreno en XZ.
    */
    float GetAltura(float X, float Z);

    /**
    Devuelve la altura interpolada que corresponde a la coordenada XZ del terreno.
    Este valor es más realista que el devuelto por GetAltura(), ya que interpola
    los valores de los vertices más cercanos para aproximar la altura con más precision.

    @param fX Coordenada X.
    @param fY Coordenada Y.
    @return Altura interpolada del terreno en la coordenada XZ.
    */
    float GetAlturaInterpolada(float fX, float fZ);

    /**
    Genera una textura para el terreno actual.
    */
    void GenerarTextura(std::ifstream &fArchivo);
    
    /**
    Actualiza el estado de las variables que controlan las opciones del terreno 
    como son el renderizado de la textura de detalle,etc
    */
    void ActualizarEstado();


private:
   
    struct VERTICE
    {   
        D3DXVECTOR3 p;
        D3DCOLOR    color;
        FLOAT tu,tv;
        FLOAT tu2,tv2;
        //D3DXVECTOR3 n;
        enum FVF{
            FVF_Flags =D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2//|D3DFVF_NORMAL
        };
    };

    typedef struct
    {
        int m_iAlturaInferior;
        int m_iAlturaOptima;
        int m_iAlturaSuperior;

    } REGIONES_TILE;

    typedef struct 
    {
        REGIONES_TILE  m_tRegiones;
        LPDIRECT3DTEXTURE9  m_pTextura;
        D3DLOCKED_RECT  m_LockedRect;
        DWORD * m_pDatos;
    }TILE;


private :
   
    /**
    Buffer de los vertices asociados al terreno
    */
    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    
    /**
    Buffer de indices de vertics que crean los triangulos.
    */
    LPDIRECT3DINDEXBUFFER9 m_pIB;

    /**
    Puntero al device sobre el que se mostrara el terreno.
    */
    LPDIRECT3DDEVICE9 m_pd3dDevice;

    /**
    Textura del terreno.
    */
    LPDIRECT3DTEXTURE9 m_pTexturaTerreno;

    /**
    Textura de detalle del terreno..
    */
    LPDIRECT3DTEXTURE9 m_pTexturaDetalle;

    /**
    Buffer que almacena los datos del mapa de altura.
    */
    BYTE * m_pAlturas;
    /**
    Tamaño del Terreno.
    */
    int m_iTam;

    /**
    Separacion entre los vertices que forman la maya del terreno.
    */
    int m_iEscala;
    
    /**
    Factor de multiplicacion de la altura del mapa. Un valor menor a 1 hace
    que el relieve sea mas suave, mientras que un valor mayor lo hace mas abrupto.
    */
    float m_fSuavizado;
    
    /**
    Tamaño del index buffer.
    */
    long m_lTamIndices;

    /**
    Indica si se aplica una segunda textura de detalle al terreno.
    */
    bool m_bTexturaDetalle;

private:

    /**
    Calcula el peso de un pixel cuando se interpolan varios tiles al crear una textura.
    Según el valor de la altura correspondiente al pixel y a la región definida
    para un tile determinado, se calculará el porcentaje de color que ese pixel compartirá
    con el el pixel correspondiente del tile.
    
    @param iAltura Altura del pixel cuyo peso se quiere calcular.
    @param tRegion Region definida para un tile determinado.
        
    @return Porcentaje de color del tile que afectará a este pixel.
    */
    float CalcularPesoPixel(int iAltura, const REGIONES_TILE & tRegion);

    /**
    Calcula la iluminacion para un pixel. 

    @param iX Coordenada X del vertice al que corresponde el pixel.
    @param iZ Coordenada Z del vertice al que corresponde el pixel.
    @param iDirLuzX Componente X de la direccion de la luz.
    @param iDirLuzZ Componente Z de la direccion de la luz.
    @param fLimiteMinimo Minimo valor de iluminacion que puede tomar el pixel
    @param fLimiteMaximo Maximo valor de iluminacion que puede tomar el pixel.
    @param fSuavidadLuz Factor que controla la oscuridad en las zonas de sombra.

    @return Iluminación del pixel.
    */
    float CalcularIluminacion(  int iX,
                                int iZ,
                                int iDirLuzX,
                                int iDirLuzZ,
                                float fLimiteMinimo,
                                float fLimiteMaximo,
                                float fSuavidadLuz);


    
};
