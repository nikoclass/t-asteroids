#ifndef NAVE_H_
#define NAVE_H_
class Nave;


#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

#include "Objeto3d.h"
#include "Settings.h"
#include "Engine.h"
#include "Misil.h"
#include "Colisionable.h"
#include "Asteroide.h"
#include "BalaComun.h"

#include "CadenaParticulas.h"

#include "FMunicion.h"
#include "Targeteable.h"
//class FMunicion;
class FBalaComun;
class FMisil;
class FElectricidad;



class Nave : public Colisionable, public Hasheable, public Targeteable{
protected:
	Vector velocidad;

	bool
        movio,
        moviolat,
        adelante,
        lateralD,
        vivo,
        colision;

	FBalaComun* primaria;
	FMunicion* secundaria;

	Objeto3d
        * fuego,
        * fuegolat,
        * espectro;

	Luz
        * luzFuegolat,
        * luzFuego;

	Sprite *brilloFuego;

    float
        maxVel,
        factor,
        incrementoVel,
        danio,
        counter,
        counterEmb,
        vida,
        masa,
        pondDanio;

    CadenaParticulas *cadenaParticulas;

    virtual void ticVivo(float dt)=0;

    int numCliente;

    bool quieroDispararPrimara;
    bool quieroDispararSecundaria;



public:
	//static Nave*getInstance();
	Nave(Objeto3d*obj,Vector posicion);
	virtual ~Nave();
	Vector getVelocidad();
	void setVelocidad( Vector v);
	void moverAdelante(float);
	void moverAtraz(float);
	void moverDer(float);
	void moverIzq(float);
	void idle(float dtr);

	void rotar(float angA,float angB,float dt);
	void girarDerecha(float dt);
	void girarIzquierda(float dt);
	void dispararPrimaria(){if(vivo) quieroDispararPrimara=true;}
	void dispararSecundaria(){if(vivo) quieroDispararSecundaria=true;} ///<<<-----------habilitar---------------
	virtual void disparar()=0;
	void teletransportar();
	void tic(float dt);

    virtual void bang(Colisionable*);
    virtual void explotar()=0;

    Objeto3d* getFuego();
    Sprite* getBrillo();
	void targeting();
	bool isConTarget();
	float getDanio();
	void setDanio(float);
	void setEnvairomental();
	void setEmbistiendo();
	float getVida();
	void setVida(float);
	int getMisiles();
	float getNormaVelocidad();
	void recargarSecundaria();
	void recargarVida();
	void balasRojas();
    bool isViva();
    bool setViva(bool val){vivo=val;}
    bool getMovio();
    void setMovio( bool val);
    float getpondDanio();
    void setpondDanio( float val);
    float getMasa(){return this->masa;}
    void setMasa(float val){this->masa = val;}
    float getCounterEmb(){return this->counterEmb;}
    void setCounterEmb(float val){this->counterEmb = val;}
    void setNumCliente(int num){numCliente=num;}
    int getNumCliente(){return numCliente;}
    Vector getPosicionTarget(){return getPosicion();}
    void setPosicionTarget(Vector v){posicionar(v);}
    void setVidaTarget(float v){
        vida=v;
        if (this->vida<=0 && vivo){
            vivo=false;
            explotar();
            //new BolaExplosiva(this->getPosicion());
        }
    }
    float getVidaTarget(){return getVida();}

    Objeto3d* getEspectro(){return espectro;}
    FMunicion* getSecundaria(){return secundaria;}

};
#include "FMisil.h"
#include "FElectricidad.h"
#include "FBalaComun.h"
#endif /*NAVE_H_*/
