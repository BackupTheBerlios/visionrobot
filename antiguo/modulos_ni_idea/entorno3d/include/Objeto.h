/** ----------------------------------------------------------------------------------
//
//  @author Jorge Mendoza 2004.
//
//  @brief  Representa los objetos 3d que componen una escena. Esta clase se encarga de
//          mostrarlos por pantalla, actualizar su posicion, rotacion,etc, generar su
//          sombra, etc.
//
*/

#pragma once

#include <d3dx9.h>
#include <string>
#include "Mesh.h"
#include "MeshManager.h"
#include "Sombra.h"

    /**
    FVF para la sombra. Incluye la posicion del vertice y el color difuso.
    */
    struct ShadowVertex
    {
        D3DXVECTOR4 p;
        D3DCOLOR    color;

	    enum FVF
	    {
		    FVF_Flags = D3DFVF_XYZRHW | D3DFVF_DIFFUSE
	    };
    };

class Objeto  
{
public:
	Objeto();

    /**
    Libera los recursos del objeto: materiales,texturas, mesh,etc
    */
	virtual ~Objeto();
    
    /**
    Crea un objeto a partir del nombre de un archivo con un mesh en formato .x .
    
    @param archivo Nombre del archivo que contiene el mesh.
    */
    Objeto(LPDIRECT3DDEVICE9 g_pd3dDevice, std::string archivo, bool bProyectaSombras);

    /**
    Dibuja el objeto en pantalla. Este metodo debe ser llamado entre las sentencias
    BeginScene y EndScene del metodo principal Render();
    */
    void DibujarObjeto();

     /**
    Mueve el objeto en el mundo (i.e. el movimiento es relativo a las coordenadas
    del objeto).

    @param x Incremento de la coordenada x de la posicion del objeto
    @param y Incremento de la coordenada y de la posicion del objeto
    @param z Incremento de la coordenada z de la posicion del objeto
    */
    void Mover(float fX,float fY, float fZ);


    /**
    Establece la velocidad a la que se desplaza el objeto en la orientacion actual.

    @param fVelocidad Velocidad de deplazamiento;
    */
    void Mover(float fVelocidad);

    /**
    Rota el objeto en el mundo(i.e. la rotacion se produce en relación
    a su propio sistema de coordenadas).

    @param fPitch Incremento en el angulo Pitch.
    @param fRoll Incremento en el angulo Roll.
    @param fYaw Incremento en el angulo Yaw.
    */
    void Rotar(float fPitch, float fYaw, float fRoll );
    

    /**
    Devuelve la posicion actual del objeto

    @return Vector con la posicion del obejto
    */
     D3DXVECTOR3* GetPosicion();

     /**
     Devuelve el angulo de rotacion Pitch.

     @return Angulo Pitch.
     */
     float * GetPitch();

     /**
     Devuelve el angulo de rotacion Yaw.

     @return Angulo Yaw.
     */
     float * GetYaw();

     /**
     Devuelve el angulo de rotacion Roll.

     @return Angulo Roll.
     */
     float * GetRoll();

     /**
     Establece la escala del objeto.
     */
     void SetEscala(float fEscala);

     /**
     Actualiza las propiedades fisicas del objeto como es posicion,rotacion,etc
     */
     void CicloFisica();

     /**
     Activa o desactiva la proyeccion de sombras del objeto.
         
     @param bSombra Cierto indica que el objeto proyectara sombras, falso en caso contrario.
     */
     inline void SetSombra( bool bSombra) { m_bProyectaSombra = bSombra;}
    
     /**
     Devuelve el vector con la dirección hacia la que está orientada el objeto.

     @return Vector direccion del objeto.
     */
     inline D3DXVECTOR3 GetDireccion(){ return m_vLook;}

      /**
     Devuelve el vector con la dirección Up del objeto

     @return Vector Up del objeto.
     */
     inline D3DXVECTOR3 GetUp(){ return m_vUp;}

     /**
     Devuelte la matriz de transformacion del objeto. De esta forma
     se pueden situar objetos en posicion relativa a este usando esta matriz.
     Por ejemplo, para situar un punto elevado sobre el objeto como pivote de la camara.

     @return Matriz de transformacion del objeto.
     */
     inline D3DXMATRIX GetMatrizTransformacion() { return m_matObjeto;}

     /**
     Establece el modo de rotación del objeto.

     @param bRotacionAbsoluta Si es verdadero, indica que el objeto rotará de forma absoluta. Para ilustrar
     este comportamiento, supongamos que el objeto es un avion volando y realiza
     una rotación longitudinal (i.e. roll). Con independencia de esta rotación, si el avión
     realiza una rotación transversal (i.e. yaw), ésta seguira la trayectoria paralela al
     horizonte.
     En caso de que sea falso, el objeto rotara de forma relativa. En este caso, si 
     utilizamos el ejemplo del caso anterior, la rotación transversal(i.e. yaw) será
     paralela a la situación actual de las alas del avión y no al horizonte.
     */
     inline void SetModoRotacion(bool bRotacionAbsoluta){m_bRotacionAbsoluta = bRotacionAbsoluta;};
     

