#include "MenuConfig3D.h"
#include "MainMenu3D.h"
#include <sstream>

MenuConfig3D* MenuConfig3D::s_instance = NULL;

MenuConfig3D* MenuConfig3D::getInstance()
{
    if (!s_instance)
        s_instance = new MenuConfig3D();
    return s_instance;
}


MenuConfig3D::MenuConfig3D()
    : Menu(MainMenu3D::getInstance())
{
    Init(true);

    m_posXIniciales = new float[4];  //m_items.size()
    m_deltaXAlineando = new float[4];

    m_posXIniciales[0] = 11.5;
    m_posXIniciales[1] = 11.5;
    m_posXIniciales[2] = 11.5;
    m_posXIniciales[3] = 11.5;
    //m_posXIniciales[2] = 1;
    m_deltaXAlineando[0] = 1;
    m_deltaXAlineando[1] = 1;
    m_deltaXAlineando[2] = 1;
    m_deltaXAlineando[3] = 1;
    //m_deltaXAlineando[2] = 0.5;

    float y = 2.6;;
    float dy = 2.1;
    Luz * luz;
    for (int i = 0; i < m_items.size(); i++)
    {
        Dibujable * d = m_items[i]->getDibujable();
        d->setOpacidad(0.7);
        d->posicionar(Vector(m_posXIniciales[i], y,1.0));
        d->rotar(PI, Vector(0,1,0));
        m_escena->addDibujable(d);

        y -= dy;
    }

    m_txtUser = new TextBox(6, Vector(-2.3, 2.5, 0));
    m_txtUser->setMaxTextLength(18);
    m_txtUser->addToEscena(m_escena);
    m_txtUser->setFocus(true);

    m_strCameras = new std::string[3];
    m_strCameras[0] = "asteroide";
    m_strCameras[1] = "nave";
    m_strCameras[2] = "clasica";
    m_indexCamera = 0;

    m_txtCamera = new TextBox(6, Vector(-4.5, 0.25, 0));
    m_txtCamera->setReadOnly(true);
    m_txtCamera->addToEscena(this->m_escena);
    m_txtCamera->setTexto(m_strCameras[m_indexCamera]);

    m_txtMusicVol = new TextBox(6, Vector(-7,-1.5,0), "charset3d.mdl");
    m_txtMusicVol->setNumeric(true);
    m_txtMusicVol->addToEscena(this->m_escena);
    m_txtMusicVol->setMaxTextLength(3);

    m_txtGameVol = new TextBox(6, Vector(-7, -3.5, 0), "charset3d.mdl");
    m_txtGameVol->setNumeric(true);
    m_txtGameVol->addToEscena(this->m_escena);
    m_txtGameVol->setMaxTextLength(3);
}

MenuConfig3D::~MenuConfig3D()
{
}

void MenuConfig3D::iniciarItems()
{
    addTextItem("Usuario");
    addTextItem("Camara 2");
    addTextItem("Vol. Musica");
    addTextItem("Vol. Juego");
}

Menu * MenuConfig3D::onEnterPressed()
{
    Settings* settings = Settings::getInstance();
    settings->setUserName(m_txtUser->getTexto());
    settings->setVolumenJuego(atoi(m_txtGameVol->getTexto().data()));
    settings->setVolumenMusica(atoi(m_txtMusicVol->getTexto().data()));
    if (m_indexCameraChanged)
        ModoJuego::getInstance()->setCamara2View(m_indexCamera+1);
    return MainMenu3D::getInstance();
}

void MenuConfig3D::handleIndexChanged(int last)
{
    switch(m_selectedIndex)
    {
        case 0:
            m_txtUser->setFocus(true);
            m_txtMusicVol->setFocus(false);
            m_txtGameVol->setFocus(false);
            break;
        case 1:
            m_txtUser->setFocus(false);
            m_txtMusicVol->setFocus(false);
            m_txtGameVol->setFocus(false);
            break;
        case 2:
            m_txtUser->setFocus(false);
            m_txtMusicVol->setFocus(true);
            m_txtGameVol->setFocus(false);
            break;
        case 3:
            m_txtUser->setFocus(false);
            m_txtMusicVol->setFocus(false);
            m_txtGameVol->setFocus(true);
            break;
    }
}

Menu* MenuConfig3D::onTicCheckKey()
{
    m_txtUser->tic();
    if (m_txtUser->isFocused())
        handleIndexText();
    m_txtMusicVol->tic();
    m_txtGameVol->tic();

    if (atoi(m_txtMusicVol->getTexto().data()) > 100)
        m_txtMusicVol->setTexto("100");
    if (atoi(m_txtGameVol->getTexto().data()) > 100)
        m_txtGameVol->setTexto("100");

    return Menu::onTicCheckKey();
}

