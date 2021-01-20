#include "Matematicas.h"
#include <stdlib.h>
#include <math.h>
#include "time.h"

double Abs(double x) {
    return x<0?-x:x;
}

int Redondear(double x) {
    return (int)(x+0.5);
}
double Redondear(double x,unsigned int cifras) {
    double m=pow(10,cifras);
    return Redondear(x*m)/m;
}

double Sgn(double x){
    if(x>0)
        return 1;
    else if (x<0)
        return -1;
    else
        return 0;
}

void InicializarSemilla(){
    srand(time(NULL));
}

//Funciones sobrecargadas de numeros pseudo aleatorios
double Random() { //Devuelve un numero perteneciente a [0,1]
    return (double)rand()/RAND_MAX;
}
double Random(double n) {//Devuelve un numero perteneciente a [0,n]
    return Random()*n;
}
double Random(double min,double max) {//Devuelve un numero perteneciente a [min,max]
    return Random(max-min)+min;
}

double Radianes(double grados){
    return grados/180.0*PI;
}
double Grados(double radianes){
    return radianes/PI*180.0;
}

double Min(double n1,double n2){
    return n1<n2?n1:n2;
}
double Max(double n1,double n2){
    return n1>n2?n1:n2;
}

Vector Min(Vector v1,Vector v2){
    return Vector(Min(v1[X],v2[X]),Min(v1[Y],v2[Y]),Min(v1[Z],v2[Z]));
}
Vector Max(Vector v1,Vector v2){
    return Vector(Max(v1[X],v2[X]),Max(v1[Y],v2[Y]),Max(v1[Z],v2[Z]));
}


//Devuelve un valor que es igual a valor si este pertenece al rango [min,max]
//sino si es menor devuelve min y sino devuelve max
double MantenerEntre(double min,double valor,double max){
    if(valor<min)
        valor=min;
    if(valor>max)
        valor=max;
    return valor;
}

//Proyeccion Ortogonal del vector v sobre el vector de direccion
Vector ProyeccionOrtogonal(Vector v,Vector direccion){
    direccion.normalizar();
    return (v*direccion)*direccion;
}

//Proyeccion Ortogonal del vector v sobre el plano determinado por los vectores de direccion
Vector ProyeccionOrtogonal(Vector v,Vector direccion1,Vector direccion2){
    Vector n=direccion1^direccion2;
    n.normalizar();
    return v - (v*n)*n;
}

//Simetriza al vector v, respecto del eje direccion
Vector SimetriaAxial(Vector v,Vector direccion){
    direccion.normalizar();
    return 2*(v*direccion)*direccion - v;
}

//Simetriza al vector v, respecto de los ejes de direccion del plano
Vector SimetriaPlanar(Vector v,Vector direccion1, Vector direccion2){
    Vector n=direccion1^direccion2;
    n.normalizar();
    return v-2*(v*n)*n;
}

//Transforma las coordenadas esfericas de la forma (Rho,Phi,Theta) a cartesianas de la forma (X,Y,Z)
Vector Esfericas2Cartesianas(Vector esfericas){
    double Rho,SinPhi,CosPhi,SinTheta,CosTheta;

    Rho=esfericas[RHO];
    SinPhi=sin(esfericas[PHI]);
    CosPhi=cos(esfericas[PHI]);
    SinTheta=sin(esfericas[THETA]);
    CosTheta=cos(esfericas[THETA]);

    return Vector(Rho*CosPhi*SinTheta,Rho*SinPhi*SinTheta,Rho*CosTheta);
}

//Transforma las coordenadas cartesianas a esfericas
Vector Cartesianas2Esfericas(Vector cartesianas){
    float Rho=cartesianas.norma();
    float Theta=acos(cartesianas[Z]/Rho);
    float Phi=atan2(cartesianas[Y],cartesianas[Z]);

    return Vector(Rho,Phi,Theta);
}

//Dados los vertices de un triangulo cualquiera, devuelve su area
double AreaTriangulo(Vector v1,Vector v2, Vector v3){
    Vector d1=v2-v1,d2=v3-v1;
    Vector altura=d1-ProyeccionOrtogonal(d1,d2);

    return d2.norma()*altura.norma()/2.0;
}

//Dados los vertices de un tetraedro cualquiera, devuelve su volumen
double VolumenTetraedro(Vector v1,Vector v2, Vector v3,Vector v4){
    v2=v2-v1;
    v3=v3-v1;
    v4=v4-v1;
    v1=v1-v1;

    double area=AreaTriangulo(v1,v2,v3);
    Vector altura=v4-ProyeccionOrtogonal(v4,v2,v3);

    return area*altura.norma()/3.0;
}
