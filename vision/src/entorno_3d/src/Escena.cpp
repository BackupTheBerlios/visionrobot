
#include "Escena.h"
#include <string>


// Inicializacion de las variables estaticas.
LPDIRECT3DVERTEXBUFFER9  Escena::m_pGranCuadradoVB;
bool Escena::m_bModoRenderSolido = true;
bool Escena::m_bFuego = false;
bool Escena::m_bSombra = true;


Escena::Escena()
{


}

Escena::~Escena()
{

}

void Escena::Inicializar(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // Guardamos el puntero al device de la escena para todas las operaciones
    // posteriores que lo necesitan.
    m_pd3dDevice=pd3dDevice;

    // Activa el zbuffer para hacer el culling,luz y brillo especular.
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );     
    m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE);
    //m_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, FALSE );
    
    // Activa la luz ambiente. 
    pd3dDevice->SetRenderState( D3DRS_AMBIENT,
                                D3DCOLOR_COLORVALUE( 0.2f, 0.2f,0.2f, 1.0f ) );

    // Se establece la matriz de proyeccion. Se usa un ratio de enfoque de pi/4
    // usado habitualmente. También se determina el plano de clipping mas cercano
    // y más lejano para construir el fustrum.
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/2.5, 1.0f, 1.0f, 100000.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    // Crea la camara de la escena.
     m_Camara=new Camara(m_pd3dDevice, CAMARA_PERSECUCION);

     
    
    // FIXME: Crear luces bien.Esto es una prueba.
    D3DLIGHT9         g_pLight0;
    g_pLight0.Type = D3DLIGHT_DIRECTIONAL;
	g_pLight0.Direction = D3DXVECTOR3(10000,-1500,0);

    g_pLight0.Diffuse.r = 1.0f;
    g_pLight0.Diffuse.g = 0.7f;
    g_pLight0.Diffuse.b = 0.3f;
	g_pLight0.Diffuse.a = 1.0f;

	g_pLight0.Specular.r = 1.0f;
	g_pLight0.Specular.g = 0.7f;
	g_pLight0.Specular.b = 0.3f;
	g_pLight0.Specular.a = 1.0f;

    g_pLight0.Ambient.r = 0.5f;
    g_pLight0.Ambient.g = 0.5f;
    g_pLight0.Ambient.b = 0.5f;

    

    

    m_pd3dDevice->SetLight( 0, &g_pLight0 );
    m_pd3dDevice->LightEnable( 0, TRUE );


    /*
    Crea el cuadrado que se usa en la proyeccion de las sombras.
    */
    m_pd3dDevice->CreateVertexBuffer( 4*sizeof(ShadowVertex),
                                      D3DUSAGE_WRITEONLY, ShadowVertex::FVF_Flags,
                                      D3DPOOL_MANAGED, &m_pGranCuadradoVB, NULL );

    // Get the width & height of the back-buffer.
    LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	D3DSURFACE_DESC d3dsd;
    m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &d3dsd );
    pBackBuffer->Release();
	float sx = (float)d3dsd.Width;
	float sy = (float)d3dsd.Height;

	// Set the size of the big square shadow
    ShadowVertex *v;

    m_pGranCuadradoVB->Lock( 0, 0, (void**)&v, 0 );
	{
		v[0].p = D3DXVECTOR4(  0, sy, 0.0f, 1.0f );
		v[1].p = D3DXVECTOR4(  0,  0, 0.0f, 1.0f );
		v[2].p = D3DXVECTOR4( sx, sy, 0.0f, 1.0f );
		v[3].p = D3DXVECTOR4( sx,  0, 0.0f, 1.0f );
		v[0].color = 0x7f000000;
		v[1].color = 0x7f000000;
		v[2].color = 0x7f000000;
		v[3].color = 0x7f000000;
        
	}
    m_pGranCuadradoVB->Unlock();


    
    // Se crea el terreno de la escena a partir de un mapa de altura.
    m_pTerreno = new Terreno();
    m_pTerreno->CrearTerreno(std::string("terrenotemplo.txt"), m_pd3dDevice );

    // Se crea la boveda del cielo.
    m_pCielo = new Cielo();
    m_pCielo->CrearCielo(m_pd3dDevice, std::string("texturas\\nubesmascara.bmp"),30000, 40000, 16);
   
   
    // Se crea el agua.
    //m_pAgua = new Agua();
    //m_pAgua->CrearAgua(m_pd3dDevice, 5000, std::string ("agua.bmp"));

    // Se crea un objeto para dar una vuelta por el terreno.
    m_pAvion = new Objeto(m_pd3dDevice, std::string("modelos\\r2.x"), true);
    m_Camara->SetPivote(m_pAvion,m_pAvion->GetPosicion(), m_pAvion->GetYaw(), m_pAvion->GetPitch(), m_pAvion->GetRoll());

    // Se crea el escenario.
    m_pEscenario = new Objeto(m_pd3dDevice, std::string("modelos\\templo.x"), false);
    // Se cambia la operacion que se usara para calcular el color difuso del objeto.
    m_pEscenario->SetRenderState(D3DTOP_ADD);
    // Se rota el escenario para situarlo correctamente.
    m_pEscenario->Rotar(0,D3DX_PI,0);
    m_pEscenario->CicloFisica();

    // Crea un sistema de particulas .
    //m_pSistemaParticulas = new SistemaParticulas(100, m_pAvion->GetPosicion(),this->m_pd3dDevice);
    //m_pSistemaParticulas->Inicializar();

    m_pLuz = new Luz();
    m_pLuz->CrearLuz(m_pd3dDevice, std::string("luz.txt"),D3DXVECTOR3(-10000,1500,000), m_Camara);

   

}