     /**
     Establece la operación para calcular el color difuso del objeto. Se supone que
     los operandos serán el color base de los vértices y el color de la textura.

    @param renderState Operacion que se aplicará. Ver documentacion DX para valores posibles.
     */
     inline void SetRenderState(DWORD renderState){ m_dwRenderState = renderState;};

private:

    
    /** 
    Matiz de escalado del objeto.
    */
    D3DXMATRIX m_matEscala;
    /**
    Device sobre el que se dibuja el objeto.
    */
    LPDIRECT3DDEVICE9 m_pd3dDevice;

    /**
    Buffer para almacenar los materiales del objeto.
    */
    LPD3DXBUFFER pD3DXMtrlBuffer;
    
    /**
    Puntero al mesh almacenado en memoria.
    */
    LPD3DXMESH              g_pMesh          ; 
    
    /**
    Buffer que contiene los materiales del objeto.
    */
    D3DMATERIAL9*           g_pMeshMaterials ; 
    
    /**
    Buffer que contiene las texturas del objeto.
    */
    LPDIRECT3DTEXTURE9*     g_pMeshTextures  ; 
    
    /**
    Numero de materiales que contiene el objeto.
    */
    DWORD                   g_dwNumMaterials ;  
    

    /**
    Mesh correspondiente al objeto.
    */
    Mesh* m_pMesh;

    /**
    Vector que indica hacia donde mira el objeto
    */
    D3DXVECTOR3 m_vLook;

    /**
    Vector que indica la derecha del objeto
    */
    D3DXVECTOR3 m_vRight;

    /**
    Vector la dirección hacia arriba del objeto.
    */
    D3DXVECTOR3 m_vUp;

    /**
    Vector que indica la posicion del objeto.
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
    Angulo yaw acumulado(i.e. contiene los grados de rotacion total).
    */
    float m_fYawAcumulado;

    /**
    Angulo yaw acumulado(i.e. contiene los grados de rotacion total).
    */
    float m_fPitchAcumulado;

    /**
    Angulo yaw acumulado(i.e. contiene los grados de rotacion total).
    */
    float m_fRollAcumulado;

    /**
    Velocidad de desplazamiento en la direccion actual.
    */
    float m_fVelocidad;

    
    /**
    Matriz que representa la transformacion del objeto en el espacio del mundo.
    */
    D3DXMATRIX m_matObjeto;

    /**
    Si es verdadero indica el objeto proyecta sombras y se realizara el calculo
    durante el render.
    */
    bool m_bProyectaSombra;

    /**
    Objeto de la clase Sombra que implementa la sombra volumetrica del objeto
    */
    Sombra * m_pSombra;

    /**
    Si es verdadero, indica que el objeto rotará de forma absoluta. Para ilustrar
    este comportamiento, supongamos que el objeto es un avion volando y realiza
    una rotación longitudinal (i.e. roll). Con independencia de esta rotación, si el avión
    realiza una rotación transversal (i.e. yaw), ésta seguira la trayectoria paralela al
    horizonte.

    En caso de que sea falso, el objeto rotara de forma relativa. En este caso, si 
    utilizamos el ejemplo del caso anterior, la rotación transversal(i.e. yaw) será
    paralela a la situación actual de las alas del avión y no al horizonte.
    */
    bool m_bRotacionAbsoluta;

    
    /**
    Establece la operación para el color difuso. Los operandos será el color base de los
    vértices del objeto y el color de la textura.
    */
    DWORD m_dwRenderState;
    
   

private:

    /**
    Rellena la matriz que representa las transformaciones del objeto en el mundo
    a partir de los vectores Look,Up,Right y Pos.
    */     
    void ActualizarMatrizAbsoluta();

    void ActualizarMatrizRelativa();

    /**
    Rellena la matriz que representa las transformaciones del objeto en el mundo
    a partir de los vectores Look,Up,Right y Pos. Se hace de forma relativa al 
    objeto y no al mundo.
    */     
   // void ActualizarMatrizObjetoRelativo();

    /**
    Extrae los vectores Look,Right,Up y Pos de la matriz que representa las 
    transformaciones del objeto en el espacio del mundo.
    */
    void ActualizarVectorObjeto();

    /**
    Inicializa los valores de los vectores, angulos y matriz objeto.
    */
    void InicializarTransformaciones();

    /**
    Regera la base del sistema coordenadas del objeto, asegurando que los vectores sean
    perpendiculares entre ellos. Para ellos se normalizan los vectores y se hacen los 
    prodcutos vectoriales entre ellos.
    */
    void RegenerarBase();

    /**
    Dibuja la sombra del objeto. Es necesario que en la creacion del objeto
    se haya puesto el valor de bProyectaSombra a true para indicar
    que este objeto calculará la proyeccion de su sombra en la escena.
    */
    void DibujarSombra();


    void RenderShadowToStencilBuffer(  );
    
    void RenderShadowToScene();





};

/* EOF */