#ifndef SISTEMAPARTICULAS_H
#define SISTEMAPARTICULAS_H
class SistemaParticulas;

#include "Multiobjeto3d.h"


class SistemaParticulas : public Dibujable
{
    private:
        int cantParticulas;
        Sprite **particulas;
        float tam;

    public:
        SistemaParticulas(int cant, string particula ="flare2.tga");
        virtual ~SistemaParticulas();

        Vector getParticula(int n);
        void setParticula(int n,const Vector &particula);

        int getCantParticulas();

        void setTamanio(float tamanio);
        float getTamanio();

        //Overrides
        Esfera getEsferaEnvolvente();
        bool esTransparente();
        bool getAfectaLuces();

        virtual void dibujar();
};

#endif // SISTEMAPARTICULAS_H
