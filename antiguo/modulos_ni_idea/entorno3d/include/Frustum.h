/** ------------------------------------------------------------------------------------
//
//
//  @author Jorge Mendoza 2004.
//
//  @brief  Esta clase constityue el frustum de la camara. Estará formado por varios planos
//          que contienen el espacio que es visible a la camara. A partir del frustum
//          se podrán hacer operaciones de culling para evitar renderizar los objetos
//          que no son visibles en ese momento.
//
*/


#pragma once
#include <d3dx9.h>
#include <string>



/**
Determina la posicion de un punto respecto a un plano.
*/
enum PuntoVsPlano
{
    NEGATIVO = -1,
    EN_PLANO = 0,
    POSITIVO = 1
};

enum TipoPlano
{
    IZQUIERDO = 0,
    DERECHO,
    SUPERIOR,
    INFERIOR,
    CERCANO,
    LEJANO
};

class Frustum  
{
public:

    Frustum();
	virtual ~Frustum();


public:
    
    /**
    Crea el frustum incializando los planos que lo componen
    */
    void CrearFrustum();

    /**
    Extrae los planos del frustum a parti de la matriz conjunta de vista y proyeccion.

    @param matCombinada Matriz combinada de vista y proyeccion.
    */
    void ExtraerPlanos(const D3DXMATRIX &matCombinada);

    /**
    Determina la posicion de un punto del espacio respecto a un plano del frustum.

    @param vPunto Punto cuya posición se examinará.
    @param TipoPlano Determina que plano del frustum se comparará.
    @return Componente de PuntoVsPlano que describe la posicion del punto respecto al plano.
    */
    PuntoVsPlano CompararPunto(const D3DXVECTOR3 &vPunto, TipoPlano ePlano);

    

private:

    /**
    Planos que forman el frustum.
    */
    D3DXPLANE * m_pPlanos;  
   

};
