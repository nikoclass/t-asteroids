#ifndef MISIL_H_
#define MISIL_H_

#include "Colisionable.h"
class Colisionable;
#include "Vector.h"
#include "Luz.h"
#include "Asteroide.h"
class Asteroide;
#include "Nave.h"
class Nave;
#include "ModoJuego.h"
#include "Settings.h"
#include "Humo.h"

#include "Matematicas.h"
#include "ServidorRed.h"
#include "Municion.h"
#include "Targeteable.h"

class Misil : public Colisionable, public ServidorRed, public Municion {
private:

	static const float mult_aceleracion;
	Vector velocidad;
	Vector aceleracion;
	float danio;
	float ttl;
	float inicio;
	Luz*luz;
	bool arranco;
	Targeteable*target;
	Objeto3d*fuego;

	float retryHumo;


public:
	Misil(Objeto3d* modelo,Vector posicion,Vector direccion);
	virtual ~Misil();

	float getDanio();
	void setDanio(float danio);
	void setTarget(Targeteable*);
	Targeteable*getTarget();

	void tic(float dt);
	void bang(Colisionable*);

	void recibirPaqueteServidor(PaqueteRed *paquete);
	string getNombreServidor();

	void setTtl(float t);

};

#endif /*MISIL_H_*/
