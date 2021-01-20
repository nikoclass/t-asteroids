#ifndef PRODUCTORCONSUMIDOR_H
#define PRODUCTORCONSUMIDOR_H
class ProductorConsumidor;

#include "SeccionCritica.h"
#include "Evento.h"
#include "Producto.h"
#include <queue>

using namespace std;

//Clase para manejar el problema del productor-consumidor de manera generica

class ProductorConsumidor{
    private:
        Evento *bufferVacio;
        SeccionCritica *mutex;
        queue<Producto*> buffer;
        bool esperando;

    public:
        ProductorConsumidor();
        virtual ~ProductorConsumidor();

        void producir(Producto *prod);
        Producto *consumir();
};

#endif // PRODUCTORCONSUMIDOR_H
