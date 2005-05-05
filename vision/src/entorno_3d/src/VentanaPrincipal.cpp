#include "VentanaPrincipal.h"
#include "Entrada.h"
#include <sstream>
#include "Luz.h"


// Inicialización de las constantes estaticas.
int VentanaPrincipal::ALTO_VENTANA = 480;
int VentanaPrincipal::ANCHO_VENTANA = 640;
const DWORD VentanaPrincipal::TIEMPO_TICK = 15;

// Inicializacin de las variables estaticas;
LPDIRECT3D9 VentanaPrincipal::g_pD3D=NULL;
LPDIRECT3DDEVICE9  VentanaPrincipal::g_pd3dDevice=NULL;

// Declaración de la función que se llamará cuando ocurra un evento del timer.
void CALLBACK OneShotTimer(UINT wTimerID, UINT msg, 
    DWORD dwUser, DWORD dw1, DWORD dw2) ;
 

 

VentanaPrincipal::VentanaPrincipal()
{
    // Se inicializa el tiempo del ultimo render con el tiempo del sistema.
    // Se usará para realizar todos los calculos de periodos de tiempo
    // transcurridos entre diferentes eventos (e.g. fps).
    m_dwTiempoUltimoRender=timeGetTime();
    m_iFrames=0;
    m_dwTiempo1 = timeGetTime();

    // Se inicializa la variable que controla el estado de la apliacion para que 
    // comience en estado de ejecución.
    m_bPausa = false;

    m_bAyuda = false;

    m_bFrames = false;

	m_bSombras = false;
    
   
	// Se parsea los datos de configuracion.
	//ParsearConfiguracion(configuracion);
}


int VentanaPrincipal::Inicializar(int show)
{   
    RegistraVentana();
    

        
    CrearVentana(show);

    
    if(SUCCEEDED(InicializarD3D(hWnd)))
    {
       

            ShowWindow(hWnd, show);
            UpdateWindow(hWnd);
			
			// Se configura y activa el timer que controlara la ejecucion de la aplicacion
			InicializarTimer();

			// Comentar la siguiente instruccion si se controla la ejecucion
			// de cada ciclo externamente llamando a ciclo().
            return  BucleMensajes(0);

            return 1;
    }
    
    else return 0;
     
    
    
    
}

void VentanaPrincipal::RegistraVentana()
{
    WNDCLASSEX wcex;

    memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= GetModuleHandle(0);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.hCursor        = LoadCursorFromFile("cursor.cur");
	wcex.lpszClassName	= "ClaseVentanaPrincipal";
	
	RegisterClassEx(&wcex);


}

bool VentanaPrincipal::CrearVentana(int show)
{
     
	DWORD style;

    RECT WindowRect;
	WindowRect.left=0;
    WindowRect.right = VentanaPrincipal::ANCHO_VENTANA;
	WindowRect.top=0;
    WindowRect.bottom = VentanaPrincipal::ALTO_VENTANA;

    
    m_PantallaCompleta=(MessageBox(NULL,"¿Desea ejecutar en pantalla completa?",    
                                   "Pantalla Completa",MB_YESNO)==IDYES);

	if(m_PantallaCompleta){
		DEVMODE devmode;
		// Cambiar este parametro para usar el stencil buffer de 24
		if(m_bSombras)
		{
			devmode.dmBitsPerPel=24;
		}
		else
		{
				devmode.dmBitsPerPel=16;
		}
        devmode.dmPelsHeight = VentanaPrincipal::ALTO_VENTANA;
        devmode.dmPelsWidth = VentanaPrincipal::ANCHO_VENTANA;
		devmode.dmFields=DM_BITSPERPEL|DM_PELSHEIGHT|DM_PELSWIDTH;

		ChangeDisplaySettings(&devmode,CDS_FULLSCREEN);
		style=WS_POPUP;
		AdjustWindowRect(&WindowRect,style,false);
	}
	else{
		style=WS_OVERLAPPEDWINDOW;
	}  
    
    hWnd = CreateWindowEx(NULL,"ClaseVentanaPrincipal", "ENTORNO VIRTUAL.ROBOT TEST", style,
        0,0,VentanaPrincipal::ANCHO_VENTANA,VentanaPrincipal::ALTO_VENTANA, 
        NULL,NULL, GetModuleHandle(0), NULL);

   if (!hWnd)
   {
      return false;
   }

  
   // Se esconde el cursor. 
   ShowCursor(false);

   return true;
}

