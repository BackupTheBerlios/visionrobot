

#include "Cielo.h"



Cielo::Cielo()
{

}

Cielo::~Cielo()
{

}


void Cielo::CrearCielo(    LPDIRECT3DDEVICE9 pd3dDevice,
                    std::string & strTextura,
                    float fRadioAtmosfera,
                    float fRadioPlaneta,
                    int iDivisiones)
{

    m_pd3dDevice = pd3dDevice;

    // Se calculan los vertices e indices necesarios para construir las iDivisiones.
    // Una division (una celda) estará compuesta de dos triangulos que se forman
    // a partir de 4 vertices ( ya que el segundo triangulo comparte dos vertices con el anterior).
    m_iNumVertices = (iDivisiones + 1) * (iDivisiones + 1);
    m_iNumIndices =  iDivisiones * iDivisiones * 2 * 3;
    
    float fTamPlano = 2.0 * (float)sqrt( (fRadioPlaneta * fRadioPlaneta)-(fRadioAtmosfera * fRadioAtmosfera));
    float fDeltaPlano = fTamPlano / (float)iDivisiones;
    float fDeltaTextura = 2.0f / (float) iDivisiones;
    

    float fDistX = 0.0f;
    float fDistZ = 0.0f;
    float fAlturaX = 0.0f;
    float fAlturaZ = 0.0f;
    float fAltura = 0.0f;

    VERTICE * pVertices = new VERTICE[m_iNumVertices];

    for (int i=0; i < iDivisiones + 1; i++)
    {
        for(int j=0; j< iDivisiones + 1; j++)
        {
   	        fDistX = (-0.5f * fTamPlano) + ((float)j * fDeltaPlano);
			fDistZ = (-0.5f * fTamPlano) + ((float)i * fDeltaPlano);

		    fAlturaX = (fDistX * fDistX) / fRadioAtmosfera;
			fAlturaZ = (fDistZ * fDistZ) / fRadioAtmosfera;
			fAltura = fAlturaX + fAlturaZ;

			pVertices[i * (iDivisiones + 1 ) + j].p = D3DXVECTOR3 (fDistX, - fAltura, fDistZ);
            pVertices[i * (iDivisiones + 1 ) + j].tu = ((float)j * fDeltaTextura * 0.5f); 
            pVertices[i * (iDivisiones + 1 ) + j].tv = (1.0f - (float)i * fDeltaTextura * 0.5f); 
            pVertices[i * (iDivisiones + 1 ) + j].color = D3DXCOLOR (1.0f, 0.7f, (float)5000.0f / fAltura, 1.0f);

		}
	}

    m_pd3dDevice->CreateVertexBuffer(  m_iNumVertices * sizeof(VERTICE),
                                      D3DUSAGE_WRITEONLY, VERTICE::FVF_Flags,
                                      D3DPOOL_MANAGED, &m_pVB, NULL );
    
    VERTICE * pVerticeAux;
    m_pVB->Lock( 0, 0, (void**)&pVerticeAux, 0 ) ;   
        
    memcpy(pVerticeAux, pVertices, m_iNumVertices * sizeof(VERTICE));
     
    m_pVB->Unlock();

    delete [] pVertices;

    short * pIndices = new short [m_iNumIndices];
    
    int iContador = 0;
    // Calculo de los indices. Se calculan para crear los triangulos
    // en el sentido contra horario para que sean visibles por la parte de abajo
    // (ya que se quiere ver el cielo desde abajo).
	
    for (i=0; i < iDivisiones; i++)
	{
		for (int j=0; j < iDivisiones; j++)
		{
			int iVerticeBase = (i * (iDivisiones + 1) + j);

		    // triangulo 1
			pIndices[iContador++] = iVerticeBase;
			pIndices[iContador++] = iVerticeBase + 1;
			pIndices[iContador++] = iVerticeBase + iDivisiones+1;

			// triangulo 2
			pIndices[iContador++] = iVerticeBase + 1;
			pIndices[iContador++] = iVerticeBase + iDivisiones + 2;
			pIndices[iContador++] = iVerticeBase + iDivisiones + 1;
		}
	}

    // Una vez calculados los vertices y los indices se crean los buffers correspondientes.
  
    m_pd3dDevice->CreateIndexBuffer( m_iNumIndices * sizeof(short), 0, D3DFMT_INDEX16,
                                                       D3DPOOL_MANAGED, &m_pIB, NULL);

    short * pIndicesAux;
    m_pIB->Lock( 0,0, (void**)&pIndicesAux, 0 )  ;   

    memcpy( pIndicesAux, pIndices, m_iNumIndices * sizeof(short));
    
    m_pIB->Unlock();

    
    // Se carga la textura.
    D3DXCreateTextureFromFile( m_pd3dDevice, strTextura.c_str(), &m_pTexturaCielo);

   

}

void Cielo::Render()
{
       
    D3DXMATRIX matTraslacion, matView;
    
    // Se anula la traslación de la matriz vista. De esta forma
    // el cielo siempre estará a la misma distancia dando la apariencia
    // de infinito. Sin embargo si que la rotación de la camara lo afectará
    // de forma que el efecto sea convincente.
    // Es necesario controlar el orden de renderizado del resto de objetos de la escena
    // ya que esta modificacion les afectará también.
    // FIXME: Esta operación debe ser controlada de forma más general desde un contexto
    // que pueda determinar que otros objetos se ven implicados(ciclo Render de la escena).
    /*m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView);
    matView._41 = 0.0f;
    matView._42 = 0.0f;
    matView._43 = 0.0f;
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView);*/
    
    D3DXMatrixTranslation(&matTraslacion,0,10000,0);
    
    
    
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matTraslacion);
    

     
  
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
   
    
  

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_ADDSIGNED);
    

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    
    m_pd3dDevice->SetTexture(0, m_pTexturaCielo);

    m_pd3dDevice->SetFVF( VERTICE::FVF_Flags );
    m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(VERTICE) );
   

	m_pd3dDevice->SetIndices( m_pIB );
	
	
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
                                     0,
                                     0,
                                     m_iNumVertices, 
                                     0,
                                     m_iNumIndices/3); 
}

