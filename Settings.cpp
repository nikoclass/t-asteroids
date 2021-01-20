#include "Settings.h"
#include <windows.h>

Settings* Settings::instance=NULL;

Settings* Settings::getInstance(){
	if(!Settings::instance)Settings::instance=new Settings;
	return Settings::instance;
}

Settings::Settings(){
	this->radioEspacio=500;
	//Nave
	this->posNave=Vector(0,0,0);

	this->radioNave=100;
	//Asteroides
	this->cantAsteroides=10;
	this->radioAsteroide=100;
	//texturas
	this->texturasEnable=true;
	this->velocidad=1;

	m_nave = NULL;

	m_teclas[0] = 'w';
	m_teclas[1] = 's';
	m_teclas[2] = 'a';
	m_teclas[3] = 'd';
	m_teclas[4] = 'q';
	m_teclas[5] = 'e';


    char acUserName[100];
    DWORD nUserName = sizeof(acUserName);
    if (GetUserName(acUserName, &nUserName))
    m_userName = acUserName;

    m_volumenMusica = 30;
    m_volumenJuego = 90;
}

Settings::~Settings()
{
}
Vector Settings::getPosNave(){
	return this->posNave;
}

int Settings::getCantAsteroides(){
	return this->cantAsteroides;
}
void Settings::setCantAsteroides(int a){
	this->cantAsteroides=a;
}
bool Settings::isTexturasEnable(){
	return this->texturasEnable;
}
string Settings::getTexturaAsteroide(){
	return this->texturaAsteroide;
}
void Settings::setTexturaAsteroide(string textura){
	this->texturaAsteroide=textura;
}

string Settings::getModeloAsteroide(){
	return this->modAsteroide;
}
void Settings::setModeloAsteroide(string modelo){
	this->modAsteroide=modelo;
}

float Settings::getRadioEspacio(){
	return this->radioEspacio;
}
void Settings::setRadioEspacio(float radio){
	this->radioEspacio=radio;
}

float Settings::getVelocidad(){
	return this->velocidad;
}
void Settings::setVelocidad(float ts){
	this->velocidad=ts;
	Engine::getInstancia()->setTimeScale(ts);
}

//********************************/
void Settings::setFactorAmbiente(float factor){
    ModoJuego::getInstance()->getEscena()->setFactorAmbiente(factor);
}
void Settings::setFactorDifuso(float factor){
    ModoJuego::getInstance()->getEscena()->setFactorDifuso(factor);
}
void Settings::setFactorEspecular(float factor){
    ModoJuego::getInstance()->getEscena()->setFactorEspecular(factor);
}
float Settings::getFactorAmbiente(){
    return ModoJuego::getInstance()->getEscena()->getFactorAmbiente();
}
float Settings::getFactorDifuso(){
    return ModoJuego::getInstance()->getEscena()->getFactorDifuso();
}
float Settings::getFactorEspecular(){
    return ModoJuego::getInstance()->getEscena()->getFactorEspecular();
}
TipoRenderizado Settings::getTipoRenderizado(){
    return ModoJuego::getInstance()->getEscena()->getTipoRenderizado();
}
void Settings::setTipoRenderizado(TipoRenderizado tipo){
    ModoJuego::getInstance()->getEscena()->setTipoRenderizado(tipo);
}

bool Settings::getTextura(){
    return ModoJuego::getInstance()->getEscena()->getTexturas();
}
void Settings::setTextura(bool val){
    ModoJuego::getInstance()->getEscena()->setTexturas(val);
}

Nave * Settings::getNave()
{
    return m_nave;
}

void Settings::setNave(Nave* nave)
{
    if (m_nave && m_nave != nave){
        //m_nave->~Nave();
        delete m_nave;
    }
    m_nave = nave;
}

void Settings::setTeclaConfigurable(TIPO_TECLA tipoTecla, char nuevaLetra)
{
    m_teclas[(int)tipoTecla] = nuevaLetra;
}

bool Settings::isPressed(TIPO_TECLA tipoTecla)
{
    char mayMin = 0;
    char c = m_teclas[(int)tipoTecla];
    if ((c <= 'z') && (c >= 'a'))
        mayMin = c - ('a' - 'A');
    else
        mayMin = c + ('a' - 'A');

    // en el caso de que no sea una letra sale por la primera expresion, bendito circuito corto
    return Teclado::getInstancia()->tecla(c) || Teclado::getInstancia()->tecla(mayMin);
}

void Settings::anularPressed(TIPO_TECLA tipoTecla)
{
    char c = m_teclas[(int)tipoTecla];
    Teclado::getInstancia()->setTecla(c, false);
    if ((c <= 'z') && (c >= 'a'))
        Teclado::getInstancia()->setTecla(c - ('a' - 'A'), false);
    else if ((c <= 'Z') && (c >= 'A'))
        Teclado::getInstancia()->setTecla(c + ('a' - 'A'), false);
}

void Settings::setUserName(const string & str)
{
    m_userName = str;
    if (ModoJuego::getInstance()->isStarted())
    {
        ModoJuego::getInstance()->setUserName(str);
    }
}

void Settings::setVolumenMusica(int v)
{
    m_volumenMusica = v;
    SoundManager::getInstance()->setMusicVolume((float)v / 100);
}

void Settings::setVolumenJuego(int v)
{
    m_volumenJuego = v;
    SoundManager::getInstance()->setGameVolume((float)v / 100);
}
