#include "math.h"
#include "Vector.h"

//Ahora son todas las funciones inline para mejorar el rendimiento

#ifndef QUIERO_INLINE
//Constructores
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
