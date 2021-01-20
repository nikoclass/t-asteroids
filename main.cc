#include <iostream>
#include <stdexcept>
//#include <unistd.h>

//references
#include "Engine.h"
#include "CtrlJuego.h"
#include "Logger.h"

#include "ServidorChat.h"
#include "ClienteChat.h"

using namespace std;


void hacerAscii(){
    FILE *f=fopen("ascii.txt","w");

    for(int i=0;i<256;i++)
        fprintf(f,"%c->%d\n",i,i);

    fclose(f);
}

void testearPaquetes(){

    PaqueteRed* p1=new PaqueteRed(),*p2;


    p1->addString("Pelao");
    p1->addInt(33);
    p1->addFloat(3.1415926535);
    p1->addVector(Vector(1,2,3));

    p2=new PaqueteRed();
    p2->addString("Paquetito");

    p1->addPaquete(p2);
    p1->addPaquete(p2);

    p1->mostrarContenido();


    /*for(int i=0;i<200;i++){
        p1->addVector(Vector(10,11,12));
        p1->addVector(Vector(13,14,15));

        p2=new PaqueteRed();
        p2->addString("Hola pelao!!!");
        p1->addPaquete(p2);

        delete p2;
    }

    FILE *f=fopen("C:\\prueba.txt","wb");
    fwrite(p1->getDatos(),p1->getTamanio(),1,f);
    fclose(f);
    */
}


void testearRed(){
    ControladorRed *ctrlRed=ControladorRed::getInstancia();


    cout<<"Servidor (S) o Cliente (C)?: ";
    char c;
    cin>>c;
    if(c=='S'){
        ctrlRed->setModoServidor();
        ServidorChat *s=new ServidorChat;

        int n;
        do{
            cin>>n;


            PaqueteRed *p=new PaqueteRed;
            p->addInt(n);
            for(int i=0;i<n;i++)
                p->addString("Hola muñango");

            s->enviarPaqueteServidor(p);

            ctrlRed->enviarTodosLosPaquetes();



        }while(n>0);


    }else{
        ctrlRed->setModoCliente("colossus");

        string str;
        do{
            cin>>str;

            PaqueteRed *p=new PaqueteRed;
            p->addString(str);
            ClienteChat::instancia->enviarPaqueteCliente(p);

            ctrlRed->enviarTodosLosPaquetes();
        }while(str!="");


    }
}


int main(int argc, char *argv[]){

    LOG_INIT(LogLevel::Debug, "log_asteroids", false);

	//hacerAscii();
	//testearPaquetes();
	//testearRed();

	try {
		CtrlJuego*juego=CtrlJuego::getInstance();
		juego->inicializar(argc,argv);
	}
	catch(exception&e){
		cout << "[ERROR] - " << e.what() << endl;
	}

    return 0;
}
