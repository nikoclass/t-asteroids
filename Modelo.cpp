#include "Modelo.h"



Modelo::Modelo(){
    this->nombre="";
}

Modelo::Modelo(const char *modelo,bool optimizarVertices){
    cargarModelo(modelo,optimizarVertices);
}

void Modelo::destruirModelo(){
    int cant=vectorMallas.size();

    for(int i=0;i<cant;i++){
        delete vectorMallas[i];
    }

    vectorMallas.clear();
    mapMallas.clear();

    esferaEnvolvente=Esfera();
}


void Modelo::cargarModelo(const char* modelo,bool optimizarVertices){

    FILE *f=fopen(modelo,"r");

    if(!f){
    	string ex="no existe el modelo: ";
    	ex+=modelo;
    	throw logic_error(ex);
    }

    this->nombre=modelo;
    string base=ManejadorModelos::getInstancia()->getRuta();
    this->nombre.erase(0,base.size());

    destruirModelo();

    //Leo cantidad de mallas
    Malla::moverseHasta(f,"Meshes: ");
    int cantMallas;
    fscanf(f,"%d",&cantMallas);

    string nom;
    Malla* malla;
    Vector min(9999,9999,9999),max=-min;

    Esfera e;
    float r;
    Vector dir=Vector(1,1,1).direccion();

    for(int i=0;i<cantMallas;i++){
        malla=new Malla(f,optimizarVertices);

        this->vectorMallas.push_back(malla);
        nom=malla->getNombre();
        this->mapMallas[nom]=malla;

        e=malla->getEsferaEnvolvente();
        r=e.getRadio();
        min=Min(min,e.getCentro()-r*dir);
        max=Max(max,e.getCentro()+r*dir);
    }
    Vector centro=(min+max)/2;
    float dist=0;
    map<string,Malla*>::iterator iter;
    for(iter=this->mapMallas.begin();iter!=this->mapMallas.end();iter++){
        e=iter->second->getEsferaEnvolvente();
        dist=Max(dist,centro.distancia(e.getCentro())+e.getRadio());
    }

    this->esferaEnvolvente=Esfera(centro,dist);

    fclose(f);

}

string Modelo::getNombre(){
    return nombre;
}

int Modelo::getCantMallas(){
    return this->vectorMallas.size();
}

Malla* Modelo::getMalla(string nombre){
    map<string,Malla*>::iterator iter=this->mapMallas.find(nombre);
    if(iter!=mapMallas.end())
        return iter->second;
    else
        return NULL;
}

Malla* Modelo::getMalla(int num){
    if(num<(int)vectorMallas.size())
        return vectorMallas[num];
    else
        return NULL;
}

Esfera Modelo::getEsferaEnvolvente(int numMalla){
    Malla *m=getMalla(numMalla);
    if(m)
        return m->getEsferaEnvolvente();
    else
        return Esfera();
}

Esfera Modelo::getEsferaEnvolvente(string nomMalla){
    Malla* m=getMalla(nomMalla);
    if(m)
        return m->getEsferaEnvolvente();
    else
        return Esfera();
}

Esfera Modelo::getEsferaEnvolvente(){
    return this->esferaEnvolvente;
}

void Modelo::toGL(){
    int cant=vectorMallas.size();

    for(int i=0;i<cant;i++){
        vectorMallas[i]->toGL();
    }
}

void Modelo::toGL(int numMalla){
    Malla *m=getMalla(numMalla);
    if(m)
        m->toGL();
}

void Modelo::toGL(string nomMalla){
    Malla* m=getMalla(nomMalla);
    if(m)
        m->toGL();
}


Modelo::~Modelo(){
    destruirModelo();
}
