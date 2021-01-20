#ifndef MALLA_H
#define MALLA_H

#include "GL.h"

#include <stdio.h>
#include <string>
#include <iostream>

#include "Esfera.h"
#include "Matematicas.h"
#include "Vector.h"
#include "stdio.h"

using namespace std;

struct Triangulo{
    int t[3];
    Vector normal;
};

struct ArbolVertices{
    int IndiceNuevo;
    int IndiceViejo;
    ArbolVertices *hijos[27];
};

class Malla
{
    private:
        float *vertices;
        float *normales;
        float *coordTextura;
        unsigned int *triangulos;

        int cantVertices;
        int cantTriangulos;

        string nombre;

        Esfera esferaEnvolvente;

        void cargarMalla(FILE *f,bool optimizar);
        void destruirMalla();

        int verificarSingularidad(Vector *vertices,Vector *coordText,int vertice,int &Indice,ArbolVertices* &A,double tolerancia);
        void pasarArbolAArrayYDestruir(ArbolVertices* &A,Vector *viejos,Vector *nuevos,Vector *viejasCoordText,Vector *nuevasCoordText);
        void optimizarVertices(float tolerancia,int &cantVertices,int cantTriangulos,Vector* &Vertices,Vector* &CoordText,Triangulo* Triangulos);
        void calcularNormales(int cantVertices,int cantTriangulos,Vector *Vertices,Triangulo *Triangulos,Vector *Normales);

    public:
        Malla(FILE *f,bool optimizarVertices);
        virtual ~Malla();

        string getNombre();

        Esfera getEsferaEnvolvente();

        void toGL();

        static int saltearLinea(FILE *f,int veces=1);
        static int moverseHasta(FILE* f,const char* c);
        static int moverseHasta(FILE* f,const char c);

};

//Utilizadas para cargar la malla
int MoverseHasta(FILE* f,const char c);
int MoverseHasta(FILE* f,const char* c);
int SaltearLinea(FILE *f,int veces=1);


#endif // MALLA_H
