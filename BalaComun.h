#ifndef BALACOMUN_H_
#define BALACOMUN_H_


#include "Engine.h"
#include "Settings.h"
#include "Colisionable.h"
#include "ExplosionParticulas.h"

#include "ServidorRed.h"
#include "Nave.h"
class Nave;
#include "Asteroide.h"
class Asteroide;
#include "Municion.h"

class BalaComun : public Colisionable, public ServidorRed, public Municion {
private:
	Luz* luz;
	Vector velocidad;
	float ttl;
	bool porMorir;
	float danio;

public:

	BalaComun(Objeto3d* modelo,Vector posicion,Vector direccion);
	virtual ~BalaComun();
	void setRoja();
	Vector getVelocidad();
	void setVelocidad(Vector velocidad);
	float getDanio();
	void setDanio(float);
	//colisionable
	void tic(float dt);
	void bang(Colisionable*);

	void recibirPaqueteServidor(PaqueteRed* paquete);

	string getNombreServidor();

	void setTtl(float t);
	//string getNombreServidor();
};

#endif /*BALACOMUN_H_*/