void Escena::Render()
{
    D3DVIEWPORT9  pViewPort;
    m_pd3dDevice->GetViewport(&pViewPort);
   

    if(m_bModoRenderSolido) m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, 0);
    else m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, 2);
   
  
    
   
    m_pTerreno->Render();

    
    m_pEscenario->DibujarObjeto();

    m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE,false);
    
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE); 

    
    
  //  m_pAgua->Render();

    m_pAvion->DibujarObjeto(); 
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
   
    m_pCielo->Render();

 
    m_pLuz->Render();
    
    if(m_bFuego)
    {
        //m_pSistemaParticulas->Render();
    }
     

}



void Escena::CicloFisica(bool bPausa)
{
   
    if(!bPausa){
        
	

        float fRotacion = Entrada::m_Movimiento.rotacion;
        
       
        float fRotacionX = 0.0f;
        float fRotacionZ = 0.0f;
        float fRotacionY = 0.0f;

   
        if(Entrada::m_Movimiento.abajo){
            // Velocidad del avatar. 
            m_pAvion->Mover( 2);
        }

        if(Entrada::m_Movimiento.arriba)
        {
			m_pAvion->Mover(- 2);
         
        }

        if(Entrada::m_Movimiento.derecha){
            //m_pAvion->Rotar(0.0f, fRotacion,0.0f);
            fRotacionY = fRotacion;
        }

        if(Entrada::m_Movimiento.izquierda){
            //m_pAvion->Rotar(0, -fRotacion,0);
            fRotacionY = -fRotacion;
        }

        if(Entrada::m_Movimiento.zoomMas){
            //m_pAvion->Rotar(0, 0,0.03);
            fRotacionY = -fRotacion;
        }

        if(Entrada::m_Movimiento.zoomMenos){
            //m_pAvion->Rotar(0, 0,-0.03);

            fRotacionY = fRotacion;
        }
        m_pAvion->Rotar(fRotacionX, fRotacionY, fRotacionZ);     
        
        m_pAvion->CicloFisica();
        
       
        //m_pSistemaParticulas->Actualizar();
       
    }

    // Mueve la camara según la entrada y el tipo de camara seleccionada. La camara
    // se debe mover en ultimo lugar para que la sincronizacion con los objetos
    // a los que sigue sea correcta. Si no, la camara irá retrasada respecto a la rotacion
    // del pivote.
    m_Camara->Mover();

    m_pTerreno->ActualizarEstado();
    m_pLuz->Actualizar();
  
  

   
  
     
   
}

void Escena::ActualizarEstado()
{
    
    if(Entrada::m_Movimiento.x)
    {
        m_bFuego = !m_bFuego;

    }

    /*
    if(Entrada::m_Movimiento.s)
    {
        m_bSombra = !m_bSombra;

        m_pAvion->SetSombra(m_bSombra);       
        
    }*/

    if( Entrada::m_Movimiento.m)
    {
        m_bModoRenderSolido = !m_bModoRenderSolido;
    }


}
/* EOF */
