#include "ControladorRed.h"

#include "PrototiposClientes.h"
#include "Logger.h"

ControladorRed* ControladorRed::instancia=NULL;

ControladorRed* ControladorRed::getInstancia(){
    if(instancia==NULL){
        instancia=new ControladorRed();
        registrarPrototipos(); //si esto lo meto en el contructor da tremendo stack overflow
    }

    return instancia;
}


ControladorRed::ControladorRed(){
    modoControlador=Inactivo;

    ctrlConexion=new ControladorConexionRed();
    ctrlConexion->addObserver(this);

    controladoresClientes=new list<ControladorClienteRed*>();
    objetosServidores=new HashTable();
    objetosCliente=new HashTable();
    prototiposCliente=new map<string,ClienteRed*>();

    paquetesPendientes=new list<PaqueteRed*>();
    snapshotsPendientes=new list<TipoSnapshot*>();

    ultimoNumCliente=0;

    mutex=new SeccionCritica();
    //mutexSuspender=new SeccionCritica();
}

ControladorRed::~ControladorRed(){
    //No hace nada porque nunca se destruye un singleton
}


void ControladorRed::setModoServidor(int puertoEscucha){
    if(modoControlador!=Inactivo) return;

    this->puertoEscucha=puertoEscucha;
    this->modoControlador=Servidor;
    this->ctrlConexion->comenzarServidor(puertoEscucha);
}

void ControladorRed::setModoCliente(string servidor,int puerto){
    if(modoControlador!=Inactivo) return;

    this->direccionServidor=servidor;
    this->puertoEscucha=puerto;
    this->modoControlador=Cliente;
    this->ctrlConexion->comenzarCliente(servidor,puerto);
}

void ControladorRed::setModoInactivo(){
    if(modoControlador==Servidor){
        ctrlConexion->matarServidor();
    }else if(modoControlador==Cliente){
        ctrlConexion->matarCliente();
    }

    modoControlador=Inactivo;
}

ModoControlador ControladorRed::getModoControlador(){
    return modoControlador;
}

void ControladorRed::registrarServidor(ServidorRed *servidor){
    ///Mutuexcluir con el ingreso de paquetes
    mutex->entrar();

    //Notifica a los clientes del servidor nuevo
    //this->snapshotsPendientes->push_back(new TipoSnapshot(servidor,true));
    this->objetosServidores->add(servidor);

    if(modoControlador!=Servidor){
        mutex->salir();
        return;
    }

    PaqueteRed *p=new PaqueteRed();
    p->addInt(PAQUETE_SNAPSHOT);
    p->addInt(1); //es un alta
    //p->addString(servidor->getNombreServidor()); esto causa un error de tiempo de ejecucion "pure virtual method called"
    p->addInt(servidor->getHashCode());
    this->paquetesPendientes->push_back(p);

    mutex->salir();

}
void ControladorRed::desregistrarServidor(ServidorRed *servidor){
    ///Mutuexcluir con el ingreso de paquetes
    mutex->entrar();
    //Notifica a los clientes de la muerte del servidor
    //cout<<"ControladorRed: Objeto Servidor desregistrado"<<endl;
    //this->snapshotsPendientes->push_back(new TipoSnapshot(servidor,false));
    this->objetosServidores->remove(servidor);

    if(modoControlador!=Servidor){
        mutex->salir();
        return;
    }

    PaqueteRed *p=new PaqueteRed();
    p->addInt(PAQUETE_SNAPSHOT);
    p->addInt(0); //es una baja
    p->addInt(servidor->getHashCode());
    this->paquetesPendientes->push_back(p);

    mutex->salir();
}

void ControladorRed::registrarPrototipo(ClienteRed *cliente){
    this->prototiposCliente->insert(make_pair(cliente->getServidorAsociado(),cliente));
}

void ControladorRed::desregistrarCliente(ClienteRed *cliente){
    this->objetosCliente->remove(cliente);
}

