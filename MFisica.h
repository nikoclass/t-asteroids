#ifndef MFISICA_H_
#define MFISICA_H_
//includes
#include <vector>
#include <iostream>
using namespace std;
//references

#include "SeccionCritica.h"

#include "Colisionable.h"
#include "Esfera.h"
#include "Animable.h"
class Animable;


class Colisionable;

class Condition;

class MFisica{

private:
	//Singleton
	static MFisica*instance;
	MFisica();
	vector<Colisionable*>colisionables;
	vector<Animable*>animables;
	void ticAnimables(float dt);
	void ticColisionables(float dt);
	bool colisionar;

	SeccionCritica* mutex;

public:
	static MFisica* getInstance();
	virtual ~MFisica();

	void addColisionable(Colisionable*);
	void removeColisionable(Colisionable*);
	vector<Colisionable*> getColisionables(){ return colisionables;}
	vector<Animable*> getAnimables() {return animables; }

	void addAnimable(Animable*);
	void removeAnimable(Animable*);

	bool existeColsionable(Colisionable*col);

	void tic(float dt);

	void setColisionar(bool c);

    vector<Colisionable*> getColisionablesByCondition(Condition*);

};

#include "Condition.h"

#endif /*MFISICA_H_*/
