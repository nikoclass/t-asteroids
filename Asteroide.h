#ifndef ASTEROIDE_H_
#define ASTEROIDE_H_

#include "Engine.h"
#include "Esfera.h"
#include "Settings.h"
#include "Matematicas.h"
#include "Colisionable.h"
#include "Matriz4x4.h"
#include "ModoJuego.h"
class ModoJuego;
#include "BalaComun.h"
class BalaComun;
#include "Nave.h"
class Nave;
#include "Misil.h"
class Misil;
#include "Poder.h"
class Poder;
#include "BolaExplosiva.h"
#include "Explosion.h"
#include "Espiral.h"
#include "ServidorRed.h"
#include "Targeteable.h"


class Asteroide:public Colisionable, public ServidorRed, public Targeteable {
private:
	int id;
	float nivel;
	float vida;
	Vector velocidad;
	Vector velocidadAngular;
	float masa;
	bool ya_choco;
	float dt;
	float maxVel; // lo seteo en el trasladar

public:
	Asteroide(int id, float nivel,Objeto3d*modelo,Vector posicion);
	virtual ~Asteroide();
	void setPosicionTarget(Vector v){Colisionable::posicionar(v);}
	Vector getPosicionTarget(){return Colisionable::getPosicion();}


	float getNivel(){return nivel;}
	void setModeloAsteroide(Objeto3d*);
	void setVelocidad(Vector vel);
	void setVelocidadAngular(Vector vel);
	Vector getVelocidad();
	Vector getVelocidadAngular();
	float getMasa();

	//colisionable
	void tic(float dt);
	void bang(Colisionable*);
	float getDanio();

	void setVida(float v);
	float getVida();

    void setVidaTarget(float v){setVida(v);}
	float getVidaTarget(){return getVida();}


	void recibirPaqueteServidor(PaqueteRed *paquete);
	string getNombreServidor();

};

#endif /*ASTEROIDE_H_*/