void ControladorRed::registrarControladorClienteRed(ControladorClienteRed* ctrlCliente){
    ///Pensar si se debe mutuexcluir

    ctrlCliente->start();
    this->controladoresClientes->push_back(ctrlCliente);
}


void ControladorRed::enviarPaqueteServidor(ServidorRed *servidor, PaqueteRed *paquete){
    mutex->entrar();

    //Lo deja en paquetes pendientes
    if(this->modoControlador==Servidor){
        PaqueteRed* cabezal=new PaqueteRed();
        cabezal->addInt(PAQUETE_USUARIO); //Su id
        cabezal->addInt(servidor->getHashCode()); //Su id
        cabezal->addPaquete(paquete);
        this->paquetesPendientes->push_back(cabezal);
    }
    delete paquete;

    mutex->salir();
}
void ControladorRed::enviarPaqueteCliente(ClienteRed *cliente, PaqueteRed *paquete){

    //Lo deja en paquetes pendientes
    if(this->modoControlador==Cliente){
        PaqueteRed* cabezal=new PaqueteRed();
        cabezal->addInt(cliente->getHashServidorAsociado());
        cabezal->addPaquete(paquete);
        this->paquetesPendientes->push_back(cabezal);
    }
    delete paquete;
}

void ControladorRed::enviarTodosLosPaquetes(){
    mutex->entrar();
    //envia y borra los paquetes pendiente
//    cout<<"ControladorRed: enviando todos los paquetes"<<endl;

    PaqueteRed* superPaquete=new PaqueteRed();
    PaqueteRed* paquete,*paqueteAux;
    ServidorRed *servidor;
    int hash;

    superPaquete->addString("Paquetitos");


    int tam=this->paquetesPendientes->size();
    int tamSP=this->snapshotsPendientes->size();

    superPaquete->addInt(tam);

    list<PaqueteRed*>::iterator iter;
    for(iter=paquetesPendientes->begin();iter!=paquetesPendientes->end();iter++){
        paquete=*iter;

        if(modoControlador==Servidor && paquete->getInt()==PAQUETE_SNAPSHOT){ //Es un snapshot
            if(paquete->getInt()==1){ //y es de alta
                //tengo que obtener el nombre de la clase
                //no lo puedo hacer antes por el tema de pure virtual method called
                hash=paquete->getInt();
                servidor=dynamic_cast<ServidorRed*>(this->objetosServidores->getValue(hash));

                paqueteAux=new PaqueteRed();
                paqueteAux->addInt(PAQUETE_SNAPSHOT);
                paqueteAux->addInt(1);
                paqueteAux->addInt(hash);

                if(servidor) //Esto es para los casos en que un servidor se instancia y se destruye en el mismo frame
                    paqueteAux->addString(servidor->getNombreServidor());
                else
                    paqueteAux->addString("");

                delete paquete;
                paquete=paqueteAux;
            }
        }

        paquete->reiniciarLectura();

        superPaquete->addPaquete(paquete);
        delete paquete;
    }

    paquetesPendientes->clear();

    if(modoControlador==Servidor){
        //tengo armado el superpaquete, ahora los mando a los clientes conectados
        //si su thread no esta corriendo, es porque el cliente se desconecto asi que lo saco
        list<ControladorClienteRed*> clientesABorrar;

        list<ControladorClienteRed*>::iterator iterCliente;
        for(iterCliente=controladoresClientes->begin();iterCliente!=controladoresClientes->end();iterCliente++){

            if(!(*iterCliente)->isActivo()){
                clientesABorrar.push_back(*iterCliente);
            }else{
                if(tam>0)
                    (*iterCliente)->enviarPaqueteCliente(superPaquete);
            }
        }

        //this->snapshotsPendientes->clear();

        //Ahora los borro
        for(iterCliente=clientesABorrar.begin();iterCliente!=clientesABorrar.end();iterCliente++){
            cout<<"ControladorRed: Eliminando Cliente nro: "<<(*iterCliente)->getHashCode()<<endl;
            this->notifyClienteLeave((*iterCliente)->getHashCode());
            controladoresClientes->remove(*iterCliente);
            delete *iterCliente;
        }

    }else{ //Modo Cliente
        ctrlConexion->enviarPaqueteAServidor(superPaquete);
    }

    delete superPaquete;

    mutex->salir();
}