void MenuConfig3D::checkCustomKeys()
{
    if (!m_selectedIndex) return;

    Teclado * t = Teclado::getInstancia();

    int vol;
    if (t->spTecla(GLUT_KEY_LEFT))
    {
        t->setSpTecla(GLUT_KEY_LEFT, false);
        stringstream str;
        switch(m_selectedIndex)
        {
            case 0:
                break;
            case 1:
                m_indexCamera = m_indexCamera ? (m_indexCamera - 1) % 3 : 2;
                m_txtCamera->setTexto(m_strCameras[m_indexCamera]);
                handleIndexCamera();
                break;
            case 2:
                vol = atoi(m_txtMusicVol->getTexto().data()) - 1;
                if (vol == -1) break;
                str<< vol;
                m_txtMusicVol->setTexto(str.str());
                Settings::getInstance()->setVolumenMusica(vol);
                break;
            case 3:
                vol = atoi(m_txtGameVol->getTexto().data()) - 1;
                if (vol == -1) break;
                str<< vol;
                m_txtGameVol->setTexto(str.str());
                Settings::getInstance()->setVolumenJuego(vol);
                break;
        }
        str.clear();
    }
    else if (t->spTecla(GLUT_KEY_RIGHT))
    {
        t->setSpTecla(GLUT_KEY_RIGHT, false);
        stringstream str;
        switch(m_selectedIndex)
        {
            case 0:
                break;
            case 1:
                m_indexCamera = (m_indexCamera + 1) % 3;
                m_txtCamera->setTexto(m_strCameras[m_indexCamera]);
                handleIndexCamera();
                break;
            case 2:
                vol = atoi(m_txtMusicVol->getTexto().data()) + 1;
                if (vol == 101) break;
                str<< vol;
                m_txtMusicVol->setTexto(str.str());
                Settings::getInstance()->setVolumenMusica(vol);
                break;
            case 3:
                vol = atoi(m_txtGameVol->getTexto().data()) + 1;
                if (vol == 101) break;
                str<< vol;
                m_txtGameVol->setTexto(str.str());
                Settings::getInstance()->setVolumenJuego(vol);
                break;
        }
        str.clear();
    }
}

void MenuConfig3D::OnEnter()
{
    Settings* settings = Settings::getInstance();
    stringstream strMus;
    strMus<<settings->getVolumenMusica();
    m_txtMusicVol->setTexto(strMus.str());
    strMus.clear(); // no se por que no lo puedo usar de nuevo al gil!

    stringstream strGam;
    strGam<<settings->getVolumenJuego();
    m_txtGameVol->setTexto(strGam.str());
    strGam.clear();

    m_txtUser->setTexto(settings->getUserName());
    handleIndexText();
    m_indexCameraChanged = false;

    Menu::OnEnter();
}

void MenuConfig3D::handleIndexCamera()
{
    if (!m_indexCameraChanged) m_indexCameraChanged = 1;
    switch(m_indexCamera)
    {
        case 0:
            m_txtCamera->setPosicion(Vector(-4.5, 0.25, 0));
            break;
        case 1:
            m_txtCamera->setPosicion(Vector(-6.1, 0.25, 0));
            break;
        case 2:
            m_txtCamera->setPosicion(Vector(-5.2, 0.25, 0));
            break;
    }
}

void MenuConfig3D::handleIndexText()
{
    int len = m_txtUser->getTexto().size();
    Vector pos = m_txtUser->getPosicion();
    // desastre
    switch(len)
    {
        case 0:
        case 1:
            pos = Vector(-7.3, 2.5, 0);
            break;
        case 2:
            pos = Vector(-6.9, 2.5, 0);
            break;
        case 3:
            pos = Vector(-6.5, 2.5, 0);
            break;
        case 4:
            pos = Vector(-6.1, 2.5, 0);
            break;
        case 5:
            pos = Vector(-5.7, 2.5, 0);
            break;
        case 6:
            pos = Vector(-5.3, 2.5, 0);
            break;
        case 7:
            pos = Vector(-4.9, 2.5, 0);
            break;
        case 8:
            pos = Vector(-4.5, 2.5, 0);
            break;
        case 9:
            pos = Vector(-4.05, 2.5, 0);
            break;
        case 10:
            pos = Vector(-3.65, 2.5, 0);
            break;
        case 11:
            pos = Vector(-3.3, 2.5, 0);
            break;
        case 12:
            pos = Vector(-2.72, 2.5, 0);
            break;
        case 13:
            pos = Vector(-2.45, 2.5, 0);
            break;
        case 14:
            pos = Vector(-2, 2.5, 0);
            break;
    }
    m_txtUser->setPosicion(pos);
}
