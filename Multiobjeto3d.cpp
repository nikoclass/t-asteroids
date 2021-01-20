#include "Multiobjeto3d.h"



Multiobjeto3d::Multiobjeto3d():Dibujable(){

}

Multiobjeto3d::Multiobjeto3d(const char *mdl,bool optimizarVertices):Dibujable(){
    Modelo* modelo=ManejadorModelos::getInstancia()->getModelo(mdl,optimizarVertices);

    int cant=modelo->getCantMallas();
    Malla *malla;
    Objeto3d *objeto;
    string nombre;

    for(int i=0;i<cant;i++){
        malla=modelo->getMalla(i);
        nombre=malla->getNombre();

        objeto=new Objeto3d(mdl,optimizarVertices);
        objeto->setNombreMalla(nombre);
        this->objetos[nombre]=objeto;
    }

}


Multiobjeto3d::~Multiobjeto3d(){
    map<string,Dibujable*>::iterator iter;
    for(iter=objetos.begin(); iter!=objetos.end();iter++){
        delete iter->second;
    }
    objetos.clear();
}


Dibujable* Multiobjeto3d::getObjeto(string nombre){
    map<string,Dibujable*>::iterator iter=objetos.find(nombre);
    if(iter!=objetos.end())
        return iter->second;
    else
        return NULL;
}
void Multiobjeto3d::addObjeto(string nombre,Dibujable *objeto){
    objetos[nombre]=objeto;
}

void Multiobjeto3d::dibujar(){
    map<string,Dibujable*>::iterator iter;
    for(iter=objetos.begin();iter!=objetos.end();iter++){
        iter->second->toGL();
    }
}

bool Multiobjeto3d::esTransparente(){
    map<string,Dibujable*>::iterator iter;
    for(iter=objetos.begin();iter!=objetos.end();iter++){
        if(iter->second->esTransparente()) //Si hay uno transparente, considero todo transparente
            return true;
    }
    return false;
}

bool Multiobjeto3d::getAfectaLuces(){
    map<string,Dibujable*>::iterator iter;
    for(iter=objetos.begin();iter!=objetos.end();iter++){
        if(iter->second->getAfectaLuces()) //Si alguno le afecta la luz, al multiobjeto le afecta
            return true;
    }
    return false;
}

Esfera Multiobjeto3d::getEsferaEnvolvente(){
    Vector min(9999,9999,9999),max=-min;
    Esfera e;
    float r;
    Vector dir=Vector(1,1,1).direccion();
    map<string,Dibujable*>::iterator iter;

    vector<Esfera> esferas(objetos.size());
    for(iter=objetos.begin();iter!=objetos.end();iter++){
        e=iter->second->getEsferaEnvolvente();
        esferas.push_back(e);
        r=e.getRadio();
        min=Min(min,e.getCentro()-r*dir);
        max=Max(max,e.getCentro()+r*dir);
    }

    Vector centro=(min+max)/2;
    float dist=0;
    int cant=esferas.size();
    for(int i=0;i<cant;i++)
        dist=Max(dist,centro.distancia(esferas[i].getCentro())+esferas[i].getRadio());

    e=Esfera(centro,dist);

    centro=this->transformacion*centro;

    Matriz4x4 m=getRotaciones();

    Vector x(1,0,0);
    Vector y(0,1,0);
    Vector z(0,0,1);

    x=m*x;
    y=m*y;
    z=m*z;

    float radio;
    radio=e.getRadio()*Max(x.norma(),Max(y.norma(),z.norma()));

    return Esfera(centro,radio);

}