//Ambas funciones reciben el hash del objeto servidor, no es un error
void ControladorRed::recibirPaquetesDeCliente(int hashServidor,PaqueteRed* paquete){
    ///Pensar si conviene meter la mutuexclusion aqui...

    ServidorRed *servidor=dynamic_cast<ServidorRed*>(this->objetosServidores->getValue(hashServidor));

    if(servidor){
        //cout<<"ControladorRed: Objeto Servidor de clase "<<servidor->getNombreServidor()<<" recibiendo paquete"<<endl;
        servidor->recibirPaqueteServidor(paquete);
    }else{
        cout<<"ControladorRed: ERROR: No existe servidor de red asociado a tal cliente"<<endl;
    }
    delete paquete;
}

void ControladorRed::recibirPaquetesDeServidor(int hashServidor,PaqueteRed* paquete){
    ClienteRed *cliente=dynamic_cast<ClienteRed*>(this->objetosCliente->getValue(hashServidor));

    if(cliente){
        //cout<<"ControladorRed: Objeto Cliente de clase "<<typeid(*cliente).name()<<" recibiendo paquete"<<endl;
       cliente->recibirPaqueteCliente(paquete);
    }else{
        cout<<"ControladorRed: ERROR: No existe cliente de red asociado a tal servidor"<<endl;

        // Pido que me de el snapshot!
        PaqueteRed *p=new PaqueteRed();
        p->addString("PeticionSnapshot");
        ctrlConexion->enviarPaqueteAServidor(p);
        delete p;
cout<<"ControladorRed: Enviando Peticion de Snapshot"<<endl;

    }

    delete paquete;
}

int ControladorRed::getUltimoNumCliente(){
    ultimoNumCliente++;
    return ultimoNumCliente;
}

void ControladorRed::enviarSnapshot(ControladorClienteRed* ctrlCliente){
    mutex->entrar();

    PaqueteRed *superpaquete=new PaqueteRed(),*paquetito;

    cout<<"ControladorRed: Enviando Snapshot a cliente nro "<<ctrlCliente->getHashCode()<<endl;

    superpaquete->addString("Paquetitos");

    list<Hasheable*> servidores=this->objetosServidores->obtenerElementos();
    int tam=servidores.size();

    superpaquete->addInt(tam);

    list<Hasheable*>::iterator iter;

    ServidorRed *servidor;

    for(iter=servidores.begin();iter!=servidores.end();iter++){
        paquetito=new PaqueteRed();

        servidor=dynamic_cast<ServidorRed*>(*iter);
        if(!servidor){
            cout<<"ControladorRed: ERROR!! Objeto Servidor NULL al armar snapshot, esto no deberia pasar!!"<<endl;
        }else{
            paquetito->addInt(PAQUETE_SNAPSHOT);
            paquetito->addInt(1); //digo que es un alta
            paquetito->addInt(servidor->getHashCode()); //le doy su id
            paquetito->addString(servidor->getNombreServidor()); //le doy el nombre de la clase

            superpaquete->addPaquete(paquetito);
            delete paquetito;

        }
    }

    ctrlCliente->enviarPaqueteCliente(superpaquete);
    delete superpaquete;

    mutex->salir();
}

