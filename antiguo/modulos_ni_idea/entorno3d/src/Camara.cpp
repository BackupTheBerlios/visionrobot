#include "Camara.h"
#include "Entrada.h"
#include <sstream>
#include <windows.h>


// Inicialización de las constantes estaticas.
const float Camara::VELOCIDAD_MOVIMIENTO= 10.5f;
const float Camara::VELOCIDAD_ROTACION = 0.002;

Camara::Camara()
{

}

Camara::~Camara()
{

}

Camara::Camara(LPDIRECT3DDEVICE9 d3dDevice, TipoCamara eTipoCamara)
{
    g_pd3dDevice=d3dDevice;

    m_vPosicion.x=0;
    m_vPosicion.y=0;
    m_vPosicion.z=0;

    
    m_eTipoCamara=eTipoCamara;
    

    InicializarTransformaciones();

    // Creamos el frustum de la camara.
    m_pFrustum = new Frustum();
    m_pFrustum->CrearFrustum();
    

}



void Camara::Mover()
{   

    // Cambia entre los diferentes tipos de camara si se ha pulsado la tecla adecuada.
    if(Entrada::m_Movimiento.c)
    {
        switch (m_eTipoCamara)
        { 
        case CAMARA_PERSECUCION:m_eTipoCamara = CAMARA_ORBITAL;
                                break;
        
        case CAMARA_ORBITAL:    m_eTipoCamara = CAMARA_FIJA;                                
                                break;
        
        case CAMARA_FIJA:       m_eTipoCamara = CAMARA_LIBRE;
                                break;

        case CAMARA_LIBRE:      m_eTipoCamara = CAMARA_PERSECUCION;
                                break;  
        }
       
    }

    switch (m_eTipoCamara)
    {
    case CAMARA_LIBRE:      MoverLibre();
                            break;

    case CAMARA_ORBITAL:    MoverOrbital();
                            break;

    case CAMARA_PERSECUCION:MoverPersecucion();
                            break;
    
    case CAMARA_FIJA:       MoverFija();
                            break;

    
    default:                MoverLibre();
    }

    // Se actualiza el frustum de la camara una vez esta se ha actualizado.
    // Para ello se extrae la matriz combinada vista/proyeccion.
    D3DXMATRIX  matVista, matProyeccion, matCombinada;

    g_pd3dDevice->GetTransform(D3DTS_VIEW, &matVista);
    g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProyeccion);
    
    matCombinada = matVista * matProyeccion;  

    
    m_pFrustum->ExtraerPlanos( matCombinada);    
    
    
}

