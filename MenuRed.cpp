#include "MenuRed.h"

#include "CtrlJuego.h"
#include "MainMenu3D.h"
#include "MenuJuego.h"

#include "Logger.h"

MenuRed * MenuRed::s_instance = NULL;

MenuRed* MenuRed::getInstance()
{
    if (!s_instance)
        s_instance = new MenuRed();
    return s_instance;
}


MenuRed::MenuRed()
    : Menu(MenuJuego::getInstance())
{
    Init(true);

    m_txtServer = new TextBox(4, Vector(-3, 2.2, 0) );
    m_txtServer->setOnlyStandardDNSChars(true);
    m_txtServer->setTexto("localhost");
    m_txtServer->addToEscena(m_escena);

    m_txtPort   = new TextBox(4, Vector(-4, 0.2, 0), "charset3d.mdl" );
    m_txtPort->setNumeric(true);
    m_txtPort->setMaxTextLength(5);
    m_txtPort->setTexto("2212");
    m_txtPort->addToEscena(m_escena);

    m_posXIniciales = new float[3];  //m_items.size()
    m_deltaXAlineando = new float[3];

    m_posXIniciales[0] = 9.5;
    m_posXIniciales[1] = 9.5;
    m_posXIniciales[2] = 1;
    m_deltaXAlineando[0] = 1;
    m_deltaXAlineando[1] = 1;
    m_deltaXAlineando[2] = 0.5;

    float y = 2.1;
    float dy = 2.6;
    for (int i = 0; i < m_items.size(); i++)
    {
        Dibujable * d = m_items[i]->getDibujable();
        d->setOpacidad(0.7);
        d->posicionar(Vector(m_posXIniciales[i],dy,1.0));
        d->rotar(PI, Vector(0,1,0));
        m_escena->addDibujable(d);
        dy -= y;
    }

    m_modoRed = ModoCliente;

    // Acomodo el item del OK para que quede debajo del todo
    m_items[2]->getDibujable()->posicionar(Vector(m_posXIniciales[2],-5.5,0));
    m_otraLuz = new Luz;
    m_otraLuz->setPos(Vector(0,0,-10));
    m_otraLuz->setDireccion(Vector(0,0,0));
    m_escena->addLuz(m_otraLuz);

    m_selectedIndex = 2;

    m_event = new Evento();
    m_eventText = NULL;

    ControladorRed::getInstancia()->addObserver(this);
}

MenuRed::~MenuRed()
{
}

void MenuRed::iniciarItems()
{
    addTextItem("Server");
    addTextItem("Puerto");
    addTextItem("Ok");
}

Menu* MenuRed::onTicCheckKey()
{
    m_txtServer->tic();
    m_txtPort->tic();

    Teclado * t = Teclado::getInstancia();
    if (m_onError)
    {
        if (t->tecla(13) || t->tecla(27))
        {
            t->setTecla(13, false);
            t->setTecla(27, false);
            m_escena->addDibujable(m_items[2]->getDibujable());
            m_escena->removeDibujable(m_eventText);
            m_onError = false;
        }
        return this;
    }

    Menu* ret = Menu::onTicCheckKey();
    return ret;
}

void MenuRed::handleIndexChanged(int last)
{
    if (m_modoRed == ModoServidor && !m_selectedIndex){
        if (last == 1)
            m_selectedIndex = m_items.size() - 1;
        else
            m_selectedIndex = 1;
    }
    if (m_selectedIndex == 0){
        m_txtServer->setFocus(true);
        m_txtPort->setFocus(false);
    }
    else if (m_selectedIndex == 1){
        m_txtServer->setFocus(false);
        m_txtPort->setFocus(true);
    }
    else{
        m_txtServer->setFocus(false);
        m_txtPort->setFocus(false);
    }
}
Menu* MenuRed::onEnterPressed()
{
    ModoJuego *mj=ModoJuego::getInstance();
    mj->setModoCliente(m_modoRed==ModoCliente);


    MFisica::getInstance()->setColisionar(m_modoRed!=ModoCliente);

    if (m_modoRed == ModoServidor)
    {
		if (! m_txtPort->getTexto().size()) return this;
        ControladorRed::getInstancia()->setModoServidor( atoi(m_txtPort->getTexto().data()) );
    }
    else{

        ControladorRed::getInstancia()->setModoCliente(m_txtServer->getTexto(), atoi(m_txtPort->getTexto().data()));

        if (!waitForConnection())
            return this;
    }

    CtrlJuego::getInstance()->setModo(mj);

    return MainMenu3D::getInstance();
}

void MenuRed::setModo(ModoMenuRed m)
{
    m_modoRed = m;
    if (m == ModoServidor)
    {
        m_txtServer->setTexto("localhost");
        m_txtServer->setReadOnly(true);
    }
    else
    {
        m_txtServer->setReadOnly(false);
    }
}

void MenuRed::onError(std::string message)
{
    m_eventData = message;
    m_onError = true;
    m_event->signal();
}

void MenuRed::onOK(std::string message)
{
    m_eventData = message;
    m_onError = false;
    m_event->signal();
}

bool MenuRed::waitForConnection()
{
    m_event->wait();
    if (m_onError)
    {
        if (!m_eventText){
            m_eventText = new Texto2D(m_eventData);
            //m_eventText->posicionar(Vector(9.5,-5.5,0));
            m_eventText->posicionar(Vector(0,-5.5,0));
            m_eventText->rotar(PI, Vector(0,1,0), false);
            //m_eventText->escalar(0.7);
            m_eventText->escalar(0.85);
            m_eventText->setColor(1,0,0);
        }
        else{
            m_eventText->setTexto(m_eventData);
        }
        m_escena->removeDibujable(m_items[2]->getDibujable());
        m_escena->addDibujable(m_eventText);
        return false;
    }
    return true;
}

void MenuRed::OnEnter()
{
    if (m_modoRed == ModoServidor){
        m_selectedIndex = 2;
        m_txtServer->setFocus(false);
    }
    else{
        m_selectedIndex = 0;
        m_txtServer->setFocus(true);
    }
    Menu::OnEnter();
}
