#include "FMunicion.h"

FMunicion::FMunicion(Nave* origen){
    this->origen = origen;
}

FMunicion::~FMunicion(){}


float FMunicion::getcantMunicion(){
	return this->cantMunicion;
}

void FMunicion::setcantMunicion(float cant){
	this->cantMunicion=cant;
}
