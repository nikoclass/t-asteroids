#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "GL.h"

#include "Vector.h"
#include "math.h"

//Forward
class Escena;

class Material
{
    private:
        Vector diffuse;
        Vector specular;
        Vector emission;
        float shininess;

    public:
        Material();
        Material(Vector a, Vector b, float c);
        virtual ~Material();

        //setters
        void setDiffuse(Vector v);
        void setSpecular(Vector v);
        void setEmission(Vector v);
        void setShininess(float v);

        //getters
        Vector getDiffuse();
        Vector getSpecular();
        Vector getEmission();
        float getShininess();

        void toGL(Vector color,float opacidad);

};

#include "Escena.h"

#endif // MATERIAL_H_INCLUDED
