
#include "Terreno.h"
#include <fstream>
#include "Entrada.h"
#include <string>




Terreno::Terreno()
{

}

Terreno::~Terreno()
{

}


void Terreno::CrearTerreno(  std::string & strNombreTerreno, LPDIRECT3DDEVICE9 pd3dDevice)
{
    
    std::ifstream archivoTerreno(strNombreTerreno.c_str());
    std::string strMapaAltura;
    std::string strTextura;
    std::string strTexturaDetalle;
    int iSuavizado;
    archivoTerreno>>strMapaAltura>>strTextura>>strTexturaDetalle>>m_iTam>>m_iEscala>>iSuavizado;
    
    m_pd3dDevice = pd3dDevice;
    m_fSuavizado = (float)iSuavizado / 100;

    FILE * fMapaAltura = fopen(strMapaAltura.c_str(), "rb");
    m_pAlturas = new BYTE[m_iTam * m_iTam];
    
    if(fMapaAltura)
    {
        fread(m_pAlturas, m_iTam * m_iTam, 1, fMapaAltura);    

        fclose(fMapaAltura);
    }

  

   m_pd3dDevice->CreateVertexBuffer( m_iTam*m_iTam*sizeof(VERTICE),
                                      D3DUSAGE_WRITEONLY, VERTICE::FVF_Flags,
                                      D3DPOOL_MANAGED, &m_pVB, NULL );

    // Genero la malla inicial tomando la altura de los datos del mapa de altura.
     VERTICE* pVerticesAux = new VERTICE [m_iTam*m_iTam] ;
    
     // Desplazamiento corrige las coordenadas para que el terreno quede centrado en el
     // origen (se considera solo el plano xz). 

     // FIXME:La altura se corrige para que la altura máxima corresponda a un valor de 0 de la
     // coordenada y. Esto se hace para que la camara se cree sobre el terreno y se
     // pueda visualizar incialmente sin desplazarse.
     int desplazamiento = m_iTam * 0.5 * m_iEscala;
     for (long i=0; i< m_iTam; i++)
     {
        for(long j=0; j<m_iTam; j++)
        {
            long indice = i* m_iTam + j;
            
            float fX = j * m_iEscala - desplazamiento;
            float fY = m_pAlturas[indice] * m_fSuavizado - 10;//-(255 * m_fSuavizado);
            float fZ = i * m_iEscala - desplazamiento;
            
            pVerticesAux[indice].p = D3DXVECTOR3(fX, fY, fZ);
                                                    
                                                    

            // Se asigna al vertice un color(escala de grises) en proporcion a la altura, siendo 
            // totalmente blanco en el punto más alto y totalmente negro en el mas bajo.
           /* pVerticesAux[indice].color = D3DXCOLOR (    (float)pAlturas[indice] / 255,
                                                        (float)pAlturas[indice] / 255,
                                                        (float)pAlturas[indice] / 255,
                                                        1.0);*/
           
            // Asigna a todos los vertices el mismo color:para aparentar recibir luz solar.
            pVerticesAux[indice].color = D3DXCOLOR (1.0,0.7,0.3,1.0);
            
            // Genera las coordenadas para la textura principal.
            pVerticesAux[indice].tu = (float) j / m_iTam;
            pVerticesAux[indice].tv = (float) i / m_iTam;
            
            // Se generan las coordendas de la textura secundaria. Se
            // asignan valores que exceden 1.0 ya que se aplica wraping
            // para repetir la textura en todas las direcciones.
            // FIXME: en caso de usar diferentes texturas de detalle
            // este sistema no sería válido.
             pVerticesAux[indice].tu2 = (float) j / (m_iTam / 10 - 1);
             pVerticesAux[indice].tv2 = (float)  i / (m_iTam / 10 -1);
          
             //Calculo de la normal para iluminacion.
           // pVerticesAux[indice].n = D3DXVECTOR3 ( 0,1,0);
        }
     }
    
      VERTICE * pVertice;
      m_pVB->Lock( 0, 0, (void**)&pVertice, 0 ) ;   
        
      memcpy(pVertice, pVerticesAux, m_iTam*m_iTam*sizeof(VERTICE));
     
      m_pVB->Unlock();

    
      
    // El tamaño del index buffer es: 3 (vertices por triangulo) * 2 (triangulos por celda) *
    // m_iTam-1 ( numero de celdas en un fila) * m_iTam-1 (numero de celdas en una columna)
    m_lTamIndices = 3 * 2 * (m_iTam-1) * (m_iTam-1);
    m_pd3dDevice->CreateIndexBuffer( m_lTamIndices * sizeof(DWORD), 0, D3DFMT_INDEX32,
                                                       D3DPOOL_MANAGED, &m_pIB, NULL  ) ;
         
    
    // Se asignan los indices para cada triangulo. Se van recorriendo cada fila
    // de la malla, generando dos triangulos para cada celda(cuadrado).
    // El orden de asignación debe ser tal que los triangulos se generen siempre
    // segun las agujas del reloj. 
    // 
    // 
    // m_iTam ---------- m_iTam+1
    //        |\       |
    //        |  \     |
    //        |    \   |
    //        |     \  |
    //        |       \|
    //      0 ---------- 1
    //  
    // Esta sería la primera celda. El orden sería:
    // Triangulo1={0, m_iTam, 1}
    // Tirangulo2={m_iTam, m_iTam+1, 1}
    // 
    // Cada fila tiene m_iTam vertices, por tanto el vertice por encima del i-esimo es
    // m_iTam + i;
    //
    // En cada vuelta del bucle interno se han asignado 6 indices ( 3 vertices * 2 triangulos),
    // de forma que el contador debe ser multiplo de 6 en cada iteración.


    DWORD * wIndices =  new DWORD[m_lTamIndices ];
           
     for(i=0; i< m_iTam-1; i++)
     {
         for(long j=0; j< m_iTam-1; j++)
         {
            wIndices[i * 6 * (m_iTam - 1) + j * 6 ]    = i * m_iTam + j; 
            wIndices[i * 6 * (m_iTam - 1) + j * 6 + 1] = (i+1) * m_iTam  + j; 
            wIndices[i * 6 * (m_iTam - 1) + j * 6 + 2] = i * m_iTam + j + 1;

            wIndices[i * 6 * (m_iTam - 1) + j * 6 + 3] = (i+1) * m_iTam + j; 
            wIndices[i * 6 * (m_iTam - 1) + j * 6 + 4] = (i+1) * m_iTam + j + 1; 
            wIndices[i * 6 * (m_iTam - 1) + j * 6 + 5] = i * m_iTam + j + 1;
            
         }
     }
    
    
     DWORD * pIndices;
     m_pIB->Lock( 0,0, (void**)&pIndices, 0 )  ;   
        
     
      
     memcpy( pIndices, wIndices, sizeof(DWORD)*m_lTamIndices  );
    
        

     m_pIB->Unlock();

    // Se carga la textura de detalle.
    D3DXCreateTextureFromFile( m_pd3dDevice, strTexturaDetalle.c_str(), &m_pTexturaDetalle);
     
    // Si se ha especificado que se va a generar la textura del terreno, se crea la
    // nueva textura, se guarda en un archivo y se carga desde él para generar
    // los mipmaps.
    if( strTextura == std::string("generar"))
    { 
    
         GenerarTextura(archivoTerreno);
         D3DXCreateTextureFromFile( m_pd3dDevice, "Textura.jpg", &m_pTexturaTerreno);
    }
    // Si no, se carga la textura cuyo nombre se ha especificado.
    else
    {
         D3DXCreateTextureFromFile( m_pd3dDevice, strTextura.c_str(), &m_pTexturaTerreno);
    }

    
   

    


    // Se incializa el modo de textura detalle para mostrar las dos texturas.
    m_bTexturaDetalle =true;

    delete[] wIndices;
       
    archivoTerreno.close();

   
    
}