VentanaPrincipal::BucleMensajes(int accion)
{
    MSG msg;

    // Comentar para control externo del bucle.
	//while(msg.message!=WM_QUIT){

        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // AQUI VA EL CUERPO DEL PROGRAMA.
            

            // Este bucle controla la sincronizacion de la logica/fisica de la apliacion.
            // Se obliga a que se hagan un numero fijo de actualizaciones de la logica/fisica
            // y se permite renderizar todo lo rapido que el hardware pueda.
            // De esta forma se consigue independencia entre el framerate y la velocidad
            // real de la apliacion. El framerate solo afectara a la fluidez de la 
            // imagen.
            m_dwTiempo2 = timeGetTime();
           
            while((m_dwTiempo2 - m_dwTiempo1)> TIEMPO_TICK )
            {

                // Si el juego esta en pausa no se actualiza la fisica de la escena y por tanto
                // los objetos no se modifican. 
                // TODO: hacer un sistema para llamar de forma constante a la fisica del juego
                // para sincronizacin en sistemas con diferente configuracion.
                
                m_pEscena->CicloFisica(m_bPausa);
               
                m_dwTiempo1 += TIEMPO_TICK;
                 
                // Se actualiza la entrada.
                 // FIXME:La actualizacion de la entrada se deberia hacer
                 // dentro del bucle de forma sincrona, pero esto da problemas
                 // ya que las teclas que funcionan como interrupcion y que se
                 // usan por ejemplo para activar opciones del render, se pierden
                 // ya se puede actualizar varias veces la salida antes de que
                 // se compruebe una vez dentro de Render().
                 // 
                 if( Entrada::m_Movimiento.pausa)
                 {
                        m_bPausa = !m_bPausa;
                 }
             
				 // Comentar si se utiliza control externo para el movimiento.(i.e.
				 // se deshabilita el control directo del teclado y raton).
                 Entrada::Actualizar();
				
                 m_pEscena->ActualizarEstado();
                 ActualizarEstado();
           }
          
        
           // Se renderiza en pantalla todos los graficos.
            Render();
            
            // Si se pulsa la tecla ESC se sale del programa.
            // TODO: llamar a los metodos necesarios para liberar los recursos.
            //if(GetAsyncKeyState(VK_ESCAPE)); 
                //break;
            
           

        }
  // Comentar para control externo de mensajes
	//}
    
    return msg.wParam;
    
}

LRESULT CALLBACK VentanaPrincipal::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

        case WM_DESTROY:
        case WM_CLOSE:
        
            PostQuitMessage(0);
            break;
        
        case WM_LBUTTONDOWN:

            Entrada::SetRatonIzquierdoPulsado(true);
		    break;

        case WM_LBUTTONUP:
            
            Entrada::SetRatonIzquierdoPulsado(false);
            break;

        case WM_RBUTTONDOWN:

            Entrada::SetRatonDerechoPulsado(true);
            break;

        case WM_RBUTTONUP:
            
            Entrada::SetRatonDerechoPulsado(false);
            break;
            
        default:
            
            return DefWindowProc(hWnd,message,wParam,lParam);
    }

    return 0;
} 



HRESULT VentanaPrincipal::InicializarD3D( HWND hWnd )
{
    // Crea el objeto principal D3D
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Configura las estructura que se usara para crear el device.
    // En caso de querer usar stencil buffer, el campo AutoDepthStencilFormat deberá
    // ser D3DFMT_D24S8 (24 bits de z-buffer y 8 para el stencil buffer).
    // Si no se quiere usar el stencil buffer, se puede optar por un valor de
    // D3DFMT_D16, aunque esto provocará posibles errores en la visualización por
    // falta de precisión en el cálculo de profundidad.
    // Según el formato elegido, se debe modificar devmode.dmBitsPerPel=X cuando se cree
    // la ventana (X = 32 con D3DFMT_D24S8 y X = 16 con D3DFMT_D16).
    // Para permitir renderizar sin sincronismo con la frecuencia del monitor (e.g. 60 Hz),
    // se debe utilizar el valor D3DPRESENT_INTERVAL_IMMEDIATE. Si se opta por el valor
    // D3DPRESENT_INTERVAL_DEFAULT, el máximo refresco será de 60 fps (evitandose flicks).

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
	
	if(m_bSombras)
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	}
	else
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}

    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_DEFAULT;
    d3dpp.Flags                  = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    
    


    // Crea el device.
	int res =  g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice  ) ;
    if( res == D3DERR_INVALIDCALL)
    {
        MessageBox(NULL,"ERROR: No se puede crear el Device D3D. Invalid Call",    
                                   "Error Device",0);
        return E_FAIL;
    }

	if( res == D3DERR_NOTAVAILABLE)
    {
        MessageBox(NULL,"ERROR: No se puede crear el Device D3D. Modo no compatible",    
                                   "Error Device",0);
        return E_FAIL;
    }

	if( res == D3DERR_OUTOFVIDEOMEMORY)
    {
        MessageBox(NULL,"ERROR: No se puede crear el Device D3D. Sin memoria de video.",    
                                   "Error Device",0);
        return E_FAIL;
    }
    

	

   

    // Se crea la escena  y se inicializa.

    m_pEscena = new Escena();
    m_pEscena->Inicializar(g_pd3dDevice);

    // Inicializa la clase estática Texto para poder escribir por pantalla.

    Texto::SetFuente(g_pd3dDevice, VentanaPrincipal::ANCHO_VENTANA, 
                    VentanaPrincipal::ALTO_VENTANA);

   



    
    return S_OK;
}



