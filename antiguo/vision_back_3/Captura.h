/*! \file Captura.h
    \brief Clase de gestión de cámaras web, basada en el trabajo de Sagar K.R.
    \autor Carlos León
    \versión 1.0
*/
#ifndef _CAPTURA_H_
#define _CAPTURA_H_


//#include <atlbase.h>
#include <dshow.h>
#include <d3d9.h>
#include <vmr9.h>

/*! \brief Define los distintos estados de ejecución */
enum estado_t {PARADO, PAUSADO, EJECUTANDO, INICIO};

/*! \brief Clase que gestiona los recursos de la cámara */
class Captura
{
public:
   /*! \brief Constructor por defecto, inicia todos los parámetros */
	Captura();
   /*! \brief Rellena un ComboBox con las cámaras conectadas y activadas
       \param combo El handler del combo
       \return El número de cámaras encontradas, -1 en caso de fallo
   */
	int Enumerar(HWND combo);
   /*! \brief Iniciar todo el interfaz con la cámara
       \param id El id de la cámara, que coincide con la opción elegida en el combo de Enumerar()
       \param hwnd El handler del recurso sobre el cual hacer el refresco y el pintado
       \param ancho El ancho de la imagen
       \param alto El alto de la imagen
       \return Un HRESULT que puede ser consultado con SUCCED(hr) y FAILED(hr)
   */
	HRESULT Iniciar(int id,HWND hwnd,int ancho,int alto);
   /*! \brief Guarda la imagen obtenida por CogerFrame() en un buffer
       \param La dirección de un puntero que será rellenado. No hay que borrar este búfer, es mejor trabajar con una copia
       \return El tamaño en bytes del buffer
   */
	DWORD GetFrame(BYTE ** frame);
   /*! \brief Pausa o restaura la captura de imágenes
       \return TRUE si todo ha ido bien, FALSE si ha habido fallo
   */
	BOOL Pausa();
   /*! \brief Guarda una instantánea actual de la cámara como buffer plano en un archivo
       \param archivo La ruta, como char *, del archivo donde se guarda
       \return El número de bytes escritos
   */
	DWORD GuardarImagen(LPCTSTR archivo);
   /*! \brief Captura la cámara en un buffer interno de la clase. <em>No borrar el buffer</em>
       \return El número de bytes leídos
   */
  DWORD CogerFrame();

   /*! \brief Destructor, elimina los recursos */
   virtual ~Captura();

   /*! \brief Devuelve el ancho de la imagen
      \return El ancho de la imagen
   */
   inline int GetAncho() {return m_nAncho;}
   /*! \brief Devuelve el alto de la imagen
      \return El alto de la imagen
   */
   inline int GetAlto() {return m_nAlto;}
protected:

	IGraphBuilder *m_pGB ;
	IMediaControl *m_pMC;
	IMediaEventEx *m_pME ;

	IVMRWindowlessControl9 *m_pWC;
	IPin * m_pCamOutPin;
	IBaseFilter *m_pDF;

	estado_t m_psEstado;

	int m_nAncho;
	int m_nAlto;

	BYTE *m_pFrame;
	long m_nTamFrame;


	bool EnlazarFiltro(int id, IBaseFilter **pFiltro);
	HRESULT IniciarWindowlessVMR(HWND hwnd);
	HRESULT IniciarVentanaVideo(HWND hwnd,int ancho, int alto);
	void PararCaptura();
	void Cerrar();

	void BorrarTipoMedio(AM_MEDIA_TYPE *pmt);
	bool ConvertirImagen24(BYTE *p32img,BYTE *p24img,DWORD dwtam32);
};

#endif
