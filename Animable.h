#ifndef ANIMABLE_H_
#define ANIMABLE_H_
#include <iostream>
using namespace std;

class MFisica;

class Animable {

private:
	bool dead;
	float tini;
	float trest;
public:
	Animable();
	Animable(float tini);
	virtual ~Animable();
	void tic(float);
	bool isDead();
	void setDead(bool);
	float porcentajeVida();
	virtual void animar(float)=0;
};

#include "MFisica.h"

#endif /*ANIMABLE_H_*/
