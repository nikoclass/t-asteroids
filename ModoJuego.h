#ifndef MODOJUEGO_H_
#define MODOJUEGO_H_
class ModoJuego;


#include <iostream>
#include <map>
#include <vector>
//references
#include "Engine.h"
#include "Modo.h"
#include "CtrlJuego.h"
#include "ModoMenu.h"
#include "Settings.h"
#include "Nave.h"
#include "Asteroide.h"
class Asteroide;
#include "Colisionable.h"
class Colisionable;
#include "MFisica.h"

#include "ClienteModoJuego.h"


//#include "MenuPrincipal.h"

//namespace
using namespace std;

class ModoJuego : public Modo, public ServidorRed,public ObserverConexion {
private:
	static ModoJuego*instance;
	static int count;
	ModoJuego();
	Camara* camara1;
	//Camara* camara2;
	Multiobjeto3d*universo;
	Multiobjeto3d*tierra_luna;
	Objeto3d* esferaJuego;

	bool pausa;
	Vector anguloPausa;
	bool cabina;
	int camara2view;

	//ASTEROIDES
	map<Asteroide*,Asteroide*> asteroides;
    Objeto3d
        * planetas[9],
        * anillosaturno;


    Sprite* auraLuna;

    Luz* luzSol;
    Luz* luzGalaxia;

	int asteroidsCount;

//	Targeteable*mejorTarget;
//	float distMejorTarget;

    Texto2D
        * textoPausa,
        * asterCercano,
        * misiles,
        * pointer;


    Escena* escena;
    float x,y;

    Objeto3d
        * objvida,
        * objmisil,
        * objaster,
        * objvel;

    Sprite
        * objtarget,
        * target;

    Texto2D
        * textoVida,
        * textoMisil,
        * textoAster,
        * textoVel,
        * textoTarget;

    //Nave principal
    Nave
        * nave_princ,
        * m_tempShip;
    //las naves de los clientes identificadas por el numero de cliente
    HashTable *naveClientes;

    bool modoCliente;
    ClienteModoJuego *cliente;
    bool modoSingle;
    bool m_waitKilled;

    SistemaParticulas *particulasFlotantes;


    //Texto3D * m_userName;
    Texto2D * m_userName;
    bool m_started;
    bool m_isOver;
    void ini();
    void reset();
    void internalAddEscene();
    Nave * dameNueva(Nave*);

public:
	static ModoJuego* getInstance();
	virtual ~ModoJuego();
	Asteroide* crearAsteroide(int id);
	void addEscenografia();
	void ticEscenografia();
	void tic(void);

	void setPausa(bool p){pausa=p;}

//	float getDistMejorAsteroide();
//	void setDistMejorAsteroide(float);
//	Asteroide*getMejorAsteroide();
//	void setMejorAsteroide(Asteroide*);

	void addAsteroide(Asteroide*);
	bool isAsteroide(Asteroide*);
	void removeAsteroide(Asteroide*);
	Escena* getEscena();
	int getCamara2View();
	void setCamara2View(int);
	void setNavePrinc(Nave* n);
	void setUserName(string userName);
    Camara* getCamaraPrincipal();
    Nave* getNavePrinc();
    bool isModoCliente() { return modoCliente; }
    bool isModoSingle() { return modoSingle;}
    bool isStarted() { return m_started; }
    bool isOver() {return m_isOver; }
    void Enter();
    void Leave();

    void setModoCliente(bool m);
    void setCliente(ClienteModoJuego *c){cliente=c;}
    void setModoSingle(bool b) {modoSingle = b;}

    //Del servidor de red
    void recibirPaqueteServidor(PaqueteRed* paquete);
    string getNombreServidor();

    void onClientLeave(int numCliente);

};

#endif /*MODOJUEGO_H_*/
