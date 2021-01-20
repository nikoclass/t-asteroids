#ifndef TARGETEABLE_H
#define TARGETEABLE_H
#include "Vector.h"

class Targeteable
{
    public:
        Targeteable(){};
        virtual ~Targeteable(){};
        virtual Vector getPosicionTarget(){return Vector();}
        virtual void setPosicionTarget(Vector)=0;
        //virtual float getDanioTarget()=0;
        virtual void setVidaTarget(float)=0;
        virtual float getVidaTarget()=0;
};

#endif // TARGETEABLE_H
