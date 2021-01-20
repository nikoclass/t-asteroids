#include "Esfera.h"

Esfera::Esfera(){
    radio=0;
}
Esfera::Esfera(Vector centro,float radio):centro(centro),radio(radio){
}

Vector Esfera::getCentro(){
    return centro;
}
float Esfera::getRadio(){
    return radio;
}

bool Esfera::intersecta(Esfera &otra){
    float dist=this->centro.distancia2(otra.centro);
    float radios=this->radio+otra.radio;
    return (dist<=radios*radios);
}
