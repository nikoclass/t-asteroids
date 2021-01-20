#include "Texto2D.h"

Texto2D::Texto2D(string texto,float espacioHorizontal,float espacioVertical,string textura):Dibujable(){
    this->afectaLuces=false;
    this->mirarCamara=true;
    this->textura=ManejadorTexturas::getInstancia()->getTextura(textura.data());
    this->idList=glGenLists(1);
    this->setTexto(texto,espacioHorizontal,espacioVertical);
}

Texto2D::~Texto2D(){
    glDeleteLists(this->idList,1);
}

void Texto2D::cargarTextura(string textura){
    this->textura=ManejadorTexturas::getInstancia()->getTextura(textura.data());
}

void Texto2D::setTexto(string texto,float espacioHorizontal,float espacioVertical){

    this->texto=texto;

    float f=1/16.0,tx,ty,offX=0,offY=0;
    int tam=texto.size();
    char c;
    Vector max;

    glNewList (this->idList, GL_COMPILE);
        glBegin(GL_QUADS);
            for(int i=0;i<tam;i++){

                c=texto[i];
                tx=(c%16)*f;
                ty=(c/16)*f;

                //Izquierda abajo
                glTexCoord2f(tx,ty);
                glVertex3f(offX,1-offY,0);

                //derecha abajo
                glTexCoord2f(tx+f,ty);
                glVertex3f(1+offX,1-offY,0);

                //derecha arriba
                glTexCoord2f(tx+f,ty+f);
                glVertex3f(1+offX,-offY,0);

                //izquierda arriba
                glTexCoord2f(tx,ty+f);
                glVertex3f(offX,-offY,0);

                if(c=='\n'){
                    offX=0;
                    offY+=espacioVertical;
                }else if(c=='\t'){
                    offX+=4*espacioVertical;
                }else{
                    offX+=espacioHorizontal;
                }

                max=Max(max,Vector(offX,offY,0));
            }

        glEnd();
    glEndList();

    centroX=max[X]*0.5;
    centroY=(max[Y]+espacioVertical)*0.5;

}
string Texto2D::getTexto(){
    return this->texto;
}

void Texto2D::setMirarCamara(bool val){
    mirarCamara=val;
}
bool Texto2D::getMirarCamara(){
    return mirarCamara;
}

void Texto2D::dibujar(){

    if(!textura) return;

    textura->toGL();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, Modular);
    glDisable(GL_CULL_FACE);
    //glColor4f(1,1,1,opacidad);

    if(mirarCamara && afectaVista){
        Matriz4x4 m;
        glToMatriz(GL_MODELVIEW_MATRIX,m);
        m(0,3)=m(1,3)=m(2,3)=0;
        m=m.transpuesta();
        matrizToGL(m);
    }

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    if(afectaVista)
        glTranslatef(-centroX,centroY,0);
    glCallList(this->idList);
}

bool Texto2D::getAfectaLuces(){
    return false;
}

bool Texto2D::esTransparente(){
    if(opacidad<1){
        if(!textura) //Si no tengo textura es transparente por la opacidad
            return true;
        else //si tengo textura y es 32 bits, es transparente. Si no es de 32 bits pero el modo no es Reemplazar => es transparente
            return (textura->getBpp()==32);
    }else if(textura) //Si tengo textura de 32 bits y no es decal, es transparente
        return (textura->getBpp()==32);
    else //No es transparente por la opacidad y porque no tiene textura
        return false;
}

