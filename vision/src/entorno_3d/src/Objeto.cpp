#include "Objeto.h"
#include "VentanaPrincipal.h"


Objeto::Objeto()
{

}

Objeto::~Objeto()
{
    if( g_pMeshMaterials != NULL ) 
        delete[] g_pMeshMaterials;

    if( g_pMeshTextures )
    {
        for( DWORD i = 0; i < g_dwNumMaterials; i++ )
        {
            if( g_pMeshTextures[i] )
                g_pMeshTextures[i]->Release();
        }
        delete[] g_pMeshTextures;
    }
    if( g_pMesh != NULL )
        g_pMesh->Release();

}


Objeto::Objeto(LPDIRECT3DDEVICE9 g_pd3dDevice, std::string archivo, bool bProyectaSombra)
{

    m_pd3dDevice=g_pd3dDevice;

    m_pMesh=MeshManager::NuevoMesh(archivo,g_pd3dDevice);

    m_bProyectaSombra = bProyectaSombra;
    if(m_bProyectaSombra) m_pSombra = new Sombra();

    m_bRotacionAbsoluta = false;

    InicializarTransformaciones();

    // Se establece el renderState por defecto a ADDSIGNED. Ver documentacion DX para posibles
    // valores.
    m_dwRenderState = D3DTOP_ADDSIGNED;
}

void Objeto::DibujarObjeto()
{
        m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObjeto);
    
        // Se activa el uso de filtros para las texturas.
        VentanaPrincipal::g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        VentanaPrincipal::g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        VentanaPrincipal::g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
        
        // El objeto se divide en las partes que tienen diferentes materiales y 
        // se van dibujando todos dentro del bucle.        
        for( DWORD i=0; i<m_pMesh->GetNumeroMateriales(); i++ )
        {


    // FIXME: Se crea un material a mano para hacer pruebas.

    D3DMATERIAL9 g_silverMetalMtrl;
     ZeroMemory( &g_silverMetalMtrl, sizeof(D3DMATERIAL9) );

    // Set the material's main color to a silver-like gray color.
    g_silverMetalMtrl.Diffuse.r = 1.0f;
    g_silverMetalMtrl.Diffuse.g = 1.0f;
    g_silverMetalMtrl.Diffuse.b = 1.0f;
    g_silverMetalMtrl.Diffuse.a = 1.0f;

    /* g_silverMetalMtrl.Emissive.r =0.2f;
    g_silverMetalMtrl.Emissive.g = 0.2f;
    g_silverMetalMtrl.Emissive.b = 0.2f;
    g_silverMetalMtrl.Emissive.a = 1.0f;*/

    g_silverMetalMtrl.Ambient.r = 0.2f;
    g_silverMetalMtrl.Ambient.g = 0.2f;
    g_silverMetalMtrl.Ambient.b = 0.2f;
    g_silverMetalMtrl.Ambient.a = 1.0f;
   
    

    // Polished silver can reflect back pure white highlights, so set the 
    // specular to pure white.
    g_silverMetalMtrl.Specular.r = 1.0;
    g_silverMetalMtrl.Specular.g = 1.0;
    g_silverMetalMtrl.Specular.b = 1.0;
    g_silverMetalMtrl.Specular.a = 1.0;

    // Set the Power value to a small number to make the highlight's size bigger.
    g_silverMetalMtrl.Power = 50.0f;

    // Establece que la informacion del color se extraiga del material actual y no
    // de la información contenida en los vertices.
    VentanaPrincipal::g_pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
    
    
            
    VentanaPrincipal::g_pd3dDevice->SetMaterial( & g_silverMetalMtrl );
          //VentanaPrincipal::g_pd3dDevice->SetMaterial( &m_pMesh->GetMateriales()[i] );
            //VentanaPrincipal::g_pd3dDevice->SetTexture( 0, g_pMeshTextures[i] );
            VentanaPrincipal::g_pd3dDevice->SetTexture( 0, m_pMesh->GetTextura()[i] );


            

            // FIXME: Este codigo se supone que activa la iluminacion + texturas.
            VentanaPrincipal::g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
            VentanaPrincipal::g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            VentanaPrincipal::g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, m_dwRenderState);
        

            

            m_pMesh->GetMesh()->DrawSubset( i );
            
            
        }



        // Si proyecta sombra, se calcula y se dibuja.
        if( this->m_bProyectaSombra)
        {
            this->DibujarSombra();
        }

}

void Objeto::CicloFisica()
{
        
    if(m_bRotacionAbsoluta)
    {
        ActualizarMatrizAbsoluta();
    }

    else
    {
        ActualizarMatrizRelativa();
    }

      
}



