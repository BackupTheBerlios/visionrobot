#include "Captura.h"
#include <stdio.h>
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

Captura::Captura()
{
  CoInitialize(NULL);
  m_pGB = NULL;
  m_pMC = NULL;
  m_pME = NULL;			
  m_pWC = NULL;		
  m_pDF =NULL;
  m_pCamOutPin =NULL;
  m_pFrame=NULL;
  m_nTamFrame=0;
  m_psEstado=PARADO;
}

Captura::~Captura()
{
  Cerrar();
  CoUninitialize( );
}
HRESULT Captura::Iniciar(int iDeviceID,HWND hWnd, int iWidth, int iHeight)
{
  HRESULT hr;
  // Coger la interfaz de DirectShow
  hr=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
		      IID_IGraphBuilder, (void **)&m_pGB);

  if(SUCCEEDED(hr))
    {
      // Crear la superficie de mezcla y meterla en el Graph
      IniciarWindowlessVMR(hWnd);
      // Enlazar el filtro
      if(!EnlazarFiltro(iDeviceID, &m_pDF))
	return S_FALSE;

      hr=m_pGB->AddFilter(m_pDF, L"Video Capture");
      if (FAILED(hr))
	return hr;

      IEnumPins* pEnum;
      m_pDF->EnumPins(&pEnum);

      hr = pEnum->Reset();
      hr = pEnum->Next(1, &m_pCamOutPin, NULL);



      // Preguntar a los interfaces
      hr = m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC);

      hr = m_pGB->QueryInterface(IID_IMediaEventEx, (void **)&m_pME);

      hr = IniciarVentanaVideo(hWnd,iWidth, iHeight);

      m_nTamFrame=iWidth*iHeight*3;
      m_pFrame=(BYTE*) new BYTE[m_nTamFrame];

        
		
      // Ejecutar el Graph

      m_psEstado=PARADO;
        
      hr = m_pGB->Render(m_pCamOutPin);
      hr = m_pMC->Run();
      m_psEstado=EJECUTANDO;

        
    }
  return hr;

}


HRESULT Captura::IniciarWindowlessVMR(HWND hWnd)
{
  IBaseFilter* pVmr = NULL;

  // Crear el VMR y añadirlo al FilterGraph
  HRESULT hr = CoCreateInstance(CLSID_VideoMixingRenderer, NULL,
				CLSCTX_INPROC, IID_IBaseFilter, (void**)&pVmr);
  if (SUCCEEDED(hr)) 
    {
      hr = m_pGB->AddFilter(pVmr, L"Video Mixing Renderer");
      if (SUCCEEDED(hr)) 
        {
	  // Establecer el modo de rendeado
	  IVMRFilterConfig* pConfig;

	  hr = pVmr->QueryInterface(IID_IVMRFilterConfig, (void**)&pConfig);
	  if( SUCCEEDED(hr))
            {
	      pConfig->SetRenderingMode(VMRMode_Windowless);
	      pConfig->Release();
            }

	  hr = pVmr->QueryInterface(IID_IVMRWindowlessControl, (void**)&m_pWC);
	  if( SUCCEEDED(hr))
            {
	      m_pWC->SetVideoClippingWindow(hWnd);

            }
        }
      else {

      }
      pVmr->Release();
    }

  return hr;
}

bool Captura::EnlazarFiltro(int deviceId, IBaseFilter **pFilter)
{
  if (deviceId < 0)
    return false;

  ICreateDevEnum *pCreateDevEnum;
  HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
				IID_ICreateDevEnum, (void**)&pCreateDevEnum);
  if (hr != NOERROR)
    {

      return false;
    }

  IEnumMoniker *pEm;
  hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
					     &pEm, 0);
  if (hr != NOERROR) 
    {
      return false;
    }

  pEm->Reset();
  ULONG cFetched;
  IMoniker *pM;
  int index = 0;
  while(hr = pEm->Next(1, &pM, &cFetched),  hr==S_OK,  index <= deviceId)
    {
      IPropertyBag *pBag;
      hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
      if(SUCCEEDED(hr)) 
	{
	  VARIANT var;
	  var.vt = VT_BSTR;
	  hr = pBag->Read(L"FriendlyName", &var, NULL);
	  if (hr == NOERROR) 
	    {
	      if (index == deviceId)
		{
		  pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
		}
	      SysFreeString(var.bstrVal);
	    }
	  pBag->Release();
	}
      pM->Release();
      index++;
    }
  return true;
}