void Terreno::Render()
{
    // Se comprueba si el usuario ha cambiado modo de render de la textura secundaria.
    

    D3DXMATRIX matIdentity;
       
    D3DXMatrixIdentity(&matIdentity);

     m_pd3dDevice->SetTransform( D3DTS_WORLD, &matIdentity);

     
    
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
   
    
  
  
  

    // Textura base
    m_pd3dDevice->SetTexture( 0, m_pTexturaTerreno);
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
   
    
    if (m_bTexturaDetalle)
    {
        // Textura de detalle
        m_pd3dDevice->SetTexture(1, m_pTexturaDetalle);
        m_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1 );
        m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT );
        m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);

        m_pd3dDevice->SetSamplerState( 1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        m_pd3dDevice->SetSamplerState( 1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    }
    else
    {
           m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

    }
    

    m_pd3dDevice->SetFVF( VERTICE::FVF_Flags );
    m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(VERTICE) );
   

	m_pd3dDevice->SetIndices( m_pIB );
	
	
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
                                     0,
                                     0,
                                     m_iTam * m_iTam,
                                     0,
                                     2*(m_iTam - 1)*(m_iTam - 1)); 

    m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
}

float Terreno::GetAltura(float fX, float fZ)
{
    float fDesplazamiento = m_iTam * 0.5 * m_iEscala;
    
    int i = static_cast<int>((fZ + fDesplazamiento) / m_iEscala);
    int j = static_cast<int>((fX + fDesplazamiento) / m_iEscala) ;
    
    // Compruebo que la coordenada buscada esté dentro de los limites del terreno.
    // Si no es así devolvemos un valor no significativo de 0.0.
    if ( (i >= m_iTam) || (i < 0) || (j >= m_iTam) || (j < 0) ) return 0.0f;
    
    else
    {
        
        float fAltura = m_pAlturas[i * m_iTam + j] * m_fSuavizado - (255 * m_fSuavizado);
    
        return fAltura;
    }
    
}

