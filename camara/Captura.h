/*! \file Captura.h
    \brief Clase de gesti�n de c�maras web, basada en el trabajo de Sagar K.R.
    \autor Carlos Le�n
    \versi�n 1.0
*/
#ifndef _CAPTURA_H_INCLUIDO_
#define _CAPTURA_H_INCLUIDO_


#include <atl/atlbase.h>
#include <dshow.h>
#include <d3d9.h>
#include <vmr9.h>

/*! \brief Define los distintos estados de ejecuci�n */
enum estado_t {PARADO, PAUSADO, EJECUTANDO, INICIO};

/*! \brief Clase que gestiona los recursos de la c�mara */
class Captura
{
public:
   /*! \brief Constructor por defecto, inicia todos los par�metros */
	Captura();
   /*! \brief Rellena un ComboBox con las c�maras conectadas y activadas
       \param combo El handler del combo
       \return El n�mero de c�maras encontradas, -1 en caso de fallo
   */
	int Enumerar(HWND combo);
   /*! \brief Iniciar todo el interfaz con la c�mara
       \param id El id de la c�mara, que coincide con la opci�n elegida en el combo de Enumerar()
       \param hwnd El handler del recurso sobre el cual hacer el refresco y el pintado
       \param ancho El ancho de la imagen
       \param alto El alto de la imagen
       \return Un HRESULT que puede ser consultado con SUCCED(hr) y FAILED(hr)
   */
	HRESULT Iniciar(int id,HWND hwnd,int ancho,int alto);
   /*! \brief Guarda la imagen obtenida por CogerFrame() en un buffer
       \param La direcci�n de un puntero que ser� rellenado. No hay que borrar este b�fer, es mejor trabajar con una copia
       \return El tama�o en bytes del buffer
   */
	DWORD GetFrame(BYTE ** frame);
   /*! \brief Pausa o restaura la captura de im�genes
       \return TRUE si todo ha ido bien, FALSE si ha habido fallo
   */
	BOOL Pausa();
   /*! \brief Guarda una instant�nea actual de la c�mara como buffer plano en un archivo
       \param archivo La ruta, como char *, del archivo donde se guarda
       \return El n�mero de bytes escritos
   */
	DWORD GuardarImagen(LPCTSTR archivo);
   /*! \brief Captura la c�mara en un buffer interno de la clase. <em>No borrar el buffer</em>
       \return El n�mero de bytes le�dos
   */
	DWORD CogerFrame();
   /*! \brief Devuelve el bitmap creado con CrearBitmap
       \return Un puntero al TBitmap (VCL)
   */
	inline Graphics::TBitmap * GetBitmap() {return m_pBitmap;}
   /*! \brief Destructor, elimina los recursos */
	virtual ~Captura();
   /*! \brief Transforma el buffer capturado con CogerFrame en un TBitmap de VCL
       \param bInvertido Un booleano que dice si la imagen ser� especular o igual a la original
       \return Un puntero al bitmap
   */
	Graphics::TBitmap *  CrearBitmap(bool bInvertido);
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

	Graphics::TBitmap * m_pBitmap;
	IVMRWindowlessControl9 *m_pWC;
	IPin * m_pCamOutPin;
	IBaseFilter *m_pDF;

	estado_t m_psEstado;

	int m_nAncho;
	int m_nAlto;

	BYTE *m_pFrame;
	long m_nTamFrame;


	bool EnlazarFiltro(int id, IBaseFiltro **pFiltro);
	HRESULT IniciarWindowlessVMR(HWND hwnd);
	HRESULT IniciarVentanaVideo(HWND hwnd,int ancho, int alto);
	void PararCaptura();
	void Cerrar();

	void BorrarTipoMedio(AM_MEDIA_TYPE *pmt);
	bool ConvertirImagen24(BYTE *p32img,BYTE *p24img,DWORD dwtam32);
};

#endif
