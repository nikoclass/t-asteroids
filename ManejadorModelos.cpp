#include "ManejadorModelos.h"

ManejadorModelos* ManejadorModelos::instancia=0;

ManejadorModelos* ManejadorModelos::getInstancia(){
    if(!instancia) instancia=new ManejadorModelos();
    return instancia;
}



Modelo* ManejadorModelos::getModelo(const char* modelo,bool optimizarVertices){
    string s=modelo;
    if(optimizarVertices)
        s+=" -OVerdadero";
    else
        s+=" -OFalso";

    map<string,Modelo*>::iterator iter=this->modelos.find(s);

    if(iter != this->modelos.end()) { //Si ya existia, devuelvo ese
        return iter->second;
    }else{ //Sino lo creo e ingreso al map
        Modelo *res=new Modelo((ruta+modelo).data(),optimizarVertices);
        this->modelos[s]=res;
        return res;
    }
}

void ManejadorModelos::setRuta(string ruta){
    this->ruta=ruta;
}
string ManejadorModelos::getRuta(){
    return ruta;
}

ManejadorModelos::ManejadorModelos(){

}

ManejadorModelos::~ManejadorModelos(){
    //dtor
}
