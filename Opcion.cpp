#include "Opcion.h"

Opcion::Opcion(int i,Texto2D* nom,Texto2D* desc){
    this->index = i;
    this->nombre = nom;
    if(nom)nom->setAfectaZbuffer(false);
    this->descripcion = desc;
    if(desc)desc->setAfectaZbuffer(false);
}

Opcion::~Opcion()
{
}

void Opcion::setIndex(int value){
    this->index = value;
}
void Opcion::setNombre(Texto2D* value){
    this->nombre = value;
}
void Opcion::setDescripcion(Texto2D* value){
    this->descripcion = value;
}

void Opcion::setSubOpciones(vector<Opcion*> value){
    this->subopciones = value;
}

void Opcion::setValor(float value){
    this->valor = value;
}

void Opcion::setTipo(TipoRenderizado value){
    this->tipo = value;
}
//getters
int Opcion::getIndex(){
    return this->index;
}
Texto2D* Opcion::getNombre(){
    return this->nombre;
}
Texto2D* Opcion::getDescripcion(){
    return this->descripcion;
}
vector<Opcion*>& Opcion::getSubOpciones(){
    return this->subopciones;
}
float Opcion::getValor(){
    return this->valor;
}
TipoRenderizado Opcion::getTipo(){
    return this->tipo;
}
