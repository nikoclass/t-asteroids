#include "Texto3D.h"

Texto3D::Texto3D(string texto,string charset){
    this->multiobjeto=NULL;
    this->texto=texto;
    this->dummy=new Objeto3d();
    cargarCharset(charset);
}

void Texto3D::cargarCharset(string charset){
    Objeto3d *obj3d;
    int len=texto.size();

    float x=0,y=0;

    if(multiobjeto)
        delete multiobjeto;

    multiobjeto=new Multiobjeto3d();

    char num[10];
    string s;

    for(int i=0;i<len;i++){
        if(texto[i]=='\n'){
            y+=1;
            x=0;
        }else if(texto[i]==' '){
            x+=1;
        }else if(texto[i]=='\t'){
            x+=4;
        }else{
            const char *nombre=charset.c_str();
            obj3d=new Objeto3d(nombre);
            s=texto[i];
            obj3d->setNombreMalla(s.c_str()); //Si no existe una malla con el nombre del caracter, entonces se ignora

            x+=obj3d->getEsferaEnvolvente().getRadio();
            obj3d->trasladar(Vector(x,-y,0));
            sprintf(num,"%d",i);
            multiobjeto->addObjeto(num,obj3d);
            //x+=1;
            x+=obj3d->getEsferaEnvolvente().getRadio();
        }
    }
    this->charset=charset;
}

void Texto3D::setTexto(string texto){
    this->texto=texto;
    cargarCharset(this->charset);
}

string Texto3D::getTexto(){
    return this->texto;
}

Objeto3d* Texto3D::getCaracter(int num){
    char s[10];
    sprintf(s,"%d",num);
    Objeto3d *ret;
    ret=dynamic_cast<Objeto3d*>(this->multiobjeto->getObjeto(s));
    if(ret)
        return ret;
    else
        return dummy;
}

Esfera Texto3D::getEsferaEnvolvente(){ //override de dibujable
    return this->multiobjeto->getEsferaEnvolvente();
}
void Texto3D::dibujar(){ //override de dibujable
    multiobjeto->dibujar();
}
Texto3D::~Texto3D(){
    delete multiobjeto;
    delete dummy;
}
