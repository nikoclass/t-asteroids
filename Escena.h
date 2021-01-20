#ifndef ESCENA_H
#define ESCENA_H

#include "Camara.h"
#include "Luz.h"
#include "Dibujable.h"
#include <set>
#include <map>

using namespace std;

enum TipoRenderizado{Wireframe=1,Facetado=2,Interpolado=3};

class Escena
{
    private:
        map<int,Camara*> camaras;
        Camara* camaraActual;

        set<Luz*> luces;
        set<Dibujable*> dibujables;

        Vector luzAmbiente;

        float factorAmbiente;
        float factorDifuso;
        float factorEspecular;

        bool texturas;

        TipoRenderizado tipoRenderizado;

        ///Falta agregar los los sistemas de particulas, fog, etc...
        ///............................................................

    void setearParametrosLuces(Dibujable *dib);

    public:
        Escena();
        set<Luz*> getLuces() { return luces; }
        set<Dibujable*> getDibujables() { return dibujables; }

        void addCamara(Camara *camara,int prioridad=-1){
            if(!camara) return;

            if(prioridad==-1)
                prioridad=camaras.size();

            camara->setId(prioridad);
            camara->setEscena(this);
            camaras.insert(make_pair(prioridad,camara));
        }
        void removeCamara(Camara *camara){
            if(!camara) return;
            camaras.erase(camara->getId());
            camara->setEscena(NULL);
        }
        Camara* getCamaraActual();

        void addLuz(Luz *luz){
            if(!luz) return;
            luces.insert(luz);
            luz->setEscena(this);
        }
        void removeLuz(Luz *luz){
            if(!luz) return;
            luces.erase(luz);
            luz->setEscena(NULL);
        }

        void addDibujable(Dibujable *dib){
            if(!dib) return;
            dibujables.insert(dib);
            dib->setEscena(this);
        }
        void removeDibujable(Dibujable *dib){
            if(!dib) return;
            dibujables.erase(dib);
            dib->setEscena(NULL);
        }

        void setLuzAmbiente(Vector luz);
        Vector getLuzAmbiente();

        void setTexturas(bool val);
        bool getTexturas();

        void setFactorAmbiente(float factor);
        float getFactorAmbiente();
        void setFactorDifuso(float factor);
        float getFactorDifuso();
        void setFactorEspecular(float factor);
        float getFactorEspecular();

        void setTipoRenderizado(TipoRenderizado tipo);
        TipoRenderizado getTipoRenderizado();

        void toGL();

        virtual ~Escena();
};

#endif // ESCENA_H
