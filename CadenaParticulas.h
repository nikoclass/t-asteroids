#ifndef CADENAPARTICULAS_H
#define CADENAPARTICULAS_H
class CadenaParticulas;

#include "SistemaParticulas.h"
#include "Animable.h"

class CadenaParticulas : public Animable {
    public:
        CadenaParticulas(int cantParticulas);
        virtual ~CadenaParticulas();

        void setCantParticulas(int cant);
        int getCantParticulas();

        void setPosInicial(const Vector &pos);
        Vector getPosInicial();

        void setElasticidad(float e);
        float getElasticidad();

        void setDistanciaEntreParticulas(float dist);
        float getDistanciaEntreParticulas();

        virtual void animar(float);

    private:
        SistemaParticulas *sp;
        float elasticidad;
        float distanciaEntreParticulas;
        Vector *velocidades;
};

#endif // CADENAPARTICULAS_H
