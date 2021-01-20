#include "Objeto3d.h"

Objeto3d::Objeto3d():Dibujable(){
    modelo=NULL;
    textura=NULL;
    modoTextura=Modular;
    setModoGeneracionCoordTextura(Ninguno);
    cullFace=true;
}

Objeto3d::Objeto3d(const char *mdl,bool optimizarVertices):Dibujable(){
    modelo=ManejadorModelos::getInstancia()->getModelo(mdl,optimizarVertices);
    textura=NULL;
    modoTextura=Modular;
    setModoGeneracionCoordTextura(Ninguno);
    cullFace=true;
}

Objeto3d::Objeto3d(const char *mdl,const char *tex,bool optimizarVertices):Dibujable(){
    //cout << "entre en objeto 3d " << tex << endl ;
    modelo=ManejadorModelos::getInstancia()->getModelo(mdl,optimizarVertices);
    textura=ManejadorTexturas::getInstancia()->getTextura(tex);
    modoTextura=Modular;
    setModoGeneracionCoordTextura(Ninguno);
    cullFace=true;
}

Objeto3d::~Objeto3d(){
}

void Objeto3d::dibujar(){

    //Setea texturas, materiales y renderiza objeto

    if(modelo){

        if(textura){
            //Modo de renderizado de textura
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, modoTextura);


            if(modoGeneracionCoordTextura!=Ninguno){
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
            }

            //Modo de mapeo de coordenadas
            switch(modoGeneracionCoordTextura){
                case Ninguno:
                    glDisable(GL_TEXTURE_GEN_S);
                    glDisable(GL_TEXTURE_GEN_T);
                break;

                case Enviromental:
                    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                break;

                case LinealObjeto:
                    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
                    glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
                    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
                    glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);

                break;

                case LinealVista:
                    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
                    glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
                    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
                    glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);
                break;

            }

            textura->toGL();
        }else
            glDisable(GL_TEXTURE_2D);


        if(cullFace)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);


        material.toGL(color,opacidad);

        if(nombreMalla=="") //Si no tengo una malla asignada, dibujo el modelo completo
            modelo->toGL();
        else
            modelo->toGL(nombreMalla);
    }

}

void Objeto3d::cargarModelo(const char *mdl,bool optimizarVertices){
    modelo=ManejadorModelos::getInstancia()->getModelo(mdl,optimizarVertices);
}

void Objeto3d::cargarTextura(const char *tex){
    textura=ManejadorTexturas::getInstancia()->getTextura(tex);
}

bool Objeto3d::getGullFace(){
    return cullFace;
}
void Objeto3d::setCullFace(bool valor){
    cullFace=valor;
}

void Objeto3d::setModoTextura(ModoTextura modo){
    modoTextura=modo;
}
ModoTextura Objeto3d::getModoTextura(){
    return modoTextura;
}

void Objeto3d::setModoGeneracionCoordTextura(ModoGeneracionCoordText modo,Vector S,Vector T){
    modoGeneracionCoordTextura=modo;
    int i;
    for(i=0;i<3;i++){
        planoS[i]=S[i];
        planoT[i]=T[i];
    }
    planoS[3]=planoT[3]=0;
}
ModoGeneracionCoordText Objeto3d::getModoGeneracionCoordTextura(){
    return modoGeneracionCoordTextura;
}

void Objeto3d::setNombreMalla(string nombre){
    nombreMalla=nombre;
}
string Objeto3d::getNombreMalla(){
    return nombreMalla;
}

void Objeto3d::setMaterial(const Material& mat){
    this->material=mat;
}
Material& Objeto3d::getMaterial(){
    return this->material;
}

bool Objeto3d::getAfectaLuces(){
    if(!afectaLuces) return false;

    if(modoTextura==Reemplazar) return false;

    return true;
}

bool Objeto3d::esTransparente(){
    if(opacidad<1){
        if(!textura) //Si no tengo textura es transparente por la opacidad
            return true;
        else //si tengo textura y es 32 bits, es transparente. Si no es de 32 bits pero el modo no es Reemplazar => es transparente
            return (modoTextura!=Reemplazar || textura->getBpp()==32);
    }else if(textura) //Si tengo textura de 32 bits y no es decal, es transparente
        return (modoTextura!=Decal && textura->getBpp()==32);
    else //No es transparente por la opacidad y porque no tiene textura
        return false;
}

Esfera Objeto3d::getEsferaEnvolvente(){
    if(!modelo) return Esfera();

    Vector x(1,0,0);
    Vector y(0,1,0);
    Vector z(0,0,1);

    Esfera e;
    if(nombreMalla=="")
        e=modelo->getEsferaEnvolvente();
    else
        e=modelo->getEsferaEnvolvente(nombreMalla);

    Vector centro=e.getCentro();
    centro=this->transformacion*centro;

    Matriz4x4 m=getRotaciones();

    x=m*x;
    y=m*y;
    z=m*z;

    float max=e.getRadio()*Max(x.norma(),Max(y.norma(),z.norma()));

    return Esfera(centro,max);
}
Modelo*Objeto3d::getModelo(){
	return this->modelo;
}
Textura*Objeto3d::getTextura(){
	return this->textura;
}
