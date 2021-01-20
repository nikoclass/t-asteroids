#include "MenuJuego.h"
#include "MenuRed.h"

MenuJuego* MenuJuego::s_instance = NULL;

MenuJuego* MenuJuego::getInstance()
{
    if (!s_instance)
        s_instance = new MenuJuego();
    return s_instance;
}

MenuJuego::MenuJuego()
        : Menu(MainMenu3D::getInstance())
{
    Init(true);

    m_escena->addLuz(new Luz());

    m_posXIniciales = new float[3];  //m_items.size()
    m_deltaXAlineando = new float[3];

    m_posXIniciales[0] = 7;
    m_posXIniciales[1] = 8;
    m_posXIniciales[2] = 4.5;
    m_deltaXAlineando[0] = 5.5;
    m_deltaXAlineando[1] = 6.5;
    m_deltaXAlineando[2] = 2;

    float y = 2.1;
    float dy = 0.5;

    for (short i = m_items.size() - 1; i >= 0; i--)
    {
        Dibujable * d = m_items[i]->getDibujable();
        d->setOpacidad(0.7);
        d->posicionar(Vector(m_posXIniciales[i],dy,1.0));
        d->setAfectaLuces(true);
        d->rotar(PI, Vector(0,1,0));
        d->setAfectaZbuffer(true);
        d->setAfectaVista(true);
        d->setColor(Vector(255,255,2e55));
        m_escena->addDibujable(d);
        dy += y;
    }

    //m_items[2]->getDibujable()->posicionar(Vector(5,m_items[2]->getDibujable()->getPosicion()[1],1.0));
    //m_items[1]->getDibujable()->posicionar(Vector(7.5,m_items[1]->getDibujable()->getPosicion()[1],1.0));

    //m_efectoRotEsc->setCurrent(dynamic_cast<Texto3D*>(m_items[0]->getDibujable()), m_deltaXAlineando[0]);

}

MenuJuego::~MenuJuego()
{
    //dtor
}

void MenuJuego::iniciarItems()
{
    addTextItem("Iniciar Batalla");
    addTextItem("Unirse a Batalla");
    addTextItem("Practica");
}


Menu* MenuJuego::onEnterPressed()
{
    Menu * ret = MainMenu3D::getInstance();

    switch (m_selectedIndex)
    {
    case 0: // Iniciar Juego en red
        MenuRed::getInstance()->setModo(ModoServidor);
        ModoJuego::getInstance()->setModoSingle(false);
        ret = MenuRed::getInstance();
        break;
    case 1:  // Unirse a Juego en red
        MenuRed::getInstance()->setModo(ModoCliente);
        ModoJuego::getInstance()->setModoSingle(false);
        ret = MenuRed::getInstance();
        break;
    case 2:  // practica
        ModoJuego::getInstance()->setModoSingle(true);
        CtrlJuego::getInstance()->setModo(ModoJuego::getInstance());
        break;
    }
    return ret;
}
