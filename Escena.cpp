#include "Escena.h"

Escena::Escena(){
    this->camaraActual=NULL;
    this->luzAmbiente=Vector(0.2,0.2,0.2);
    this->factorAmbiente=this->factorDifuso=this->factorEspecular=1;
    this->tipoRenderizado=Interpolado;
    this->texturas=true;
}

Escena::~Escena(){
}

void Escena::setFactorAmbiente(float factor){
    this->factorAmbiente=factor;
}
float Escena::getFactorAmbiente(){
    return this->factorAmbiente;
}
void Escena::setFactorDifuso(float factor){
    this->factorDifuso=factor;
}
float Escena::getFactorDifuso(){
    return this->factorDifuso;
}

void Escena::setFactorEspecular(float factor){
    this->factorEspecular=factor;
}
float Escena::getFactorEspecular(){
    return this->factorEspecular;
}
void Escena::setTipoRenderizado(TipoRenderizado tipo){
    this->tipoRenderizado=tipo;
}
TipoRenderizado Escena::getTipoRenderizado(){
    return this->tipoRenderizado;
}

Camara* Escena::getCamaraActual(){
    return this->camaraActual;
}

void Escena::setLuzAmbiente(Vector luz){
    this->luzAmbiente=luz;
}
Vector Escena::getLuzAmbiente(){
    return this->luzAmbiente;
}

void Escena::setearParametrosLuces(Dibujable *dib){
    if(!dib->getAfectaLuces()) return;

    Esfera eluz,edib=dib->getEsferaEnvolvente();;

    int cant=0;
    set<Luz*>::iterator iter=luces.begin();

    while(cant<8 && iter!=luces.end()){
        if((*iter)->getIntensidad()>0){ //Solo si esta prendida
            eluz=Esfera((*iter)->getPos(),(*iter)->getDistanciaMaxima());

            if(eluz.getRadio()==0 || eluz.intersecta(edib)){ //lo esta iluminando con su luz
                (*iter)->toGL(GL_LIGHT0+cant);
                cant++;
            }
        }
        iter++;
    }

    //Deshabilito el resto de las luces, si es que hay
    for(int j=cant;j<8;j++)
        glDisable(GL_LIGHT0+j);

}

void Escena::setTexturas(bool val){
    this->texturas=val;
}
bool Escena::getTexturas(){
    return this->texturas;
}

void Escena::toGL(){

    //Limpio la pantalla completa
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat lmodel_ambient[4] = {luzAmbiente[RED], luzAmbiente[GREEN],luzAmbiente[BLUE], 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    switch(this->tipoRenderizado){
        case Wireframe:
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        break;
        case Facetado:
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glShadeModel(GL_FLAT);
        break;

        case Interpolado:
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glShadeModel(GL_SMOOTH);
        break;
    }


    //Por cada camara registrada
    map<int,Camara*>::iterator itercamara;
    for(itercamara=camaras.begin();itercamara!=camaras.end();itercamara++){

        camaraActual=itercamara->second;
        camaraActual->toGL();

        glDepthMask(GL_TRUE); //limpio el buffer de profundidad
        glClear(GL_DEPTH_BUFFER_BIT);



        set<Dibujable*>::iterator iter;
        glDepthMask(GL_TRUE);
        //Por cada objeto opaco...
        for(iter=dibujables.begin(); iter!=dibujables.end(); iter++){
            if((*iter)->esTransparente()==false){
                setearParametrosLuces(*iter);
                (*iter)->toGL();
            }
        }

        //Por cada fuente de luz posiciona sus LensFlares correspondiente
        set<Luz*>::iterator iterluz=luces.begin();
        for(iterluz=luces.begin();iterluz!=luces.end();iterluz++){
            (*iterluz)->dibujarLensFlares();
        }


        //Por cada objeto transparente...
        glDepthMask(GL_FALSE);
        for(iter=dibujables.begin(); iter!=dibujables.end(); iter++){
            if((*iter)->esTransparente()==true){
                setearParametrosLuces(*iter);
                (*iter)->toGL();
            }
        }

    }

    glFlush();
    glutSwapBuffers();
}
