#ifndef CONDTARGETEABLE_H
#define CONDTARGETEABLE_H

#include "Condition.h"
#include "Nave.h"


class CondTargeteable : public Condition
{
    public:
        CondTargeteable(Nave* n);
        virtual ~CondTargeteable();
        Nave* Getnave() { return nave; }
        void Setnave(Nave* val) { nave = val; }
        Targeteable* getTarget(){return this->target;}
        bool cumple(Condicionable*);
    private:
        Nave* nave;
        Targeteable* target;
        float distTarget;
};

#endif // CONDTARGETEABLE_H
