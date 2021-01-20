#ifndef MATRIZ4X4_H
#define MATRIZ4X4_H

#include "Vector.h"

class Matriz4x4
{
    private:

        float m[4][4];

    public:

        //Constructores
        explicit Matriz4x4();
        explicit Matriz4x4(float **m);
        Matriz4x4(const Matriz4x4 &m);

        //Asignacion
        Matriz4x4& operator=(const Matriz4x4 &m);

        //Indexaciones
        float& operator()(unsigned int i, unsigned int j);

        //Sumas y restas
        Matriz4x4 operator+(const Matriz4x4 &m)const;
        Matriz4x4 operator-(const Matriz4x4 &m)const;

        Matriz4x4& operator+=(const Matriz4x4 &m);
        Matriz4x4& operator-=(const Matriz4x4 &m);

        //Producto y division por un escalar
        Matriz4x4 operator*(float f)const;
        Matriz4x4 operator/(float f)const;

        Matriz4x4& operator*=(float f);
        Matriz4x4& operator/=(float f);

        //Producto entre matrices
        Matriz4x4 operator*(const Matriz4x4 &m)const;
        Matriz4x4& operator*=(const Matriz4x4 &m);

        //Producto entre matriz y vector
        Vector operator*(Vector &v)const;

        //Comparacion
        bool operator==(const Matriz4x4 &m)const;
        bool operator!=(const Matriz4x4 &m)const;

        //Operaciones utilitarias
        Matriz4x4 transpuesta() const;

        //Operaciones estaticas
        static Matriz4x4 Nula();
        static Matriz4x4 Identidad();
        static Matriz4x4 Escalado(float x,float y,float z);
        static Matriz4x4 Escalado(Vector v);
        static Matriz4x4 Traslacion(float x,float y,float z);
        static Matriz4x4 Traslacion(Vector t);
        static Matriz4x4 RotacionX(double angulo);
        static Matriz4x4 RotacionY(double angulo);
        static Matriz4x4 RotacionZ(double angulo);
        static Matriz4x4 Rotacion(double angulo,Vector eje);
        static Matriz4x4 Rotacion(double angulo,float x,float y,float z);

};

Matriz4x4 operator*(float f,const Matriz4x4 &m);

#endif // MATRIZ4X4_H
