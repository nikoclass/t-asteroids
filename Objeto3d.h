#ifndef OBJETO3D_H
#define OBJETO3D_H
class Objeto3d;

#include "Dibujable.h"
#include "ManejadorModelos.h"
#include "ManejadorTexturas.h"
#include "Material.h"

enum ModoGeneracionCoordText{Ninguno=0, Enviromental=1, LinealObjeto=2, LinealVista=3};

class Objeto3d : public Dibujable
{
    private:
        Modelo *modelo;
        Textura *textura;

        ModoTextura modoTextura;
        ModoGeneracionCoordText modoGeneracionCoordTextura;

        float planoS[4];
        float planoT[4];

        bool cullFace;

        string nombreMalla;

        Material material;

    public:
        Objeto3d();
        Objeto3d(const char* mdl,bool optimizarVertices=true);
        Objeto3d(const char* mdl,const char *tex,bool optimizarVertices=true);


        virtual ~Objeto3d();

        void dibujar();

        void cargarModelo(const char *modelo,bool optimizarVertices=true);
        void cargarTextura(const char *tex);

        bool getGullFace();
        void setCullFace(bool valor);

        void setModoTextura(ModoTextura modo);
        ModoTextura getModoTextura();

        void setModoGeneracionCoordTextura(ModoGeneracionCoordText modo,Vector S=Vector(1,0,0),Vector T=Vector(0,1,0));
        ModoGeneracionCoordText getModoGeneracionCoordTextura();

        void setNombreMalla(string nombre);
        string getNombreMalla();

        void setMaterial(const Material& mat);
        Material& getMaterial();

        bool esTransparente(); //override de dibujable
        bool getAfectaLuces(); //override de dibujable
        Esfera getEsferaEnvolvente(); //override de dibujable

        Modelo*getModelo();
        Textura*getTextura();
};

#endif // OBJETO3D_H
