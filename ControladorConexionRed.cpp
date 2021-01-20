#include "ControladorConexionRed.h"

#include "Logger.h"

ControladorConexionRed::ControladorConexionRed()
    : SubjectConexion()
{
    modo=Inactivo;
    in=NULL;
    out=NULL;
}

ControladorConexionRed::~ControladorConexionRed(){
    if(in) delete in;
    if(out) delete out;
}

void ControladorConexionRed::comenzarServidor(int puerto){
    if(modo!=Inactivo) return;

    this->modo=Servidor;
    this->puerto=puerto;
    this->start();
}

void ControladorConexionRed::comenzarCliente(string direccion,int puerto){
    if(modo!=Inactivo) return;

    this->modo=Cliente;
    this->direccion=direccion;
    this->puerto=puerto;
    this->start();
}

void ControladorConexionRed::run(){
    if(modo==Inactivo) return;

    ControladorRed* ctrlRed=ControladorRed::getInstancia();

    if(modo==Servidor){
        cout<<"ControladorConexionRed: Iniciando Servidor"<<endl;
        cout<<"ControladorConexionRed: Puerto: "<<puerto<<endl;

        this->in=new SocketServer(puerto,5); ///Por ahora 5 conexiones, despues ver...
        Socket *cliente;
        ControladorClienteRed* ctrlCliente;


        ///Cambiar luego por una variable 'salir'
        while(1){
            cliente=in->Accept();
            ctrlCliente=new ControladorClienteRed(ctrlRed->getUltimoNumCliente(),cliente);

            ctrlRed->suspender();
            ctrlRed->registrarControladorClienteRed(ctrlCliente);

            //Le envia el numero de cliente
            PaqueteRed *p=new PaqueteRed();
            p->addString("NumCliente");
            p->addInt(ctrlCliente->getHashCode());
            ctrlCliente->enviarPaqueteCliente(p);
            delete p;

            //le envia el snapshot principal
            ctrlRed->enviarSnapshot(ctrlCliente);

            Sleep(100); ///investigar que pasa si se queda este sleep!!

            cout<<"Cliente Nro "<<ctrlCliente->getHashCode()<<" conectado"<<endl;
            ctrlRed->reanudar();
        }

    }else{ //Cliente

        ProcesadorPaquetes *procesador_paquetes=new ProcesadorPaquetes();

        cout<<"ControladorConexionRed: Iniciando Cliente"<<endl;
        cout<<"ControladorConexionRed: Host: "<<direccion<<", Puerto: "<<puerto<<endl;

        try{
            this->out=new SocketClient(direccion,puerto);
            cout<<"ControladorConexionRed: Socket creado"<<endl;
            notifyOK("ControladorConexionRed: Notificacion Socket creado");
        }catch(SocketException e){
            cout<<"ControladorConexionRed: Error: "<<e.what()<<endl;
            modo=Inactivo;
            notifyError(e.what());
            return;
        }
        catch(const char *error){
            cout<<"ControladorConexionRed: Error: "<<error<<endl;
            modo=Inactivo;
            notifyError(error);
            return;
        }catch(std::string error){
            cout<<"ControladorConexionRed: Error: "<<error<<endl;
            modo=Inactivo;
            notifyError(error);
            return;
        }catch (...) {
            cout << "ControladorConexionRed: Error creando socket";
            modo=Inactivo;
            return;
        }

        int cant,hashServidor;
        string tipo;
        char buf[1024*256]; //256 kb de buffer
        int len;
        PaqueteRed *paquete,*paquetito;

        procesador_paquetes->start();

        //Le envia la peticion de snapshot
        /*PaqueteRed *p=new PaqueteRed();
        p->addString("PeticionSnapshot");
        enviarPaqueteAServidor(p);
        delete p;
        */

        ///Cambiar luego por una variable 'salir'
        while(1){

            //recibo y armo el paquete
            out->RecibirBytes(buf,len);
            if(len<=0){
                //modo=Inactivo;
                cout<<"ERROR: Servidor desconectado\n";
                LOG(LogLevel::Error,"Servidor desconectado");
                exit(1);
            }

            paquete=new PaqueteRed(buf,len);

            if(paquete->chequearConsistencia()==false){
                LOG_MORE(LogLevel::Error,"paquete no consistente [buf=%s] [len=%d]",buf,len);
                continue;
            }

            //lo proceso
            tipo=paquete->getString();

            if(tipo=="Salir"){
                ///Se desconecta el servidor, ver que hacer...
                //salir=true;
                cout<<"ControladorConexionRed: Servidor desconectado"<<endl;
                procesador_paquetes->procesarPaquete(NULL); //con esto fuerzo a que termine de procesar lo que le queda y se vaya
                delete paquete;

            }else if (tipo=="Paquetitos"){
                procesador_paquetes->procesarPaquete(paquete);
            }else if (tipo=="NumCliente"){
                ctrlRed->setNumCliente(paquete->getInt());
                delete paquete;
            }else
                LOG_MORE(LogLevel::Error,"Tipo no esperado [buf=%s] [len=%d]",buf,len);

        }

        procesador_paquetes->waitForEnd();
        delete procesador_paquetes;

    }

}

