#ifndef SETTINGS_H_
#define SETTINGS_H_
#include <iostream>
#include <string>

//references
#include "SoundManager.h"
#include "Vector.h"
#include "Engine.h"
#include "ModoJuego.h"
#include "Nave.h"
class Nave;

// definicion de teclas
#define CANTIDAD_TECLAS 6 // CAMBIAR SI SE AGREGAN MAS TECLAS

/*
#define TECLA_UP 0
#define TECLA_DOWN 1
#define TECLA_LEFT 2
#define TECLA_RIGHT 3
#define TECLA_UP_LEFT 4
#define TECLA_UP_RIGHT 5
*/
/// Las teclas configurables no distinguiran mayusculas ni minusculas :)
/// manejarlas desde aca, no desde el teclas

typedef enum
{
    TECLA_UP = 0,
    TECLA_DOWN,
    TECLA_LEFT,
    TECLA_RIGHT,
    TECLA_UP_LEFT,
    TECLA_UP_RIGHT,
}TIPO_TECLA;


//namespace
using namespace std;
//Preferencias del juego
class Settings{
private:
	Settings();
	static Settings*instance;

	float
        velocidad,
        radioEspacio;
	int
        radioNave,
        cantAsteroides,
        radioAsteroide,
        m_volumenMusica,
        m_volumenJuego;

	string
        m_userName,
        modAsteroide,
        texturaAsteroide;

	bool texturasEnable;

	Vector posNave;
	Nave * m_nave;

	// TECLAS CONFIGURABLES
	char m_teclas[6];// = {'w', 's', 'a', 'd', 'q', 'e'};
	//double v[6] = {1., 2., 3., 3., 2., 1.};

public:
	static Settings*getInstance();
	virtual ~Settings();
	float getRadioEspacio();
	void setRadioEspacio(float radio);
	//NAVE
	Vector getPosNave();
	string getModNave();
	string getTextureNave();
	int getRadioNave();
	//ASTEROIDES
	int getCantAsteroides();
	void setCantAsteroides(int);
	int getRadioAsteroide();
	string getModeloAsteroide();
	void setModeloAsteroide(string modelo);
	string getTexturaAsteroide();
	void setTexturaAsteroide(string textura);
	//TEXTURAS
	bool isTexturasEnable();
	float getVelocidad();
	void setVelocidad(float);

    void setFactorAmbiente(float factor);
    void setFactorDifuso(float factor);
    void setFactorEspecular(float factor);
    void setTipoRenderizado(TipoRenderizado tipo);
    void setTextura(bool val);

    float getFactorAmbiente();
    float getFactorDifuso();
    float getFactorEspecular();
    TipoRenderizado getTipoRenderizado();
    bool getTextura();

    Nave * getNave();
    void setNave(Nave* nave);

    char getTeclaConfigurable(TIPO_TECLA tipoTecla) { return m_teclas[(int)tipoTecla]; }
    void setTeclaConfigurable(TIPO_TECLA tipoTecla, char nuevaLetra);
    bool isPressed(TIPO_TECLA tipoTecla);
    void anularPressed(TIPO_TECLA tipoTecla);

    void setUserName(const string &);
    string getUserName() { return m_userName; }

    int getVolumenMusica() { return m_volumenMusica; }
    void setVolumenMusica(int);

    int getVolumenJuego() { return m_volumenJuego; }
    void setVolumenJuego(int);

};

#endif /*SETTINGS_H_*/
