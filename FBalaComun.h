#ifndef FBALACOMUN_H_
#define FBALACOMUN_H_
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include "FMunicion.h"

#include "BalaComun.h"
class BalaComun;

using namespace std;

class FBalaComun : public FMunicion {

private:
	string nameModelo;
	string textura;
	map<BalaComun*,BalaComun*>balas;
	float retryTime;
	float retryConst;
	float balasRojasc;
	int lado;
public:
	FBalaComun(Nave* origen);
	virtual ~FBalaComun();

	int getLado();
	void setLado(int);
	void setRetryConst(float);
	void balasRojas();

	void disparar(Vector posicion,Vector direccion);
	void tic(float dt);
	bool isConTarget();
	void setConTarget(bool);
	float getcantMunicion(){};
	void setcantMunicion(float){};

};

#endif /*FBALACOMUN_H_*/
