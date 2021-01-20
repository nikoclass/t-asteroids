#include "LensFlare.h"

LensFlare::LensFlare(Luz* luz){
    this->luz=luz;
    this->escena=luz->getEscena();

    if(!this->escena)this->escena=Engine::getInstancia()->getEscena();

    char* nombre[CANT_FLARES]={"part_ring.tga","flare2.tga","part_ring.tga","flare1.tga","flare3.tga","part_glow.tga","flare2.tga"};

    for(int i=0;i<CANT_FLARES;i++){
        this->flares[i]=new Sprite(nombre[i]);
        this->flares[i]->setModoTransparencia(Aditivo);
        this->flares[i]->setAfectaVista(false);
        this->flares[i]->setAfectaZbuffer(false);
        this->escena->addDibujable(flares[i]);
    }

}

LensFlare::~LensFlare(){
    for(int i=0;i<CANT_FLARES;i++){
        delete this->flares[i];
    }
}

void LensFlare::toGL(){
    float tams[CANT_FLARES]={0.1,0.3,0.2,0.7,0.6,0.1,0.1};
    float opacidades[CANT_FLARES]={0.3,0.5,0.3,1,0.7,0.3,0.2};

    Matriz4x4 vista,proyeccion,m;
    Vector pos=this->luz->getPos();
    Camara *c=this->escena->getCamaraActual();
    Vector posCamara=c->getPosicion();

    Dibujable::glToMatriz(GL_MODELVIEW_MATRIX,vista);
    pos=vista*pos;

    float factor=Vector(0,0,-1)*pos.direccion();
    factor*=factor;

    Vector proyectado=Dibujable::proyectar(this->luz->getPos());
    float z;
    glReadPixels((int)proyectado[X],(int)proyectado[Y],1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);

    float factorDeTapado=factoresDeTapado[c];
    if(z<proyectado[Z]){//Lo tapa algun objeto opaco
        factorDeTapado=Max(0,factorDeTapado-0.03);
    }else{//No lo tapan
        factorDeTapado=Min(1,factorDeTapado+0.03);
    }
    factoresDeTapado[c]=factorDeTapado;


    if(pos[Z]<0 && factorDeTapado>0){

        Dibujable::glToMatriz(GL_PROJECTION_MATRIX,proyeccion);
        pos=proyeccion*pos;

        float opacidad=1;
        float dist_max=luz->getDistanciaMaxima(),dist=posCamara.distancia(this->luz->getPos());

        if(dist_max>0)
            opacidad*=Max(0,1-dist/dist_max);

        float f;
        for(int i=0;i<CANT_FLARES;i++){

            flares[i]->setVisible(true);
            flares[i]->identidad();
            flares[i]->setColor(this->luz->getColor());
            if(opacidades[i]<1)
                flares[i]->setOpacidad(opacidad*factor*opacidades[i]*factorDeTapado);
            else
                flares[i]->setOpacidad(factorDeTapado);
            flares[i]->escalar(tams[i]*opacidad);

            f=0.5+(float)(i-1)/CANT_FLARES;
            this->flares[i]->posicionar(Vector(pos[X]*f,pos[Y]*f,-pos[Z]));
        }
    }else{
        for(int i=0;i<CANT_FLARES;i++)
            flares[i]->setVisible(false);
    }
}
