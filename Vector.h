#ifndef VECTOR_H
#define VECTOR_H


class Vector;
#include <math.h>
#include <iostream>
using namespace std;

//Forma vieja (da conflictos con la porqueria de la API de windows
/*#define X 0
#define Y 1
#define Z 2

#define RHO 0
#define PHI 1
#define THETA 2

#define RED 0
#define GREEN 1
#define BLUE 2

#define U 0
#define V 1
#define W 2
*/


//Forma un poco mas evolucionada
namespace ConstantesVector{

const int X=0;
const int Y=1;
const int Z=2;

const int RHO=0;
const int PHI=1;
const int THETA=2;

const int RED=0;
const int GREEN=1;
const int BLUE=2;

const int U=0;
const int V=1;
const int W=2;

}

using namespace ConstantesVector;

#define cout_vector(v) cout<<#v<<"=("<<v[X]<<","<<v[Y]<<","<<v[Z]<<")"

///Comentar la linea de abajo si no se quieren funciones inline
#define QUIERO_INLINE

#ifdef QUIERO_INLINE
#define Inline inline
#else
#define Inline
#endif

//Ahora son todas las funciones inline para mejorar el rendimiento

class Vector{
    private:
        float v[3];

    public:

        //Constructores
        Inline Vector(const Vector &v);
        Inline explicit Vector(float *v);
        Inline explicit Vector(float x=0.0,float y=0.0,float z=0.0);

        //Asignacion
        Inline Vector& operator=(const Vector &v);

        //Indexacion
        Inline float& operator[](unsigned int i);

        //Conversion
        //operator float*(); //Da problemas con la indexacion

        //Sumas y restas
        Inline Vector operator+(const Vector &v)const;
        Inline Vector operator-(const Vector &v)const;

        Inline Vector& operator+=(const Vector &v);
        Inline Vector& operator-=(const Vector &v);

        //Multiplicacion punto a punto
        Inline Vector operator&(const Vector &v)const;
        Inline Vector& operator&=(const Vector &v);

        //Multiplicaciones y divisiones escalares
        Inline Vector operator*(float f)const;
        Inline Vector operator/(float f)const;

        Inline Vector& operator*=(float f);
        Inline Vector& operator/=(float f);

        //Producto interno y vectorial
        Inline float operator*(const Vector &v)const;
        Inline Vector operator^(const Vector &v)const;

        Inline Vector& operator^=(const Vector &v);

        //Comparaciones
        Inline bool operator==(const Vector &v);
        Inline bool operator!=(const Vector &v);

        //Negativo y positivo
        Inline Vector operator+()const;
        Inline Vector operator-()const;


        //Normas, direccion y distancias
        Inline float norma()const;
        Inline float norma2()const;
        Inline float normalizar();
        Inline Vector direccion()const;
        Inline float distancia(const Vector &v)const;
        Inline float distancia2(const Vector &v)const;

};

Inline Vector operator*(float f,const Vector &v);


#ifdef QUIERO_INLINE
//Comienzan los metodos:

Vector::Vector(float *v){
    this->v[X]=v[X];
    this->v[Y]=v[Y];
    this->v[Z]=v[Z];
}

Vector::Vector(const Vector &v){
    this->v[X]=v.v[X];
    this->v[Y]=v.v[Y];
    this->v[Z]=v.v[Z];
}

Vector::Vector(float x,float y,float z){
    this->v[X]=x;
    this->v[Y]=y;
    this->v[Z]=z;
}

//Asignacion
Vector& Vector::operator=(const Vector &v){
    this->v[X]=v.v[X];
    this->v[Y]=v.v[Y];
    this->v[Z]=v.v[Z];
    return *this;
}

//Indexacion
float& Vector::operator[](unsigned int i){
    return this->v[i];
}

//Conversion
/*Vector::operator float*(){
    return this->v;
}*/ //Da problemas con la indexacion