///No va mas!!
/*
void ControladorRed::enviarSnapshotPendiente(ControladorClienteRed* ctrlCliente){
    PaqueteRed *superpaquete=new PaqueteRed(),*paquetito;

    superpaquete->addString("Snapshot");

    list<TipoSnapshot*> *servidores=this->snapshotsPendientes;
    int tam=servidores->size();

    if(tam==0){ //no hay nada que enviar
        delete superpaquete;
        return;
    }

    //cout<<"ControladorRed: Enviando Snapshot Pendiente a cliente nro "<<ctrlCliente->getHashCode()<<endl;

    superpaquete->addInt(tam);

    list<TipoSnapshot*>::iterator iter;
    TipoSnapshot* ts;

    ServidorRed *servidor;

    for(iter=servidores->begin();iter!=servidores->end();iter++){

        ts=*iter;

        paquetito=new PaqueteRed();

        servidor=ts->servidor;
        if(!servidor){
            cout<<"ControladorRed: ERROR!! Objeto Servidor NULL al armar snapshot, esto no deberia pasar!!"<<endl;
        }else{

            //if(this->objetosServidores->containsKey((int)servidor)){
            if(ts->alta){
                //Es un alta
                paquetito->addInt(1);
                paquetito->addString(servidor->getNombreServidor()); //le doy el nombre de la clase
            }else{
                //Es una baja

                paquetito->addInt(0);
            }
            paquetito->addInt(ts->id); //le doy su id

            superpaquete->addPaquete(paquetito);
            delete paquetito;

            ///Esto no lo puedo hacer porque si tengo mas clientes conectados pierdo la informacion, asi que pensar como hacerlo
            ///delete ts; //no borro el iterador, borro el TipoSnapshot que apunta ese iterador
        }
    }

    ctrlCliente->enviarPaqueteCliente(superpaquete);
    delete superpaquete;
}
*/

void ControladorRed::recibirSnapshot(PaqueteRed *snapshot){
    //cout<<"ControladorRed: Recibiendo Snapshot"<<endl;

    string nombre;
    int hash,tipo;
    ClienteRed *prototipo,*cliente;


    snapshot->getInt(); //PAQUETE_SNAPSHOT
    tipo=snapshot->getInt();
    hash=snapshot->getInt(); //su id

    if(tipo==1){ //Es un alta de Servidor, hay que crear el cliente

        nombre=snapshot->getString(); //el nombre de la clase

        if(nombre!=""){
            prototipo=(*this->prototiposCliente)[nombre];

            if(!prototipo){
                cout<<"ControladorRed: ERROR recibiendo snapshot, no se encuentra el prototipo asociado al servidor "<<nombre<<endl;
            }else{

                if(!objetosCliente->containsKey(hash)){
                    cliente=prototipo->clonar();
                    cliente->setHashServidorAsociado(hash);

                    this->objetosCliente->add(hash,cliente);
                    cliente->onInit();
                }
            }
        }
    }else{ //Es una baja de servidor, hay que borrar el cliente
        cliente=dynamic_cast<ClienteRed*>(this->objetosCliente->getValue(hash));
        if(cliente){
            cliente->onDispose();
            delete this->objetosCliente->remove(hash); //libero la memoria del cliente que quito del hash
        }
    }

    delete snapshot;
}


TipoSnapshot::TipoSnapshot(ServidorRed* servidor,bool alta){
    this->servidor=servidor;
    id=servidor->getHashCode();
    this->alta=alta;
}


int ControladorRed::getNumCliente(){
    if(modoControlador==Servidor)
        return 0;
    else if(modoControlador==Cliente)
        return numCliente;
    else
        return -1;
}

void ControladorRed::setNumCliente(int num){
    numCliente=num;
}


void ControladorRed::suspender(){
    //mutexSuspender->entrar();
    mutex->entrar();
}

void ControladorRed::reanudar(){
    //mutexSuspender->salir();
    mutex->salir();
}

void ControladorRed::onError(std::string message){
    cout<<"ControladorRed recibio error"<<endl;
    notifyError(message);
    this->modoControlador = Inactivo;
}

void ControladorRed::onOK(std::string message){
    //cout<<"ControladorRed recibio un OK y esta notificando a sus observers"<<endl;
    notifyOK(message);
}

void ControladorRed::addObserver(ObserverConexion* o){
    if (o != this)
        SubjectConexion::addObserver(o);
}
