#ifndef MATEMATICAS_H
#define MATEMATICAS_H

#include "Vector.h"
#include "Matriz4x4.h"

#ifndef PI
#define PI 3.1415926535897932384626
#endif

double Abs(double x);
double Sgn(double x);
int Redondear(double x);
double Redondear(double x,unsigned int cifras);

void InicializarSemilla();

double Random();
double Random(double);
double Random(double,double);

double Radianes(double grados);
double Grados(double radianes);

double Min(double n1,double n2);
double Max(double n1,double n2);

Vector Min(Vector v1,Vector v2);
Vector Max(Vector v1,Vector v2);

//Devuelve un valor que es igual a valor si este pertenece al rango [min,max]
//sino si es menor devuelve min y sino devuelve max
double MantenerEntre(double min,double valor,double max);

//Proyeccion Ortogonal del vector v sobre el vector de direccion
Vector ProyeccionOrtogonal(Vector v,Vector direccion);

//Proyeccion Ortogonal del vector v sobre el plano determinado por los vectores de direccion
Vector ProyeccionOrtogonal(Vector v,Vector direccion1,Vector direccion2);

//Simetriza al vector v, respecto del eje direccion
Vector SimetriaAxial(Vector v,Vector direccion);

//Simetriza al vector v, respecto de los ejes de direccion del plano
Vector SimetriaPlanar(Vector v,Vector direccion1, Vector direccion2);

//Transforma las coordenadas esfericas de la forma (Rho,Phi,Theta) a cartesianas de la forma (X,Y,Z)
Vector Esfericas2Cartesianas(Vector esfericas);

//Transforma las coordenadas cartesianas a esfericas
Vector Cartesianas2Esfericas(Vector cartesianas);

//Dados los vertices de un triangulo cualquiera, devuelve su area
double AreaTriangulo(Vector v1,Vector v2, Vector v3);

//Dados los vertices de un tetraedro cualquiera, devuelve su volumen
double VolumenTetraedro(Vector v1,Vector v2, Vector v3,Vector v4);


#endif