VOID VentanaPrincipal::Limpiar()
{
    //FIXME: Según se añadan nuevos elementos se deberan liberar en este metodo.
    
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

VOID VentanaPrincipal::Render()
{
    
	// Descomentar si se usa el stencilbuffer.
	if(m_bSombras)
	{
		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
                         D3DCOLOR_XRGB(200,150,0), 1.0f, 0);
	}
	else
	{

		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB(200,150,0), 1.0f, 0);
	}

    
    // Empieza la escena.
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
       m_pEscena->Render();
        
       ActualizarInformacion();
       
       // Termina la escena.
        g_pd3dDevice->EndScene();
    }
    

    // Presenta el backbuffer en la pantalla.
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );


}





void VentanaPrincipal::RenderizarTexto()
{
    

    ActualizarInformacion();

}

void VentanaPrincipal::ActualizarInformacion()
{
   
    // Calculo  los fps.
    static int fps;
    DWORD dwTiempoActual=timeGetTime();
    DWORD dwTiempoTranscurrido= dwTiempoActual-m_dwTiempoUltimoRender;
    m_iFrames++;
    
    if(dwTiempoTranscurrido>=1000)
    {
        m_dwTiempoUltimoRender=dwTiempoActual;
        fps=m_iFrames;
        m_iFrames=0;

    }
    
    

    

    std::stringstream ss;
    if (m_bFrames)
    {
        ss<<"FPS: "<<fps<<std::endl;
    }
    
    if(m_bPausa)
    {
        ss<<"PAUSA"<<std::endl;
    }

    
  
    // Se elige el color del texto.
    //Texto::SetColor( 0, 255, 0);

    // Mensaje de titulo.
    Texto::DibujarTexto(0, 0, std::string("Generador de Terrenos Virtuales por LordArkam\nPresione F1 para Ayuda"),true);
    // Mensaje de estado.
    Texto::DibujarTexto(0, 30, ss.str(), true);

         
    if(m_bAyuda)
    {
        std::stringstream strAyuda;
        strAyuda<<  "AYUDA:"<<std::endl<<std::endl<<
                    "RatonIzq:      Rotar Camara"<<std::endl<<
                    "RatonDrc:      Zoom Camara"<<std::endl<<
                    "C:             Cambiar Modo Camara"<<std::endl<<
                    "M:             Modo Renderizado"<<std::endl<<
                    "D:             Textura Detalle"<<std::endl<<
                    "F:             Mostrar Frames"<<std::endl;


                   
        Texto::DibujarTexto(10,100,strAyuda.str(),false);
    }
    
}

void VentanaPrincipal::ActualizarEstado()
{
    if(Entrada::m_Movimiento.f1)
    {   
        m_bAyuda= !m_bAyuda;
        
    }
    if(Entrada::m_Movimiento.f)
    {
        m_bFrames = !m_bFrames;
    }

}

