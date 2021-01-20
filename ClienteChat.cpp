#include "ClienteChat.h"

ClienteChat *ClienteChat::instancia=NULL;

ClienteChat::ClienteChat():ClienteRed("ServidorChat"){
}

ClienteChat::~ClienteChat(){
}

void ClienteChat::onInit(){
    cout<<"Conectado, hash="<<this->getHashServidorAsociado()<<endl;
    instancia=this;
}

void ClienteChat::onDispose(){
}

ClienteRed* ClienteChat::clonar(){ //Para los prototipos
    return new ClienteChat();
}

void ClienteChat::recibirPaqueteCliente(PaqueteRed* paquete){ //para override

cout<<"Verificando Integridad Paquete tam="<<paquete->getTamanio()<<endl;

    if(paquete->chequearConsistencia()==false){
        cout<<"Paquete no consistente"<<endl;
        return;
    }

    int n=paquete->getInt();

    for(int i=0;i<n;i++){
        if(paquete->getString()!="Hola muñango"){
            cout<<"Error i="<<i<<endl;
            return;
        }
    }
cout<<"Paquete correcto"<<endl;
}