void Camara::MoverLibre()
{

    // Se pone a cero la velocidad actual
    float fVelStrafe = 0.0f;    
    float fVelocidad = 0.0f;
    float fVelocidadVertical = 0.0f;

    // Se rellena la variable de la velocidad con el valor de la constante
    // VELOCIDAD_MOVIENTO según sea la accion que determina la clase estática
    // entrada.
    if(Entrada::m_Movimiento.arriba) 
    {
       fVelocidad= VELOCIDAD_MOVIMIENTO;
       
    }

    if(Entrada::m_Movimiento.abajo)
    {
        fVelocidad= -VELOCIDAD_MOVIMIENTO;
       
    }

    if(Entrada::m_Movimiento.izquierda)
    {
        fVelStrafe= -VELOCIDAD_MOVIMIENTO;
    }

    if(Entrada::m_Movimiento.derecha)
    {
        fVelStrafe= VELOCIDAD_MOVIMIENTO;
    }

    if(Entrada::m_Movimiento.zoomMas)
    {
        fVelocidadVertical = -VELOCIDAD_MOVIMIENTO;
    }

    if(Entrada::m_Movimiento.zoomMenos)
    {
        fVelocidadVertical= VELOCIDAD_MOVIMIENTO;
    }

   // Se actualizan los angulos de giro en el eje X e Y.
   m_fYaw += Entrada::m_Movimiento.giroX * VELOCIDAD_ROTACION;
   m_fPitch += Entrada::m_Movimiento.giroY * VELOCIDAD_ROTACION;

  
   // Bloquea el angulo de cabeceo en +/- 90 grados. OPCIONAL.
   if( m_fPitch > (D3DX_PI/2)- 0.01f) m_fPitch = (D3DX_PI/2) - 0.01f;
   if( m_fPitch < -(D3DX_PI/2) + 0.01f) m_fPitch = -(D3DX_PI/2) + 0.01f;

   
    // Matrices auxiliares para el calculo de la transformacion de la camara.  
    D3DXMATRIX view,matRotX,matRotY,matTotal;

    // Se crea una matriz de rotacion del eje X con un angulo determinado por el 
    // valor del pitch. Idem con el yaw.
    D3DXMatrixRotationX(&matRotX,m_fPitch);
    D3DXMatrixRotationY(&matRotY,m_fYaw);
    
   
    D3DXVECTOR3 vRight=D3DXVECTOR3 (1.0f, 0.0f, 0.0f);
    D3DXVec3TransformCoord(&vRight,&vRight,&matRotY);
    D3DXVec3Normalize(&vRight,&vRight);


    // Se concatenan las dos matrices para obtener la matriz de rotacion conjunta.
    matTotal=matRotX*matRotY;

    // Se crea un vector auxiliar cuya direccion inicial es mirando hacia la pantalla.
    // Se rota el vector según la matriz de rotacion calculada anteriormente y 
    // se normaliza para obtener un vector de longitud 1.
    // De esta forma se obtiene la dirección hacia la que enfoca la camara.
    D3DXVECTOR3 vLook =D3DXVECTOR3(0,0,1);
    D3DXVec3TransformCoord(&vLook,&vLook,&matTotal);
    D3DXVec3Normalize(&vLook,&vLook);

    
    D3DXVec3TransformCoord(&m_vUp,&m_vUp,&matTotal);
    D3DXVec3Normalize(&vLook,&vLook);


    
    // Estas lineas hacen perpendicular el vector m_vUp para mantener ortonormal los
    // ejes de la camara. 
     D3DXVec3Cross(&m_vUp, &vLook, &vRight);
     D3DXVec3Normalize(&m_vUp, &m_vUp);
    
    

    // Se actualiza la posicion de la camara desplazandola según la velocidad del 
    // movimiento y en la dirección calculada previamente. Si se incluye el vector Up
    // en el calculo, el movimiento vertical de la camara será respecto a su orientacion
    // y no perpendicular al suelo. Para conseguir eso, eliminar el ultimo sumando
    // y descomentar la linea que actualiza el componente y de la posicion.
    m_vPosicion += (vLook * fVelocidad) + (vRight * fVelStrafe);// + (m_vUp * fVelocidadVertical);
    m_vPosicion.y +=  fVelocidadVertical;
    
    
    // Se calcula el vector que representa la posicion del objeto pivote. Para ello
    // partimos de la posicion de la camara y se suma con el vector de orientacion
    // que esta normalizado. Es decir, el pivote se encuentra a una unidad de distancia
    // de la camara en la dirección que hemos calculado previamente.
    vLook+=m_vPosicion;


   
    // Se construye la matriz con los vectores calculados previamente junto
    // al vector Up que no varia (mantiene el valor inicial apuntando a la parte
    // superior del monitor). El vector look representa la posicion del objetivo,
    // y no es por tanto simplemente el vector de orientacion. 
     D3DXMatrixLookAtLH (&view,&m_vPosicion, &vLook, &m_vUp);   
  
     // Se establece la transformacion para la camara.
     g_pd3dDevice->SetTransform(D3DTS_VIEW, &view);


}



void Camara::MoverOrbital()
{

    static float fVelocidad=0.0f;
    static float fVelStrafe = 0.0f;
    
    
  
     if(Entrada::m_Movimiento.arriba) 
    {
       fVelocidad+= VELOCIDAD_MOVIMIENTO;
       
    }
    if(Entrada::m_Movimiento.abajo)
    {
        fVelocidad+= -VELOCIDAD_MOVIMIENTO;
        
        
    }

    if(Entrada::m_Movimiento.izquierda)
    {
        fVelStrafe+= -VELOCIDAD_MOVIMIENTO;
    }

    if(Entrada::m_Movimiento.derecha)
    {
        fVelStrafe+= VELOCIDAD_MOVIMIENTO;
    }
   

    // La ultima fila de la matriz representa el pivote del giro de camara
    D3DXMATRIX matView = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f,
                                      0.0f, 1.0f, 0.0f, 0.0f,
                                      0.0f, 0.0f, 1.0f, 0.0f,
                                      (*m_vPivote).x, (*m_vPivote).y,(*m_vPivote).z, 1.0f);

   // Se actualizan los angulos de giro en el eje X e Y.
    m_fYaw+=Entrada::m_Movimiento.giroX * VELOCIDAD_ROTACION;
    m_fPitch+=Entrada::m_Movimiento.giroY * VELOCIDAD_ROTACION;

    
    // Se actualiza el zoom de la camara (en realidad no es un zoom optico, si no que
    // es la distancia de la camara al pivote).
    m_fZoom+=Entrada::m_Movimiento.zoom;
   
   
    
    
    // Update position and view matrices
    D3DXMATRIX matR, matTemp;
    D3DXQUATERNION   qR;
    D3DXQuaternionRotationYawPitchRoll (&qR, m_fYaw, m_fPitch, m_fRoll);
    D3DXMatrixRotationQuaternion (&matR, &qR);
    D3DXMatrixMultiply (&matView, &matR, &matView);
    D3DXMatrixTranslation (&matTemp, 0.0f, 0.0f, m_fZoom);
    D3DXMatrixMultiply (&matView, &matTemp, &matView);
    D3DXMatrixInverse (&matTemp, NULL, &matView);
   
    // Calculo de la posicion de la camara. Se parte de un vector  orientado
    // a la posicion inicial de la camara. Luego se rota según la matriz creada
    // previamente por los quaternions para actualizar la orientacion actual.
    // Por ultimo, se calcula la posicion final partiendo de la posicion del 
    // pivote y trasladandose en la direccion recien calculada la distancia
    // entre camara/pivote almacenada en m_fZoom.    
   D3DXVECTOR3 vPosicion=D3DXVECTOR3(0, 0, 1);
   D3DXVec3TransformCoord(&vPosicion, &vPosicion, &matR);
   D3DXVec3Normalize(&vPosicion, &vPosicion);
   vPosicion = *m_vPivote + (vPosicion * m_fZoom);
    

    // Actualiza el vector que muestra la informacion de la posicion
    m_vPosicion = vPosicion;

    this->g_pd3dDevice->SetTransform(D3DTS_VIEW, &matTemp );



}



