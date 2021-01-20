#include "Matriz4x4.h"
#include "math.h"

//Constructores
Matriz4x4::Matriz4x4(){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j]=0;
}

Matriz4x4::Matriz4x4(float **m){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j]=m[i][j];
}

Matriz4x4::Matriz4x4(const Matriz4x4 &m){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j]=m.m[i][j];
}

//Asignacion
Matriz4x4& Matriz4x4::operator=(const Matriz4x4 &m){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j]=m.m[i][j];
    return *this;
}

//Indexacion
float& Matriz4x4::operator()(unsigned int i, unsigned int j){
    return this->m[i][j];
}

//Sumas y restas
Matriz4x4 Matriz4x4::operator+(const Matriz4x4 &m)const{
    Matriz4x4 m2;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            m2.m[i][j]=this->m[i][j]+m.m[i][j];
    return m2;
}
Matriz4x4 Matriz4x4::operator-(const Matriz4x4 &m)const{
    Matriz4x4 m2;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            m2.m[i][j]=this->m[i][j]-m.m[i][j];
    return m2;
}

Matriz4x4& Matriz4x4::operator+=(const Matriz4x4 &m){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j]+=m.m[i][j];
    return *this;
}

Matriz4x4& Matriz4x4::operator-=(const Matriz4x4 &m){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j]-=m.m[i][j];
    return *this;
}

//Producto y division por un escalar
Matriz4x4 Matriz4x4::operator*(float f)const{
    Matriz4x4 m2;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            m2.m[i][j]=this->m[i][j]*f;
    return m2;
}
Matriz4x4 Matriz4x4::operator/(float f)const{
    Matriz4x4 m2;
    float inv=1.0/f;

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            m2.m[i][j]=this->m[i][j]*inv;
    return m2;
}

Matriz4x4& Matriz4x4::operator*=(float f){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j]*=f;
    return *this;
}

Matriz4x4 operator*(float f,const Matriz4x4 &m){
    return m*f;
}


Matriz4x4& Matriz4x4::operator/=(float f){
    float inv=1.0/f;

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j]*=inv;
    return *this;
}

//Producto entre matrices
Matriz4x4 Matriz4x4::operator*(const Matriz4x4 &m)const{
    Matriz4x4 m2;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            for(int k=0;k<4;k++)
                m2.m[i][j]+=(this->m[i][k]*m.m[k][j]);

    return m2;
}

Matriz4x4& Matriz4x4::operator*=(const Matriz4x4 &m){
    *this=(*this)*m;
    return *this;
}

//Producto entre matriz y vector
Vector Matriz4x4::operator*(Vector &v)const{
    float w[4]={v[X],v[Y],v[Z],1};
    float res[4]={0,0,0,0};

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            res[i]+=(this->m[i][j]*w[j]);

    if(res[3]==0)
        return Vector(res);
    else
        return Vector(res)/res[3];
}

//Comparacion
bool Matriz4x4::operator==(const Matriz4x4 &m)const{

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(this->m[i][j]!=m.m[i][j])
                return false;

    return true;
}

bool Matriz4x4::operator!=(const Matriz4x4 &m)const{

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(this->m[i][j]!=m.m[i][j])
                return true;

    return false;
}

//Operaciones utilitarias
Matriz4x4 Matriz4x4::transpuesta() const{
    Matriz4x4 m2;

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            m2.m[i][j]=this->m[j][i];
    return m2;
}

//Operaciones estaticas
Matriz4x4 Matriz4x4::Nula(){
    Matriz4x4 m;
    return m;
}

Matriz4x4 Matriz4x4::Identidad(){
    Matriz4x4 m;
    m(0,0)=m(1,1)=m(2,2)=m(3,3)=1;
    return m;
}
Matriz4x4 Matriz4x4::Escalado(float x,float y,float z){
    Matriz4x4 m;
    m(0,0)=x;
    m(1,1)=y;
    m(2,2)=z;
    m(3,3)=1;
    return m;
}
Matriz4x4 Matriz4x4::Escalado(Vector v){
    Matriz4x4 m;
    m(0,0)=v[X];
    m(1,1)=v[Y];
    m(2,2)=v[Z];
    m(3,3)=1;
    return m;
}
Matriz4x4 Matriz4x4::Traslacion(float x,float y,float z){
    Matriz4x4 m=Identidad();
    m(0,3)=x;
    m(1,3)=y;
    m(2,3)=z;
    m(3,3)=1;
    return m;
}
Matriz4x4 Matriz4x4::Traslacion(Vector v){
    Matriz4x4 m=Identidad();
    m(0,3)=v[X];
    m(1,3)=v[Y];
    m(2,3)=v[Z];
    m(3,3)=1;
    return m;
}
Matriz4x4 Matriz4x4::RotacionX(double angulo){
    Matriz4x4 m;
    double c=cos(angulo),s=sin(angulo);
    m(0,0)=m(3,3)=1;
    m(1,1)=m(2,2)=c;
    m(2,1)=s;
    m(1,2)=-s;
    return m;
}

Matriz4x4 Matriz4x4::RotacionY(double angulo){
    Matriz4x4 m;
    double c=cos(angulo),s=sin(angulo);
    m(1,1)=m(3,3)=1;
    m(0,0)=m(2,2)=c;
    m(2,0)=-s;
    m(0,2)=s;
    return m;
}

Matriz4x4 Matriz4x4::RotacionZ(double angulo){
    Matriz4x4 m;
    double c=cos(angulo),s=sin(angulo);
    m(2,2)=m(3,3)=1;
    m(0,0)=m(1,1)=c;
    m(1,0)=s;
    m(0,1)=-s;
    return m;
}

Matriz4x4 Matriz4x4::Rotacion(double angulo,Vector eje){
    float norma=eje.normalizar();
    if(norma==0)
        return Matriz4x4::Identidad();

    Matriz4x4 s;
    s(0,1)=-eje[Z];
    s(0,2)=+eje[Y];
    s(1,0)=+eje[Z];
    s(1,2)=-eje[X];
    s(2,0)=-eje[Y];
    s(2,1)=+eje[X];

    return Matriz4x4::Identidad() + sin(angulo)*s + (1-cos(angulo))*(s*s);
}

Matriz4x4 Matriz4x4::Rotacion(double angulo,float x,float y,float z){
    return Matriz4x4::Rotacion(angulo,Vector(x,y,z));
}
