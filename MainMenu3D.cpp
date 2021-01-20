#include "MainMenu3D.h"

#include "NavePichi.h"
#include "CtrlJuego.h"

// Tienen que volar
//#include "MenuConfig.h"
//#include "MenuAyuda.h"
#include "MenuCreditos.h"


#include "MenuNave.h"
#include "MenuJuego.h"
#include "MenuConfig3D.h"

#include "EfectoRotEsc.h"


MainMenu3D* MainMenu3D::s_instance = NULL;

MainMenu3D* MainMenu3D::getInstance()
{
    if (!s_instance)
        s_instance = new MainMenu3D();
    return s_instance;
}

MainMenu3D::MainMenu3D()
        : Menu()
{
    Init(true);

    m_gotOnGameText = false;
    m_gotShip = false;

    float y = 2.1;
    float dy = -6.5;
    //short len = (short) m_items.size();

    float posY = 6.5;
    float difY = 2.1;

    m_posXIniciales = new float[6];  //m_items.size()
    m_deltaXAlineando = new float[6];

    m_posXIniciales[0] = 6;
    m_posXIniciales[1] = 6.9;
    m_posXIniciales[2] = 6.5;
    m_posXIniciales[3] = 4;
    m_posXIniciales[4] = 2.65;
    m_posXIniciales[5] = 4.5;
    m_deltaXAlineando[0] = 3.7;
    m_deltaXAlineando[1] = 4;
    m_deltaXAlineando[2] = 4.2;
    m_deltaXAlineando[3] = 2.8;
    m_deltaXAlineando[4] = 1.6;
    m_deltaXAlineando[5] = 3.5;


    for (int i = 0; i < m_items.size(); i++)
    {
        posY -= difY;
        Dibujable * d = m_items[i]->getDibujable();
        d->posicionar(Vector(m_posXIniciales[i],posY,1.0));
        d->setAfectaLuces(true);
        d->rotar(PI, Vector(0,1,0));
        d->setAfectaZbuffer(true);
        d->setAfectaVista(true);
        d->setColor(Vector(255,255,255));
        m_escena->addDibujable(d);
    }

    // Corro el item "Continuar" para arriba y lo saco temporalmente de la escena
    m_items[m_items.size() - 1]->getDibujable()->trasladar(Vector(0,5*2.1 ,0));
    m_escena->removeDibujable(m_items[m_items.size() - 1]->getDibujable());
}


void MainMenu3D::iniciarItems()
{
    addTextItem("Juego Nuevo");
    addTextItem("Elegi tu Nave");
    addTextItem("Configuracion");
    addTextItem("Creditos");
    //addTextItem("Ayuda");
    addTextItem("Salir");
    addTextItem("Continuar");
    m_hiddenItemCount = 1;
}


void MainMenu3D::OnEnter()
{
    Menu::OnEnter();
    short lastSelected = m_selectedIndex;
    if (ModoJuego::getInstance()->isStarted())
    {
        if (!m_gotOnGameText){
            m_escena->removeDibujable(m_items[0]->getDibujable());
            m_escena->addDibujable(m_items[m_items.size() - 1]->getDibujable());
            m_gotOnGameText = true;
        }
        m_selectedIndex = 5;
    }
    else
    {
        if (m_gotOnGameText){
            m_escena->removeDibujable(m_items[m_items.size()-1]->getDibujable());
            m_escena->addDibujable(m_items[0]->getDibujable());
            m_gotOnGameText = false;
        }
        m_selectedIndex = 0;
    }

    //if (Settings::getInstance()->getNave() && !m_gotShip)
    // Elimino la opcion de elegir nave, para que no se rompa todo :)
    // La idea seria guardar info sobre la nave antes de ir al juego
    // Y crearla solo en ese momento
    // y no antes como esta funcionando ahora
    if (ModoJuego::getInstance()->isStarted() && !m_gotShip)
    {
        m_escena->removeDibujable(m_items[1]->getDibujable());
        Vector pos;
        for (int i = 2; i < m_items.size() - 1; i++)
        {
            pos = m_items[i]->getDibujable()->getPosicion();
            pos[Y] += 2.1;
            m_items[i]->getDibujable()->posicionar(pos);
        }
        m_gotShip = true;
    }

    if (m_selectedIndex != lastSelected){
        m_efectoRotEsc->setCurrent(dynamic_cast<Texto3D*>(m_items[m_selectedIndex]->getDibujable()), m_deltaXAlineando[m_selectedIndex]);
    }
}

MainMenu3D::~MainMenu3D()
{
}

Menu* MainMenu3D::onEnterPressed()
{
    switch (m_selectedIndex)
    {
    case 0:
        if (!Settings::getInstance()->getNave())
        {
            MenuNave::getInstance()->goToGame();
            return MenuNave::getInstance();
        }
        else
        {
            return MenuJuego::getInstance();
        }
        break;
    case 1:
        if (!ModoJuego::getInstance()->isStarted())
            return MenuNave::getInstance();
        break;
    case 2:
        return MenuConfig3D::getInstance();
        break;
    case 3:
        return MenuCreditos::getInstance();
        break;
    case 4:
        exit(0);
        break;
    case 5:
        CtrlJuego::getInstance()->setModo(ModoJuego::getInstance());
        break;
    }
    return this;
}

void MainMenu3D::checkCustomKeys()
{
    Settings * settings = Settings::getInstance();
    Teclado *t = Teclado::getInstancia();

    //los truquitos
    if (t->tecla('i'))
    {
        cout<<"i\n";
        t->setTecla('i',false);
        settings->setCantAsteroides(settings->getCantAsteroides()+1);
    }
    if (t->tecla('k'))
    {
        t->setTecla('k',false);
        settings->setCantAsteroides(settings->getCantAsteroides()-1);
    }
}


void MainMenu3D::handleIndexChanged(int last)
{
    if (ModoJuego::getInstance()->isStarted()){
        if (!m_selectedIndex)
            m_selectedIndex = last == 5 ? 2 : 5;
        else if (m_selectedIndex == 1)
            m_selectedIndex = (last < m_selectedIndex) ? 2 : 5;
    }
    else if (m_selectedIndex == 5)
        m_selectedIndex = last ? 0 : 4;
}