float Terreno::GetAlturaInterpolada(float fX, float fZ)
{
    // Para interpolar la altura, calculamos el valor de la altura del vertice
    // más cercano a la coordenada y del siguiente (para eso sumamos m_iEscala
    // que nos asegura que el valor escalado será mayor o igual al correspondiente 
    // al siguiente indice). Este calculo se realiza por separado para el eje X y 
    // el eje Z. La altura final será la media entre las dos alturas calculadas
    // en cada uno de los ejes.
    float fAlturaXMin, fAlturaXMax, fAlturaZMin, fAlturaZMax;

    fAlturaXMin = GetAltura(fX, fZ);
    fAlturaXMax = GetAltura(fX + m_iEscala, fZ);

    fAlturaZMin = GetAltura (fX, fZ);
    fAlturaZMax = GetAltura (fX, fZ + m_iEscala);
    

    
    float fXEntero = static_cast<int>(fX / m_iEscala);
    float fDeltaX = fX - fXEntero * m_iEscala;
    float fFactorX = fDeltaX / m_iEscala;
    float fAlturaX = fAlturaXMin + ((fAlturaXMax - fAlturaXMin) * fFactorX);
    
    float fZEntero = static_cast<int>(fZ / m_iEscala);
    float fDeltaZ = fZ - fZEntero * m_iEscala;
    float fFactorZ = fDeltaZ / m_iEscala ;
    float fAlturaZ = fAlturaZMin + ((fAlturaZMax - fAlturaZMin) * fFactorZ);

    return (fAlturaX + fAlturaX) * 0.5f;
}

void Terreno::GenerarTextura(std::ifstream &fArchivo)
{

    // Se crea una textura vacia que se rellenará interpolando los pixels de diferentes texturas
    // según la altura del punto del terreno al que corresponda dicho pixel.

    // Se lee del archivo el numero de tiles que se usarán.
    int iNumTiles;     
    fArchivo>>iNumTiles;

    // Se lee el tamaño de las textruas. Todas las texturas de los tiles deben tener
    // el mismo tamaño. La textura resultante a su vez tambien será de las mismas dimensiones.
    int iTam;
    fArchivo>>iTam;

    // Se reserva espacio en memoria para los tiles.
    TILE * pTiles = new TILE [iNumTiles];
     
    // Para cada tile se extrae la textura y la región correspondiente.
    // La region determinará para cada tile como se interpola con el resto
    // según el valor de la altura correspondiente a cada pixel.
    std::string strNombreTextura;
    for(int i = 0; i < iNumTiles; i++)
    {
        fArchivo>>strNombreTextura;
        fArchivo>>pTiles[i].m_tRegiones.m_iAlturaInferior;
        fArchivo>>pTiles[i].m_tRegiones.m_iAlturaOptima;
        fArchivo>>pTiles[i].m_tRegiones.m_iAlturaSuperior;

        D3DXCreateTextureFromFile( m_pd3dDevice, strNombreTextura.c_str(), &pTiles[i].m_pTextura);
    
    }
 
    
    
    // Se bloquea la region de datos de las texturas de cada tile para poder leer su contenido.
    for(int j = 0; j < iNumTiles; j++)
    {
        pTiles[j].m_pTextura->LockRect(0, &pTiles[j].m_LockedRect, 0, 0);
        pTiles[j].m_pDatos = (DWORD *) pTiles[j].m_LockedRect.pBits;    
    }

    
    // Se genera la textura final que inicialmente estará vacia. Tendrá la misma
    // dimensiones que los tiles.
    LPDIRECT3DTEXTURE9 pTexturaFinal;
    D3DXCreateTexture(  m_pd3dDevice,
                        iTam, 
                        iTam,
                        0,
                        0,
                        D3DFMT_X8R8G8B8,
                        D3DPOOL_MANAGED,
                        &pTexturaFinal);
                        
    D3DLOCKED_RECT  lockedRectFinal;
    pTexturaFinal->LockRect(0, &lockedRectFinal, 0, 0);
    DWORD * datosFinal = (DWORD *) lockedRectFinal.pBits;
    
    // Se calcula cada pixel de la textura final.
    for( int k = 0; k < iTam; k++)
    {
        for(int l = 0; l < iTam; l++)
        {
           
           BYTE colorRojo = 0;
           BYTE colorVerde = 0;
           BYTE colorAzul = 0;

           for (int m = 0; m < iNumTiles; m++)
           {

                float fPeso = CalcularPesoPixel(m_pAlturas[k * iTam+l],pTiles[m].m_tRegiones);
               
                BYTE colorRojoAux = (pTiles[m].m_pDatos[k * (lockedRectFinal.Pitch / 4 ) + l] & 0x00ff0000) >>16;
                BYTE colorVerdeAux = (pTiles[m].m_pDatos[k * (lockedRectFinal.Pitch / 4 ) + l] & 0x0000ff00) >> 8;
                BYTE colorAzulAux = pTiles[m].m_pDatos[k * (lockedRectFinal.Pitch / 4 ) + l] & 0x000000ff;

                // Se usa la formula de LERP para que al interpolar no resulten
                // colores más claros que los originales.
                colorRojo += fPeso * (colorRojoAux - colorRojo);
                colorVerde += fPeso * (colorVerdeAux - colorVerde); 
                colorAzul += fPeso * (colorAzulAux - colorAzul);
                
              
           }
           
           // Se calcula la iluminacion de cada pixel.
           // TODO: Añadir un filtro de suavizado para que las sombras no sean
           // demasiado nitidas.(filtro gaussiano?)
            float fLuz = CalcularIluminacion(l, k, 1, 0, 0.5f, 0.9f, 0.1);

            float fLuzRojo =  fLuz * 1.0f;
            float fLuzVerde = fLuz * 0.9f;
            float fLuzAzul =  fLuz * 0.7f;

            colorRojo =  colorRojo * fLuzRojo;
            colorVerde = colorVerde * fLuzVerde;
            colorAzul =  colorAzul * fLuzAzul;

          /*  colorRojo =  255 * fLuzRojo;
            colorVerde = 255 * fLuzVerde;
            colorAzul =  255 * fLuzAzul;*/

            // Se asigna el valor resultante de los calculos anteriores.
            datosFinal [ k * (lockedRectFinal.Pitch / 4 ) + l] = 
            0xff000000 | (colorRojo << 16 | (colorVerde << 8 | (colorAzul))); 

    
        }
    }

    // Se liberan los recursos.
    for( j = 0; j < iNumTiles; j++)
    {
        pTiles[j].m_pTextura->UnlockRect(0);
        
        pTiles[j].m_pTextura->Release();
        
    }
    delete [] pTiles;

    pTexturaFinal->UnlockRect(0);
  
    // Se salva la textura creada en un archivo de imagen.
    D3DXSaveTextureToFile("Textura.jpg",D3DXIFF_JPG,pTexturaFinal,0);
    
    
    
}

