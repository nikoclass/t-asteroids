#include "ProductorConsumidor.h"

ProductorConsumidor::ProductorConsumidor(){
    bufferVacio=new Evento();
    mutex=new SeccionCritica();
    esperando=false;
}
ProductorConsumidor::~ProductorConsumidor(){
    delete bufferVacio;
    delete mutex;
}

void ProductorConsumidor::producir(Producto *prod){
    bool vacio;

    mutex->entrar();
        buffer.push(prod);

        if(esperando){
            bufferVacio->signal();
            esperando=false;
        }

    mutex->salir();

}

Producto* ProductorConsumidor::consumir(){
    Producto *res;

    mutex->entrar();

        if(buffer.empty()){ //si el buffer esta vacio me quedo esperando a que llegue algo
            esperando=true;
            mutex->salir();
            bufferVacio->wait();
            mutex->entrar();
        }

        res=buffer.front();
        buffer.pop();

    mutex->salir();
    return res;
}