//Sumas y restas
Vector Vector::operator+(const Vector &v)const{
    return Vector(this->v[X]+v.v[X],this->v[Y]+v.v[Y],this->v[Z]+v.v[Z]);
}

Vector Vector::operator-(const Vector &v)const{
    return Vector(this->v[X]-v.v[X],this->v[Y]-v.v[Y],this->v[Z]-v.v[Z]);
}

Vector& Vector::operator+=(const Vector &v){
    this->v[X]+=v.v[X];
    this->v[Y]+=v.v[Y];
    this->v[Z]+=v.v[Z];
    return *this;
}

Vector& Vector::operator-=(const Vector &v){
    this->v[X]-=v.v[X];
    this->v[Y]-=v.v[Y];
    this->v[Z]-=v.v[Z];
    return *this;
}

//Multiplicaciones y divisiones escalares
Vector Vector::operator*(float f)const{
    return Vector(this->v[X]*f,this->v[Y]*f,this->v[Z]*f);
}

Vector Vector::operator/(float f)const{
    float inv=1.0/f;
    return Vector(this->v[X]*inv,this->v[Y]*inv,this->v[Z]*inv);
}

Vector& Vector::operator*=(float f){
    this->v[X]*=f;
    this->v[Y]*=f;
    this->v[Z]*=f;
    return *this;
}

Vector& Vector::operator/=(float f){
    float inv=1.0/f;
    this->v[X]*=inv;
    this->v[Y]*=inv;
    this->v[Z]*=inv;
    return *this;
}

//Multiplicacion punto a punto
Vector Vector::operator&(const Vector &v)const{
    return Vector(this->v[0]*v.v[0],this->v[1]*v.v[1],this->v[2]*v.v[2]);
}

Vector& Vector::operator&=(const Vector &v){
    this->v[0]*=v.v[0];
    this->v[1]*=v.v[1];
    this->v[2]*=v.v[2];
    return *this;
}

//Producto interno y vectorial
float Vector::operator*(const Vector &v)const{
    return this->v[X]*v.v[X] + this->v[Y]*v.v[Y] + this->v[Z]*v.v[Z];
}

Vector Vector::operator^(const Vector &v)const{
    return Vector(this->v[Y]*v.v[Z] - this->v[Z]*v.v[Y],
                  -this->v[X]*v.v[Z] + this->v[Z]*v.v[X],
                  this->v[X]*v.v[Y] - this->v[Y]*v.v[X]);
}

Vector& Vector::operator^=(const Vector &v){
    Vector w=*this;
    *this=w^v;
    return *this;
}

//Comparaciones
bool Vector::operator==(const Vector &v){
    return (this->v[X]==v.v[X] && this->v[Y]==v.v[Y] && this->v[Z]==v.v[Z]);
}

bool Vector::operator!=(const Vector &v){
    return !(this->v[X]==v.v[X] && this->v[Y]==v.v[Y] && this->v[Z]==v.v[Z]);
}

//Normas y distancias
float Vector::norma()const{
	//cout << "Vector::norma() ("<<v[X]<<","<<v[Y]<<","<<v[Z]<< ")"<< endl;
	//cout << "Vector::norma() " << sqrt((*this)*(*this)) << endl;
    return sqrt((*this)*(*this));
}
float Vector::norma2()const{
    return (*this)*(*this);
}
float Vector::normalizar(){
    float norma=this->norma();
    if(norma>0)
        *this/=norma;
    return norma;
}

Vector Vector::direccion()const{
    Vector v=*this;
    v.normalizar();
    return v;
}

float Vector::distancia(const Vector &v)const{
    return (*this - v).norma();
}
float Vector::distancia2(const Vector &v)const{
    return (*this - v).norma2();
}

Vector operator*(float f,const Vector &v){
    return v*f;
}

Vector Vector::operator+()const{
    return *this;
}

Vector Vector::operator-()const{
    return Vector(-this->v[X],-this->v[Y],-this->v[Z]);
}

#endif

#endif
