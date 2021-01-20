#include "Electricidad.h"
#include "Vector.h"
#include "Objeto3d.h"


Electricidad::Electricidad():Animable(){
    this->cantPartes=0;
    this->curvatura=1;
    this->rayo = NULL;
    this->periodo=0;
    this->tiempo=0;
}

Electricidad::Electricidad(float timeout):Animable(timeout){
    this->cantPartes=0;
    this->curvatura=1;
    this->rayo = NULL;
    this->periodo=0;
    this->tiempo=0;
}

Electricidad::~Electricidad(){
    if(rayo)
        delete rayo;
}

void Electricidad::animar(float dt){

    if(cantPartes<=0) return;

    int i;
    char id[20];
    Dibujable *parte;

    tiempo-=dt;
    if(tiempo<=0){

        tiempo=periodo;

        Vector dif,delta=(pos_final-pos_inicial)/cantPartes;
        Vector pos,pos_sig,pos_ant;

        pos=pos_ant=pos_inicial;
        float c;
        float dist=pos_inicial.distancia(pos_final)*0.5;

        for(i=0;i<cantPartes;i++){
            sprintf(id,"parte_%d",i);
            parte=this->rayo->getObjeto(id);

            pos_sig=pos+delta + dist*curvatura*(sin(PI*(i+1)/cantPartes))*Vector(Random(-1,1),Random(-1,1),Random(-1,1)).direccion()*Random();
            dif=pos_sig-pos_ant;

            parte->identidad();
            parte->escalar(Vector(1,dif.norma(),1));
            parte->rotar(Vector(0,1,0),dif);
            parte->posicionar(pos_ant);
            c=Random(0.5,1);
            parte->setColor(c,c,1);

            pos_ant=pos_sig;
            pos+=delta;
        }
    }

    float pVida=sqrt(this->porcentajeVida()/100);

     for(i=0;i<cantPartes;i++){
            sprintf(id,"parte_%d",i);
            parte=this->rayo->getObjeto(id);
            parte->setOpacidad(pVida);
     }

}

void Electricidad::setPosIni(Vector pos){
    this->pos_inicial = pos;
}

void Electricidad::setPosFin(Vector pos){
    this->pos_final = pos;
}

Vector Electricidad::getPosIni(){
    return this->pos_inicial;
}

Vector Electricidad::getPosFin(){
    return this->pos_final;
}

void Electricidad::setCantPartes(int cant){

    this->cantPartes=cant;

    if(rayo)
        delete rayo;

    this->rayo=new Multiobjeto3d();
    this->rayo->setAfectaDetrasCamara(false);
    this->rayo->setAfectaLuces(false);

    ModoJuego::getInstance()->getEscena()->addDibujable(this->rayo);


    Objeto3d *parte;
    char id[20];

    for(int i=0;i<cant;i++){
        parte=new Objeto3d("electricidad.mdl","electricidad.tga");
        parte->setAfectaLuces(false);
        parte->setModoTransparencia(Aditivo);
        parte->setAfectaDetrasCamara(false);
        //parte->setCullFace(false);

        sprintf(id,"parte_%d",i);
        this->rayo->addObjeto(id,parte);
    }

}

int Electricidad::getCantPartes(){
    return this->cantPartes;
}

void Electricidad::setCurvatura(float c){
    curvatura=c;
}
float Electricidad::getCurvatura(){
    return curvatura;
}

Multiobjeto3d *Electricidad::getMultiobjeto(){
    return this->rayo;
}

void Electricidad::setFrecuencia(float frec){
    if(frec>0)
        periodo=1/frec;
    else
        periodo=0;
}
float Electricidad::getFrecuencia(){
    if(periodo>0)
        return 1/periodo;
    else
        return 0;
}