HRESULT Captura::IniciarVentanaVideo(HWND hWnd,int width, int height)
{

  HRESULT hr;
  RECT rcDest;
	
  IAMStreamConfig * pConfig;
  IEnumMediaTypes *pMedia;
  AM_MEDIA_TYPE *pmt = NULL, *pfnt = NULL;

  hr = m_pCamOutPin->EnumMediaTypes( &pMedia );
  if(SUCCEEDED(hr))
    {

      while(pMedia->Next(1, &pmt, 0) == S_OK)
        {
	  if( pmt->formattype == FORMAT_VideoInfo )
            {
	      VIDEOINFOHEADER *vih = (VIDEOINFOHEADER *)pmt->pbFormat;

	      if( vih->bmiHeader.biWidth == width && vih->bmiHeader.biHeight == height )
                {
		  pfnt = pmt;

		  break;
                }
	      BorrarTipoMedio( pmt );
            }                        
        }
      pMedia->Release();
    }
  hr = m_pCamOutPin->QueryInterface( IID_IAMStreamConfig, (void **) &pConfig );
  if(SUCCEEDED(hr))
    {
      if( pfnt != NULL )
        {
	  hr=pConfig->SetFormat( pfnt );

	  BorrarTipoMedio( pfnt );
        }
      hr = pConfig->GetFormat( &pfnt );
      if(SUCCEEDED(hr))
        {
			
	  m_nAncho = ((VIDEOINFOHEADER *)pfnt->pbFormat)->bmiHeader.biWidth;
	  m_nAlto = ((VIDEOINFOHEADER *)pfnt->pbFormat)->bmiHeader.biHeight;
			
	  BorrarTipoMedio( pfnt );
        }
    }
  ::GetClientRect (hWnd,&rcDest);
  hr = m_pWC->SetVideoPosition(NULL, &rcDest);
  return hr;
}

void Captura::PararCaptura()
{
  HRESULT hresult;

  if((m_psEstado == PAUSADO) || (m_psEstado == EJECUTANDO))
    {
      hresult = m_pMC->Stop();
      m_psEstado = PARADO;
      hresult = m_pMC->Pause();
    }

	   

}

void Captura::Cerrar(void)
{
  HRESULT hr;

  if(m_pMC)
    hr = m_pMC->Stop();	    
  m_psEstado = PARADO;  


  if(m_pCamOutPin)
    m_pCamOutPin->Disconnect ();


  SAFE_RELEASE(m_pCamOutPin);        
  SAFE_RELEASE(m_pMC);    
  SAFE_RELEASE(m_pGB);    
  SAFE_RELEASE(m_pWC);	
  SAFE_RELEASE(m_pDF);

  if(m_pFrame!=NULL)
    delete []m_pFrame;

}

DWORD Captura::GuardarImagen(LPCTSTR szFile)
{
  BYTE *pImage;
  DWORD dwSize,dwWritten;
  dwSize=this->CogerFrame ();
  this->GetFrame (&pImage);
				
  HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

  if (hFile == INVALID_HANDLE_VALUE)
    return FALSE;

  WriteFile(hFile, (LPCVOID)pImage , m_nTamFrame, &dwWritten, 0);
  CloseHandle(hFile);

  return dwWritten;
}

void Captura::BorrarTipoMedio(AM_MEDIA_TYPE *pmt)
{
  if (pmt == NULL) {
    return;
  }

  if (pmt->cbFormat != 0) {
    CoTaskMemFree((PVOID)pmt->pbFormat);
    pmt->cbFormat = 0;
    pmt->pbFormat = NULL;
  }
  if (pmt->pUnk != NULL) {
    pmt->pUnk->Release();
    pmt->pUnk = NULL;
  }

  CoTaskMemFree((PVOID)pmt);
}