void VentanaPrincipal::ProcesarComandos(const robot_in_t *comando)
{
	
	
	// comando->m_orden de parar.
	if(strcmp(comando->m_orden,"parar") == 0)
	{
		Entrada::Resetear();		
	}
	
	// comando->m_orden de girar(antihorario)
	else if(strcmp(comando->m_orden,"girar") == 0)
	{
		Entrada::Resetear();
		Entrada::m_Movimiento.izquierda = true;

	}

	// comando->m_orden de girar_negativo(sentido horario)
	else if(strcmp(comando->m_orden,"girar_negativo") == 0)
	{
		Entrada::Resetear();
		Entrada::m_Movimiento.derecha = true;
	}

	// comando->m_orden de avanzar.
	else if(strcmp(comando->m_orden,"avanzar") == 0)
	{
		Entrada::Resetear();
		Entrada::m_Movimiento.arriba = true;

	}

	// grado nulo.
	if(strcmp(comando->m_parametro,"nulo") == 0)
	{
		
		Entrada::m_Movimiento.rotacion = 0.0f;
	}

	// comando->m_parametro bajo
	else if(strcmp(comando->m_parametro,"baja") == 0)
	{
		Entrada::m_Movimiento.rotacion = 0.005f;
	}

	// comando->m_parametro medio
	else if(strcmp(comando->m_parametro,"media") == 0)
	{
		Entrada::m_Movimiento.rotacion = 0.01f;
	}

	// comando->m_parametro alto
	else if(strcmp(comando->m_parametro,"alta") == 0)
	{
		Entrada::m_Movimiento.rotacion = 0.02f;
	}

	//comando->m_parametro maximo
	else if(strcmp(comando->m_parametro,"maxima") == 0)
	{
		Entrada::m_Movimiento.rotacion = 0.05f;

	}

}

void VentanaPrincipal::ParsearConfiguracion(GHashTable * configuracion)
{
/*	// Se copia la cadena de texto (debido a que es constante, no se puede
	// manipular, así que hacemos una copia para poder trabajar)
	char * texto =(char*) malloc(strlen(configuracion));
	strcpy(texto, configuracion);



  // Se tokeniza la cadena para extraer cada palabra.	  
  char *array[10];
  int loop;

  array[0]=strtok(texto," ");
  if(array[0]==NULL)
  {
    
    return;
  }

  for(loop=1;loop<10;loop++)
   {
    array[loop]=strtok(NULL," ");
    if(array[loop]==NULL)
      break;
   }*/

 
  // Se comprueba que el número de parametros de configuracion es el esperado.	
	if(g_hash_table_size(configuracion) != 7) 
  {
	  MessageBox(NULL,"Error en los parámetros de configuracion. Número incorrecto","Error",0);
	  
	  return;
  }

  const char * argumento;
  // Se ajusta la resolucion de la ventana.
  argumento = static_cast<const char*>(g_hash_table_lookup(configuracion,"ancho_ventana"));
  VentanaPrincipal::ANCHO_VENTANA = atoi(argumento);

  argumento = static_cast<const char*>(g_hash_table_lookup(configuracion,"alto_ventana"));
  VentanaPrincipal::ALTO_VENTANA = atoi(argumento);

  argumento = static_cast<const char*>(g_hash_table_lookup(configuracion,"sombras"));
  // Se comprueba la activacion de sombras dinamicas.
  if(strcmp(argumento,"sombras_activadas") ==0)
  {
		m_bSombras = true;
  }

  if(strcmp(argumento, "sombras_desactivadas") ==0)
  {
		m_bSombras = false;
  }
  
  // Se establece el color del texto
  argumento = static_cast<const char*>(g_hash_table_lookup(configuracion,"color_rojo"));
  Texto::m_Rojo = atoi(argumento);

  argumento = static_cast<const char*>(g_hash_table_lookup(configuracion,"color_verde"));
  Texto::m_Verde = atoi(argumento);

  argumento = static_cast<const char*>(g_hash_table_lookup(configuracion,"color_azul"));
  Texto::m_Azul = atoi(argumento);

  // Se establece el delay del timer que controla cómo de a menudo se realizará un ciclo
  argumento = static_cast<const char*>(g_hash_table_lookup(configuracion,"delay"));
  m_DelayTimer = atoi(argumento);
  

  

}

void VentanaPrincipal::InicializarTimer()
{

	
#define TARGET_RESOLUTION 1         // 1-millisecond target resolution
	
	static bool primera_vez = true;
	static TIMECAPS		tc;
	static UINT     wTimerRes;

	if(primera_vez)
	{
		

		if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
		{
			MessageBox(NULL,"Error al obtener resolulcion del Timer","Error",0);
		}


		wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
		timeBeginPeriod(wTimerRes); 

		primera_vez = false;
	}

	// Se configura el timer y se activa.
	 if( NULL == timeSetEvent(
        m_DelayTimer,                    // delay
        wTimerRes,                     // resolution (global variable)
        OneShotTimer,               // callback function
        (DWORD)this,                  // user data
        TIME_ONESHOT))                // single timer event
	 {
		 MessageBox(NULL,"Error al activar el timer","Error",0);
	 }
	

}


void CALLBACK OneShotTimer(UINT wTimerID, UINT msg, 
    DWORD dwUser, DWORD dw1, DWORD dw2) 
{
	VentanaPrincipal * v = (VentanaPrincipal *)dwUser;
	v->BucleMensajes(0);
	v->InicializarTimer();

}

/* EOF */