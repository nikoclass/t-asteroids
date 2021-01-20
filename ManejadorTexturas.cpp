#include "ManejadorTexturas.h"


ManejadorTexturas* ManejadorTexturas::instancia=0;

ManejadorTexturas* ManejadorTexturas::getInstancia(){
    if(!instancia) instancia=new ManejadorTexturas();
    return instancia;
}

ManejadorTexturas::ManejadorTexturas(){

}

ManejadorTexturas::~ManejadorTexturas()
{
    //dtor
}

void ManejadorTexturas::setRuta(string ruta){
    this->ruta=ruta;
}
string ManejadorTexturas::getRuta(){
    return ruta;
}

Textura* ManejadorTexturas::getTextura(const char *textura,bool forzarTransparencia){
    string s=ruta+textura;
    string forzar;
    if(forzarTransparencia)
        forzar="-ForzarTransparencia";
    map<string,Textura*>::iterator iter=this->texturas.find(s+forzar);
    if(iter != this->texturas.end()) return iter->second;
    //Si ya existia, devuelvo esa
    else {
    	//Sino lo creo e ingreso al map
        Textura *res=new Textura(s.data(),forzarTransparencia);
        this->texturas[s+forzar]=res;
        return res;
    }
}