DWORD Captura::CogerFrame()
{
  long lOut=-1;
  if(m_pWC ) 
    {
      BYTE* lpCurrImage = NULL;
      if(m_pWC->GetCurrentImage(&lpCurrImage) == S_OK)
        {
			
	  LPBITMAPINFOHEADER  pdib = (LPBITMAPINFOHEADER) lpCurrImage;

	  if(m_pFrame==NULL || (pdib->biHeight * pdib->biWidth * 3) !=m_nTamFrame )
	    {
	      if(m_pFrame!=NULL)
		delete []m_pFrame;

	      m_nTamFrame=pdib->biHeight * pdib->biWidth * 3;
	      m_pFrame=new BYTE [pdib->biHeight * pdib->biWidth * 3] ;
				
				
	    }			

	  if(pdib->biBitCount ==32) 
	    {
	      BYTE *pTemp32;
	      pTemp32=lpCurrImage + sizeof(BITMAPINFOHEADER);

	      this->ConvertirImagen24(pTemp32, m_pFrame, pdib->biSizeImage);
				
	    }

	  CoTaskMemFree(lpCurrImage);
	}
      else
	{
	  return lOut;
	}

    }
  else
    {
      return lOut;
    }

	
   
	
  return lOut=m_nTamFrame;

}

bool Captura::ConvertirImagen24(BYTE *p32Img, BYTE *p24Img,DWORD dwSize32)
{

  if(p32Img != NULL && p24Img != NULL && dwSize32>0)
    {

      DWORD dwSize24;

      dwSize24=(dwSize32 * 3)/4;

      BYTE *pTemp,*ptr;
      pTemp=p32Img + dwSize32 - 1;

      ptr=p24Img;

      for (DWORD index = 0; index < dwSize32/4 ; index++)
	{	
	  (pTemp--);
	  unsigned char r = *(pTemp--);
	  unsigned char g = *(pTemp--);
	  unsigned char b = *(pTemp--);
			
						
	  *(ptr++) = r;
	  *(ptr++) = g;
	  *(ptr++) = b;			

	}	
    }
  else
    {
      return false;
    }

  return true;
}

BOOL Captura::Pausa()
{
  if (!m_pMC)
    return FALSE;


  if(((m_psEstado == PAUSADO) || (m_psEstado == PARADO)) )
    {
      this->PararCaptura();
      if (SUCCEEDED(m_pMC->Run()))
	m_psEstado = EJECUTANDO;
		
    }
  else
    {
      if (SUCCEEDED(m_pMC->Pause()))
	m_psEstado = PAUSADO;
    }

  return TRUE;
}



DWORD Captura::GetFrame(BYTE **pFrame)
{
  if(m_pFrame && m_nTamFrame)
    {
      *pFrame=m_pFrame;
    }
	

  return m_nTamFrame;
}

int  Captura::Enumerar(HWND hList)
{
  if (!hList)
    return  -1;

	
  int id = 0;

  ICreateDevEnum * pCreateDevEnum;
  HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
				IID_ICreateDevEnum, (void**)&pCreateDevEnum);
  if (hr != NOERROR)
    {

      return -1;
    }



  IEnumMoniker *pEm;
  hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
					     &pEm, 0);
  if (hr != NOERROR) 
    {

      return -1 ;
    }

  pEm->Reset();
  ULONG cFetched;
  IMoniker *pM;
  while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
    {
      IPropertyBag *pBag;
      hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
      if(SUCCEEDED(hr)) 
	{
	  VARIANT var;
	  var.vt = VT_BSTR;
	  hr = pBag->Read(L"FriendlyName", &var, NULL);
	  if (hr == NOERROR) 
	    {
	      TCHAR str[2048];		


	      id++;
	      WideCharToMultiByte(CP_ACP,0,var.bstrVal, -1, str, 2048, NULL, NULL);
	      printf("-------%s--------", str);

	      (long)SendMessage(hList, CB_ADDSTRING, 0,(LPARAM)str);

	      SysFreeString(var.bstrVal);
	    }
	  pBag->Release();
	}
      pM->Release();
    }
  return id;
}


//---------------------------------------------------------------------------