void ControladorConexionRed::enviarPaqueteAServidor(PaqueteRed* paquete){
    /// Quizas esto se deba hacer de manera asincronica...

    if(modo!=Cliente) return;

    //cout<<"ControladorConexionRed: Enviando paquete hacia el servidor"<<endl;

    out->SendBytes(paquete->getDatos(),paquete->getTamanio());
}

void ControladorConexionRed::matarServidor(){
    ///Ver como hacer esto...

    if(modo!=Servidor) return;

    ///Matar primero a los clientes que estan conectados!!



    ///------------------------


    in->Close();
    delete in;
    in=NULL;
    modo=Inactivo;

}

void ControladorConexionRed::matarCliente(){
    ///Ver como hacer esto...

    if(modo!=Cliente) return;

    ///Avisar al servidor que me voy



    ///------------------------------------

    out->Close();
    delete out;
    out=NULL;
    modo=Inactivo;
}


ProcesadorPaquetes::ProcesadorPaquetes(){
    productor_consumidor=new ProductorConsumidor();
}

ProcesadorPaquetes::~ProcesadorPaquetes(){
    delete productor_consumidor;
}

void ProcesadorPaquetes::run(){

    int cant,hashServidor,tipo;
    string s;
    PaqueteRed *paquete,*paquetito;

    ControladorRed *ctrlRed=ControladorRed::getInstancia();

    // Este bucle ejecuta mientras no se reciba un PaqueteRed NULL
    // o el Producto consumido no sea de tipo PaqueteRed

    while(paquete=dynamic_cast<PaqueteRed*>(productor_consumidor->consumir())){

        ctrlRed->suspender(); //entra a la seccion critica

        paquete->reiniciarLectura();
        paquete->getString(); //Paquetitos

        cant=paquete->getInt();

        for(int i=0;i<cant;i++){
            paquetito=paquete->getPaquete();

            tipo=paquetito->getInt();

            if(tipo==PAQUETE_USUARIO){
                //cout<<"ProcesadorPaquetes: Recibiendo "<<cant<<" paquetitos del servidor"<<endl;

                hashServidor=paquetito->getInt();
                ctrlRed->recibirPaquetesDeServidor(hashServidor,paquetito->getPaquete());
                delete paquetito;


            }else if(tipo==PAQUETE_SNAPSHOT){
                paquetito->reiniciarLectura();
                ctrlRed->recibirSnapshot(paquetito);
            }

        }

        delete paquete;

        ctrlRed->reanudar(); //sale de la seccion critica
    }

}

//Ingresa al objeto productor_consumidor el paquete dado para que el thread lo atienda cuando pueda
void ProcesadorPaquetes::procesarPaquete(PaqueteRed *paquete){
    productor_consumidor->producir(paquete);
}