void Camara::MoverPersecucion()
{

    // Se actualiza el zoom de la camara (en realidad no es un zoom optico, si no que
    // es la distancia de la camara al pivote).
    m_fZoom+=Entrada::m_Movimiento.zoom;

    D3DXMATRIX matView;
    D3DXVECTOR3 vPivoteDesplazado, vUp, vDireccion, vPosicion;
           
    // Se establece el punto de pivote desplazado a partir de la posición del objeto pivote.
    vPivoteDesplazado= *m_vPivote; 
    vPivoteDesplazado.y+=50.0;
    
    vDireccion = vPivoteDesplazado - m_pObjetoPivote->GetDireccion();
    vPosicion = vPivoteDesplazado - (m_pObjetoPivote->GetDireccion() * m_fZoom);

    vUp=m_pObjetoPivote->GetUp();
        
    // Se usa un vector Up que apunta hacia arriba siempre de forma que la cámara
    // no rota cuando el objeto hace roll.
    // Si se quiere que la cámara siga rígidamente al objeto, se debe usar el vector
    // Up del objeto pivote. En este caso será necesario modificar el cáculo del 
    // pivote desplazado para que su traslación sea en el espacio del objeto. Para
    // ello se debe crear un vector de desplazamiento que debe ser transformado
    // por la matriz de transformación del objeto.
    D3DXMatrixLookAtLH ( &matView, &vPosicion,&vDireccion,&D3DXVECTOR3(0.0,1.0,0.0));

    // Actualiza el vector que muestra la informacion de la posicion
    m_vPosicion=vPosicion;
    
    this->g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView );

    


}

void Camara::MoverFija()
{
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH (&matView,&m_vPosicion, m_vPivote, &D3DXVECTOR3(0.0, 1.0, 0.0));   
    this->g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

}

std::string Camara::GetInformacion()
{
    
    // Se introduce la información sobre la posicion de la camara.
    std::stringstream ss;    
    ss<<"Posicion:"<<"["<<m_vPosicion.x<<"]["<<m_vPosicion.y<<"]["<<m_vPosicion.z<<"]"<<std::endl;
    
    // Se introduce la informacion sobre el tipo de camara.
    ss<<"Camara: ";
    switch (m_eTipoCamara)
    {
    case CAMARA_LIBRE:      ss<<"MODO LIBRE";
                            break;

    case CAMARA_ORBITAL:    ss<<"MODO ORBITAL";
                            break;

    case CAMARA_PERSECUCION:ss<<"MODO PERSECUCION";
                            break;

    case CAMARA_FIJA       :ss<<"MODO FIJO";
                            break;
        
    }
    ss<<std::endl;

    // Se devuelve el string con toda la informacion previa.
    return ss.str();
    
}

void Camara::InicializarTransformaciones()
{
   
    // Se inicializa la camara orientada hacia el interior de la pantalla y
    // posicionado en el origen de coordenadas.
    m_vLook= D3DXVECTOR3 (0.0f, 0.0f, 1.0f);
    m_vRight= D3DXVECTOR3 (1.0f, 0.0f, 0.0f);
    m_vUp= D3DXVECTOR3 (0.0f, 1.0f, 0.0f);
    m_vPos= D3DXVECTOR3 (0.0f, 0.0f, 0.0f);
    
    m_fPitch = 0.0f;
    m_fYaw = 0.0f;
    m_fRoll = 0.0f;
    m_fZoom = -200.0f;
    m_fVelocidad=0.0f;
    
    

    // Por defecto se toma el pivote como la posicion de la camara.
    m_vPivote=&m_vPos;
    

}

void Camara::RegenerarBase()
{   
    D3DXVec3Normalize(&m_vLook, &m_vLook);
    D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook); 
    D3DXVec3Normalize(&m_vRight, &m_vRight);
    D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight); 
    D3DXVec3Normalize(&m_vUp, &m_vUp);


}

void Camara::SetPivote(Objeto * pObjetoPivote,D3DXVECTOR3 * vPosicion,float * fYaw, float * fPitch, float * fRoll)
{
    m_vPivote=vPosicion;
    m_fPivoteRotacionYaw=fYaw;
    m_fPivoteRotacionPitch=fPitch;
    m_fPivoteRotacionRoll=fRoll;
    m_pObjetoPivote = pObjetoPivote;
}

