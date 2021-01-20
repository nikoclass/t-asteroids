#include "ServidorChat.h"

ServidorChat::ServidorChat(){
    cout<<"hash:"<<this->getHashCode()<<endl;
}

ServidorChat::~ServidorChat(){

}


void ServidorChat::recibirPaqueteServidor(PaqueteRed* paquete){
    cout<<"> "<<paquete->getString()<<endl;
}

string ServidorChat::getNombreServidor(){
    return "ServidorChat";
}
