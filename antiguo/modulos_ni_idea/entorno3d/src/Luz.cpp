

#include "Luz.h"
#include <fstream>



Luz::Luz()
{

}

Luz::~Luz()
{

    if(m_pBillboards)
    {
         for(int i = 0; i < m_iNumBillboards; i++) delete m_pBillboards[i];
         delete [] m_pBillboards;
    }

    if(m_pBillboards2D)
    {
         for(int i = 0; i < m_iNumBillboards2D; i++) delete m_pBillboards2D[i];
         delete [] m_pBillboards2D;
    }
}



void Luz::CrearLuz( LPDIRECT3DDEVICE9 pd3dDevice,
                    std::string &strArchivo, 
                    D3DXVECTOR3 & vPos,
                    Camara * pCamara)
{

    m_pd3dDevice = pd3dDevice;
    m_vPos = vPos;

    // Leemos del archivo de configuración los datos necesarios para crar la luz.
    std::ifstream fArchivo(strArchivo.c_str());
    
    

    // Se reserva el espacio para almacenar los billboards que componene la luz.
    fArchivo>>m_iNumBillboards;    
    m_pBillboards = new  Billboard *[m_iNumBillboards];

    std::string strTextura;
    float fRojo,fVerde,fAzul,fAlpha;
    float fTam;

    // Para cada uno de ellos se leen del archivo los datos para crearlos adecuadamente.
    for(int i = 0; i < m_iNumBillboards; i++)
    {
        fArchivo>>strTextura>>fTam>>fRojo>>fVerde>>fAzul>>fAlpha;
        
        m_pBillboards[i] = new Billboard(   m_pd3dDevice, 
                                            pCamara,
                                            vPos,
                                            strTextura,
                                            fTam,
                                            D3DXCOLOR(fRojo, fVerde, fAzul, fAlpha));
    }

    // Se reserva espacio para los billboards2D que forman los lens flares.
    // Si el valor leido del archivo que indica el numero de lens flares que se
    // van a crear es 0, se entiende que esta luz no utiliza este efecto.

    fArchivo>>m_iNumBillboards2D;

    if(m_iNumBillboards2D == 0) m_bLensFlares =false;
    else
    {
        m_bLensFlares = true;

        m_pBillboards2D = new Billboard2D *[m_iNumBillboards2D];

        float fAncho,fAlto;
        
        for(int i = 0; i < m_iNumBillboards2D; i++)
        {
            fArchivo>>strTextura>>fAncho>>fAlto>>fRojo>>fVerde>>fAzul>>fAlpha;
            
            m_pBillboards2D[i] = new Billboard2D( m_pd3dDevice,
                                                D3DXVECTOR3(200,200,0),
                                                strTextura,
                                                fAncho,
                                                fAlto,
                                                1024,768,                                                
                                                D3DXCOLOR(fRojo, fVerde, fAzul, fAlpha));
                                                
                                                
        }

    }

    

}

void Luz::Render()
{
    for(int i = 0; i < m_iNumBillboards; i++) m_pBillboards[i]->Render();
    
    if(m_bLensFlares && m_bLuzEnPantalla)
    {
        D3DXMATRIX matProjection, matView;
        m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
        m_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
        for(int i = 0; i < m_iNumBillboards2D; i++) m_pBillboards2D[i]->Render();
        m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProjection);
        m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
    }
}

void Luz::Actualizar()
{

    for(int i = 0; i < m_iNumBillboards; i++) m_pBillboards[i]->Actualizar();

    // Se calcula la posicion de los reflejos secundarios según la posicion de la luz
    // respecto al centro de la pantalla.
    // Para ello se calcula las coordenadas en el espacio de la pantalla de la luz.
    // A partir de ahí se situán los reflejos secundarios en la linea que une la luz con
    // el centro de la pantalla.
    if(m_bLensFlares)
    {
        D3DXMATRIX matView, matProj, matTotal;
        m_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
        m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);

        matTotal = matView * matProj;

        D3DXVECTOR3 vPosPantalla;
        D3DXVec3TransformCoord(&vPosPantalla, &m_vPos, &matTotal);

        
        // Se corrige el resultado obtenido para obtener las coordenadas de pantalla reales.
        // Los valores devueltos están comprendidos entre (-1,1). Por tanto hay que 
        // sumarle 1 para obtener el rango (0,2). Luego se multiplica por el ancho o alto
        // de la pantalla dividido entre dos para que la coordenada final este en (0,ancho_pantalla)
        // o (0,alto_pantalla). También hay que cambiar la coordenada Y ya que está invertida
        // respecto a su valor real en la pantalla.
        D3DXVECTOR3 vPosPantallaReal;        
        vPosPantallaReal.x = (1 + vPosPantalla.x ) * (1024 * 0.5);
        vPosPantallaReal.y = (1 - vPosPantalla.y ) * (768 * 0.5);
        
        // Se comprueba que la luz este dentro de la pantalla.
        if( vPosPantalla.x > -1.0 && vPosPantalla.x < 1.0 &&
            vPosPantalla.y > -1.0 && vPosPantalla.y < 1.0 && 
             vPosPantalla.z < 1.0) m_bLuzEnPantalla = true;
        else m_bLuzEnPantalla = false;

        // Se actualizan los lens flares a partir de la posicion calculada anteriormente.
        for(int i = 0; i < m_iNumBillboards2D; i++)
        {
            float fDesplazamiento;
            
            
            fDesplazamiento = 1.0 /( (i+1) * (i+1) );
                       
            m_pBillboards2D[i]->Mover(  (1 - (vPosPantalla.x * fDesplazamiento )) * (1024 * 0.5),
                                        (1 + (vPosPantalla.y * fDesplazamiento)) * (768 * 0.5));
        }

    }
    
}

void Luz::SetPosicion(D3DXVECTOR3 & vPos)
{
    for(int i = 0; i < m_iNumBillboards; i++) m_pBillboards[i]->Mover(vPos);
}

/* EOF */