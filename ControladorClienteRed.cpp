#include "ControladorClienteRed.h"

ControladorClienteRed::ControladorClienteRed(int numCliente,Socket *socket){
    this->activo=true;
    this->numCliente=numCliente;
    this->cliente=socket;
    this->mutex=new SeccionCritica();
}

ControladorClienteRed::~ControladorClienteRed(){
    ///Hacer que se detenga el thread de atencion!!

    delete cliente;
}

int ControladorClienteRed::getHashCode(){
    return numCliente;
}

bool ControladorClienteRed::isActivo(){
    return activo;
}

//Thread que atiende los paquetes que recibe de cada cliente
void ControladorClienteRed::run(){

    ControladorRed *ctrlRed=ControladorRed::getInstancia();
    PaqueteRed *paquete,*paquetito;
    char buf[1024*256]; //256 kb de buffer
    string s;
    int len;
    bool salir=false;

    int cant,hashServidor;

    while(!salir){

        //recibo el paquete del cliente
        cliente->RecibirBytes(buf,len);
        if(len<=0){
            break; //sale del while
        }

        paquete=new PaqueteRed(buf,len);


        //lo proceso
        s=paquete->getString();

        if(s=="Salir"){
            salir=true;
            cout<<"ControladorClienteRed: Cliente nro "<<numCliente<<" desconectado"<<endl;

            ///Notificar al observer que se desconecto el cliente!!


        }else if(s=="Paquetitos"){
            cant=paquete->getInt();

            //if(cant>0)
            //    cout<<"ControladorClienteRed: Recibiendo "<<cant<<" paquetitos del cliente nro "<<numCliente<<endl;

            ctrlRed->suspender(); //entro a la seccion critica

            for(int i=0;i<cant;i++){
                paquetito=paquete->getPaquete();
                hashServidor=paquetito->getInt();
                ctrlRed->recibirPaquetesDeCliente(hashServidor,paquetito->getPaquete());
                delete paquetito;
            }

            ctrlRed->reanudar(); //salgo de la seccion critica

        }else if(s=="PeticionSnapshot"){
cout<<"ControladorClienteRed: Recibiendo Peticion Snapshot"<<endl;
            ctrlRed->enviarSnapshot(this);
        }

        delete paquete;
    }

    activo=false;

}

void ControladorClienteRed::enviarPaqueteCliente(PaqueteRed* paquete){
    /// Quizas esto se deba hacer de manera asincronica para no atrasar
    /// a los otros clientes en caso de que falle uno, ni al propio servidor
    /// pero por ahora queda asi para probar

    //cout<<"ControladorClienteRed: Enviando paquete hacia cliente nro "<<numCliente<<endl;
    mutex->entrar();

    cliente->SendBytes(paquete->getDatos(),paquete->getTamanio());

    mutex->salir();
}

void ControladorClienteRed::mutexEntrar(){
    mutex->entrar();
}
void ControladorClienteRed::mutexSalir(){
    mutex->salir();
}

