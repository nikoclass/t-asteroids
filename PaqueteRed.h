#ifndef PAQUETERED_H
#define PAQUETERED_H
class PaqueteRed;

#include "Vector.h"
#include "Producto.h"
#include <iostream>
#include <vector>


#define PAQUETE_ACTUALIZACION 1
#define PAQUETE_INFO_TOTAL 2

#define PAQUETE_SNAPSHOT 1
#define PAQUETE_USUARIO 2

using namespace std;

enum TiposDatosPaquete{tipo_invalido=0,tipo_int,tipo_float,tipo_vector,tipo_string,tipo_paquete};

//Clase utilizada para serializar los datatypes en memoria como son:
//int, float, Vector, string. Tambien se puede armar una jerarquia de paquetes incluyendo
//uno dentro del otro
//Se utiliza para el envio de bytes como stream de datos para poder leerlos y procesarlos
//de manera natural. Es posible tambien guardarlo en un archivo y levantarlo luego.
//Esto abre la posibilidad de guardar demos de jugadas tambien...
//No se pueden leer tipos de datos si no es en el orden en que fueron guardados,
//esto es para evitar posibles errores en la interpretacion del stream de bytes
class PaqueteRed: public Producto{
    private:
        char *datos;
        int tamanio;
        int posicion;
        int reservado;

        void verificarReservado(int tamanio_agregado);
        char* tipo2string(TiposDatosPaquete tipo);
        bool tipoBien(TiposDatosPaquete tipoEsperado);

        void mostrarContenido(int nivel);

    public:
        PaqueteRed(const char *datos=NULL,int tamanio=0);

        char* getDatos();
        int getTamanio();

        void reiniciarLectura();

        void addInt(int i);
        void addFloat(float f);
        void addVector(Vector v);
        void addString(string s);
        void addPaquete(PaqueteRed* p);

        int getInt();
        float getFloat();
        Vector getVector();
        string getString();
        PaqueteRed* getPaquete();

        TiposDatosPaquete getTipoProximoDato();

        bool eof();

        void mostrarContenido();

        bool chequearConsistencia();

        virtual ~PaqueteRed();

};

#endif // PAQUETERED_H