void Objeto::ActualizarMatrizRelativa()
{
            
        // Update rotation matrix
        D3DXQUATERNION qR;
        D3DXMATRIX matRot;
        D3DXQuaternionRotationYawPitchRoll (&qR, m_fYaw, m_fPitch, m_fRoll);
        D3DXMatrixRotationQuaternion (&matRot, &qR);
        D3DXMatrixMultiply (&m_matObjeto, &matRot, &m_matObjeto);

        
        m_vLook.x = m_matObjeto._31;    
        m_vLook.y = m_matObjeto._32;
        m_vLook.z = m_matObjeto._33;
        
        m_vUp.x = m_matObjeto._21;
        m_vUp.y = m_matObjeto._22;
        m_vUp.z = m_matObjeto._23;

        m_vPos.x += m_fVelocidad * m_vLook.x;
        m_vPos.y += m_fVelocidad * m_vLook.y;
        m_vPos.z += m_fVelocidad * m_vLook.z;

        m_matObjeto._41 = m_vPos.x ;
        m_matObjeto._42 = m_vPos.y ;
        m_matObjeto._43 = m_vPos.z ;
        
        
       
        m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObjeto);

        m_fVelocidad=0.0f;
}

void Objeto::ActualizarMatrizAbsoluta()
{
       m_vLook = D3DXVECTOR3 (0.0f, 0.0f, 1.0f);
        m_vRight = D3DXVECTOR3 (1.0f, 0.0f, 0.0f);
        m_vUp = D3DXVECTOR3 (0.0f, 1.0f, .0f);
        
        // Matrices para pitch, yaw y roll.
        D3DXMATRIX matPitch, matYaw, matRoll;
        
        D3DXMatrixRotationAxis(&matYaw, &m_vUp, m_fYaw );
        

        // Rotar los vectores look y right alrededor de up.
        D3DXVec3TransformCoord(&m_vLook, &m_vLook, &matYaw);
        D3DXVec3TransformCoord(&m_vRight, &m_vRight, &matYaw);

        D3DXMatrixRotationAxis(&matPitch, &m_vRight, m_fPitch );
        // Rotar los vectores look y up alrededor de right.
        D3DXVec3TransformCoord(&m_vLook, &m_vLook, &matPitch);
        D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matPitch);

        D3DXMatrixRotationAxis(&matRoll, &m_vLook, m_fRoll);

        // Rotar los vectores right y up alrededor de look.
        D3DXVec3TransformCoord(&m_vRight, &m_vRight, &matRoll);
        D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRoll);

        m_vPos.x+=m_fVelocidad*m_vLook.x;
        m_vPos.y+=m_fVelocidad*m_vLook.y;
        m_vPos.z+=m_fVelocidad*m_vLook.z;

       
        m_matObjeto._11 = m_vRight.x;
        m_matObjeto._12 = m_vRight.y;
        m_matObjeto._13 = m_vRight.z;
        m_matObjeto._21 = m_vUp.x;
        m_matObjeto._22 = m_vUp.y;
        m_matObjeto._23 = m_vUp.z;
        m_matObjeto._31 = m_vLook.x;
        m_matObjeto._32 = m_vLook.y;
        m_matObjeto._33 = m_vLook.z;
        m_matObjeto._41 = m_vPos.x;
        m_matObjeto._42 = m_vPos.y;
        m_matObjeto._43 = m_vPos.z;
        
        // Introduce la escala del objeto.
        m_matObjeto = m_matEscala * m_matObjeto;

        m_fVelocidad=0.0f;
}
void Objeto::ActualizarVectorObjeto()
{
    m_vRight.x = m_matObjeto._11;
    m_vRight.y = m_matObjeto._12;
    m_vRight.z = m_matObjeto._13;
    m_vUp.x = m_matObjeto._21;
    m_vUp.y = m_matObjeto._22;
    m_vUp.z = m_matObjeto._23;
    m_vLook.x = m_matObjeto._31;
    m_vLook.y = m_matObjeto._32;
    m_vLook.z = m_matObjeto._33;
    m_vPos.x = m_matObjeto._41;
    m_vPos.y = m_matObjeto._42;
    m_vPos.z = m_matObjeto._43;


    
}

void Objeto::InicializarTransformaciones()
{
   
    // Se inicializa el objeto orientado hacia el interior de la pantalla y
    // posicionado en el origen de coordenadas.
    m_vLook= D3DXVECTOR3 (0.0f, 0.0f, 1.0f);
    m_vRight= D3DXVECTOR3 (1.0f, 0.0f, 0.0f);
    m_vUp= D3DXVECTOR3 (0.0f, 1.0f, 0.0f);
    m_vPos= D3DXVECTOR3 (0.0f, 0.0f, 0.0f);
    

    m_fPitch=0.0f;
    m_fYaw=0.0f;
    m_fRoll=0.0f;
    m_fVelocidad=0.0f;
    

    //FIXME: PONER BONITO ESTA PARTE
    m_fYawAcumulado = 0.0f;
    m_fPitchAcumulado = 0.0f;
    m_fRollAcumulado = 0.0f;

    D3DXMatrixIdentity(&m_matObjeto);
    D3DXMatrixIdentity(&m_matEscala);

}

void Objeto::RegenerarBase()
{   
    D3DXVec3Normalize(&m_vLook, &m_vLook);
    D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook); 
    D3DXVec3Normalize(&m_vRight, &m_vRight);
    D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight); 
    D3DXVec3Normalize(&m_vUp, &m_vUp);


}

