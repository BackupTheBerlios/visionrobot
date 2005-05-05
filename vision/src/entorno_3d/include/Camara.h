/** --------------------------------------------------------------------------------
// 
// @author Jorge Mendoza 2004.
//
// @brief   Camara de la aplicacion. Permite realizar las operaciones de movimiento
//          para navegar por la escena.
//
*/
#pragma once

#include <d3dx9.h>
#include <string>
#include "Objeto.h"
#include "Frustum.h"



enum TipoCamara{CAMARA_LIBRE,CAMARA_ORBITAL,CAMARA_PERSECUCION, CAMARA_FIJA};

class Camara  
{
public:
	Camara();
	
    /**
    Constructora de la camara.
    
    @param d3dDevice Device al que pertenece la camara.
    */
    Camara(LPDIRECT3DDEVICE9 d3dDevice, TipoCamara eTipoCamara); 
    
    virtual ~Camara();
    
    
    /**
    Mueve la camara segun la entrada.
    */
    void Mover();

    /**
    Devuelve una cadena conteniendo la informacion sobre la posicion de la camara.
    */
    std::string GetInformacion();

    /**
    Establece la posicion del pivote al que sigue la camara en el modo orbital.

    @param vPosicion Posicion del pivote.
    @param fYaw Grados de rotacion del angulo yaw.
    @param fPitch Grados de rotacion del angulo pitch.
    */
    void SetPivote(Objeto * pObjetoPivote,D3DXVECTOR3 * vPosicion, float * fYaw, float * fPitch, float * fRoll);

    



    
    
private:
    

    /** 
    Constantes que determinan la velocidad del movimiento lineal y de rotacion
    de la camara.
    */
    static const float VELOCIDAD_MOVIMIENTO;
    static const float VELOCIDAD_ROTACION;
    /**
    Device d3d al que pertenece la camara
    */
    LPDIRECT3DDEVICE9       g_pd3dDevice; 

    /**
    Posicion de la camara:
    */
    D3DXVECTOR3 m_vPosicion;

 
     /**
    Vector que indica hacia donde mira el camara
    */
    D3DXVECTOR3 m_vLook;

    /**
    Vector que indica la derecha del camara
    */
    D3DXVECTOR3 m_vRight;

    /**
    Vector la dirección hacia arriba del camara.
    */
    D3DXVECTOR3 m_vUp;

    /**
    Vector que indica la posicion del camara.
    */
    D3DXVECTOR3 m_vPos;

    /**
    Angulo de Pitch.
    */
    float m_fPitch;

    /**
    Angulo de Roll.
    */
    float m_fRoll;

    /**
    Angulo de Yaw.
    */
    float m_fYaw;

    /**
    Zoom de la camara.
    */
    float m_fZoom;

    /**
    Velocidad de desplazamiento en la direccion actual.
    */
    float m_fVelocidad;

    /**
    Determina el modo de camara.
    
    El modo libre permite navegar por el mundo
    como en los editores de escenarios tipo WorldCraft.

    El modo orbital permite enfocar la camara a un punto y rotar sobre el.
    Usa quaternions y es una forma adecuada para crear camaras en tercera persona.
    */
    TipoCamara m_eTipoCamara;

    /**
    Puntero al vector de la posicion del pivote.
    */
    D3DXVECTOR3 * m_vPivote;

    /**
    Puntero al valor de rotacion del pivote para sincronizar la rotacion yaw
    en la camara de tipo persecucion.
    */
    float * m_fPivoteRotacionYaw;

     /**
    Puntero al valor de rotacion del pivote para sincronizar la rotacion pitch
    en la camara de tipo persecucion.
    */
    float * m_fPivoteRotacionPitch;

     /**
    Puntero al valor de rotacion del pivote para sincronizar la rotacion roll
    en la camara de tipo persecucion.
    */
    float * m_fPivoteRotacionRoll;

    /**
    Objeto que sirve de pivote a la camara.
    */
    Objeto * m_pObjetoPivote;

    /**
    Frustum de la camara.
    */
    Frustum * m_pFrustum;
   
     
private:
    /**
    Inicializa los valores de los vectores, angulos y velocidad.
    */
    void InicializarTransformaciones();

    /**
    Regera la base del sistema coordenadas de la vista, asegurando que los vectores sean
    perpendiculares entre ellos. Para ellos se normalizan los vectores y se hacen los 
    prodcutos vectoriales entre ellos.
    */
    void RegenerarBase();

    /**
    Mueve la camara de forma libre permitiendo la navegacion por el mundo como 
    en los editores de mapas tipo WorldCraft. Se usa el raton pulsado para girar y
    las teclas para moverse hacia delante/atrás y hacer strafe lateral.
    */
    void MoverLibre();

    /**
    Mueve la camara de forma orbital alrededor de un punto. Usa quaternions para
    calcular las rotaciones. Util para implementar camaras de tercera persona que 
    siguen a un objeto.
    */
    void MoverOrbital();

    /**
    Mueve la camara siguiendo al pivote.La camara permanece a las espaldas del objeto
    pudiendo variar la distancia de seguimiento.
    */
    void MoverPersecucion();

    /**
    Mantiene la camara en un punto mientras enfoca al objeto pivote.
    */
    void MoverFija();



 
};