float Terreno::CalcularPesoPixel(int iAltura, const REGIONES_TILE & tRegion)
{
    // Si la altura esta fuera de los limites de la región, este tile no
    // aportará nada para el cálculo del color del pixel.
    if(iAltura < tRegion.m_iAlturaInferior || iAltura > tRegion.m_iAlturaSuperior) 
        return 0.0f;

    else
    {
        // La altura está por debajo de la óptima.
        if(iAltura < tRegion.m_iAlturaOptima)
        {
            float fTemp1 = iAltura - tRegion.m_iAlturaInferior;
            float fTemp2 = tRegion.m_iAlturaOptima - tRegion.m_iAlturaInferior;

            return fTemp1 / fTemp2;
        }

        // La altura está por encima de la óptima.
        else if (iAltura > tRegion.m_iAlturaOptima)
        {
            float fTemp1 = iAltura - tRegion.m_iAlturaOptima;
            float fTemp2 = tRegion.m_iAlturaSuperior - tRegion.m_iAlturaOptima;

            return (1.0f - (fTemp1 / fTemp2));
        }

        // La altura coincide con la altura optima de la region y por tanto
        // este pixel tendrá el mismo color que el del tile al que pertenece la region.
        else return 1.0f;
    }

}


float Terreno::CalcularIluminacion(     int iX,
                                        int iZ,
                                        int iDirLuzX,
                                        int iDirLuzZ,
                                        float fLimiteMinimo,
                                        float fLimiteMaximo,
                                        float fSuavidadLuz)
{
    float fLuz;
    
    if(iX > iDirLuzX && iZ > iDirLuzZ)
    {

        fLuz = 1.0f - (m_pAlturas[(iZ - iDirLuzZ) * m_iTam + (iX - iDirLuzX)] 
                    - m_pAlturas[iZ * m_iTam + iX]) / fSuavidadLuz; 
    }

    else fLuz = 1.0f;

    if(fLuz < fLimiteMinimo) fLuz = fLimiteMinimo;
    else if(fLuz > fLimiteMaximo) fLuz = fLimiteMaximo;

    return fLuz;


    
}

void Terreno::ActualizarEstado()
{
    if (Entrada::m_Movimiento.d)
    {
        m_bTexturaDetalle = !m_bTexturaDetalle;
    }
}