void Objeto::Mover(float fX,float fY, float fZ)
{
        m_vPos.x = fX;
        m_vPos.y = fY;;
        m_vPos.z = fZ;
}
    
void Objeto::Rotar(float fPitch, float fYaw, float fRoll )
{
   
    // Si la rotacion es absoluta, los valores de los valores de los ángulos son acumulativos,
    // mientras que en caso de rotación relativa, los ángulos son directamente el parámetro
    // introducido. 
    if(m_bRotacionAbsoluta)
    {
        m_fPitch += fPitch;
        m_fRoll += fRoll;
        m_fYaw += fYaw;
    
        m_fYawAcumulado+=m_fYaw;
        m_fPitchAcumulado+=m_fPitch;
        m_fRollAcumulado+=m_fRoll;
    }
    
    else
    {
        m_fPitch = fPitch;
        m_fRoll = fRoll;
        m_fYaw = fYaw;
    
        m_fYawAcumulado=m_fYaw;
        m_fPitchAcumulado=m_fPitch;
        m_fRollAcumulado=m_fRoll;

    }

}

void Objeto::Mover(float fVelocidad)
{
    m_fVelocidad=fVelocidad;
}

D3DXVECTOR3 * Objeto::GetPosicion()
{
    return &m_vPos;
}

float * Objeto::GetPitch()
{
    return &m_fPitchAcumulado;
}

float * Objeto::GetYaw()
{
    return &m_fYawAcumulado;
}

float * Objeto::GetRoll()
{
    return &m_fRollAcumulado;
}
    
void Objeto::SetEscala(float fEscala)
{
   
 
    D3DXMatrixScaling(&m_matEscala, fEscala, fEscala, fEscala);
    
 
    
}

void Objeto::DibujarSombra()
{
   
    // Posicion de la luz. 
    // Se parte de la posicion de la luz en el mundo y se calcula la posicion
    // relativa al objeto. Esto se hace porque cuando se genera la silueta,
    // es necesario analizar cada segmento del objeto y el vector de la luz incidente.
    // Como los vertices que forman los segmentos están sin transformar (i.e. son la geometria
    // orginal), es necesario que la posicion de la luz se de respecto al espacio del objeto.
    // Si no se hiciera así, tanto el valor de los vertices como el de la luz no se modificarían
    // al transformar el objeto( si se rota por ejemplo), y por tanto el cálculo es incorrecto.
    // Para pasar la luz al espacio del objeto simplemente es necesario multiplicar por la 
    // matriz inversa del objeto.
    // FIXME: Se debera leer la posicion de la luz desde la escena.Aqui es una 
    // prueba en la que se fija la luz  elevada
    // 100 unidades del suelo.
    //
    D3DXVECTOR3 vLightInWorldSpace( -10000,1500,0 );
    D3DXVECTOR3 vLightInObjectSpace;
    D3DXMATRIXA16 matInverse;
    D3DXMatrixInverse( &matInverse, NULL, &m_matObjeto );
	D3DXVec3TransformNormal( &vLightInObjectSpace, &vLightInWorldSpace, &matInverse );

    
    m_pSombra->Reset();
    m_pSombra->ConstruirSombra(m_pMesh->GetMesh(), vLightInObjectSpace);
    
  
    RenderShadowToStencilBuffer();
    RenderShadowToScene();
    
}


void Objeto::RenderShadowToStencilBuffer( )
{
    // Disable z-buffer writes (note: z-testing still occurs), and enable the
    // stencil-buffer
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );

    // Dont bother with interpolating color
    m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );

    // Set up stencil compare fuction, reference value, and masks.
    // Stencil test passes if ((ref & mask) cmpfn (stencil & mask)) is true.
    // Note: since we set up the stencil-test to always pass, the STENCILFAIL
    // renderstate is really not needed.
    m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_ALWAYS );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILFAIL,  D3DSTENCILOP_KEEP );

    // If z-test passes, inc/decrement stencil buffer value
    m_pd3dDevice->SetRenderState( D3DRS_STENCILREF,       0x1 );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILMASK,      0xffffffff );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0xffffffff );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS,      D3DSTENCILOP_INCR );

    // Make sure that no pixels get drawn to the frame buffer
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	
        // Draw front-side of shadow volume in stencil/z only
        
        m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObjeto );
        m_pSombra->Render( m_pd3dDevice );

        

        // Decrement stencil buffer value
        m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_DECR );

        // Now reverse cull order so back sides of shadow volume are written.
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CW );
        m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObjeto);
        m_pSombra->Render( m_pd3dDevice );
    //}

    // Restore render states
    m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,     TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

void Objeto::RenderShadowToScene( )
{
    // Set render states
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

    // Only write where stencil value >= 1 (count indicates # of shadows that
    // overlap that pixel)
    m_pd3dDevice->SetRenderState( D3DRS_STENCILREF,  0x1 );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );

    // Draw a big, gray square
    m_pd3dDevice->SetFVF( ShadowVertex::FVF_Flags );
    m_pd3dDevice->SetStreamSource( 0, Escena::m_pGranCuadradoVB, 0, sizeof(ShadowVertex) );
    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

    // Restore render states
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

/* EOF */