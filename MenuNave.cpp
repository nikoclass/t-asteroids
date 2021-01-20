#include "MenuNave.h"

#include "Objeto3d.h"
#include "NaveGhostM.h"
#include "NavePichi.h"
#include "NavePesada.h"

//#include "Logger.h"

MenuNave * MenuNave::s_instance = NULL;

MenuNave *MenuNave::getInstance()
{
    if (NULL == s_instance)
        s_instance = new MenuNave();

    return s_instance;
}

MenuNave::MenuNave()
        :Menu(MainMenu3D::getInstance())
{
    Init();

    m_otraLuz = new Luz();
    m_otraLuz->setPos(Vector(0,6,10));
    m_otraLuz->setDistanciaMaxima(3000);
    m_otraLuz->setDireccion(Vector(0,0,1));
    m_escena->addLuz(m_otraLuz);

    Luz* otraLuz = new Luz();
    otraLuz->setPos(Vector(0,10,-8));
    otraLuz->setDistanciaMaxima(3000);
    otraLuz->setDireccion(Vector(0,3,1));
    m_escena->addLuz(otraLuz);
    //otraLuz->setLensFlares(true);

    otraLuz = new Luz();
    otraLuz->setPos(Vector(0,0,-20));
    otraLuz->setDistanciaMaxima(3000);
    otraLuz->setDireccion(Vector(0,0,1));
    m_escena->addLuz(otraLuz);
    //otraLuz->setLensFlares(true);

    otraLuz = new Luz();
    otraLuz->setPos(Vector(0,-10,-8));
    otraLuz->setDistanciaMaxima(3000);
    otraLuz->setDireccion(Vector(0,-3,1));
    m_escena->addLuz(otraLuz);
    //otraLuz->setLensFlares(true);

    int i = 0;
    for (vector<Dibujable*>::iterator it = m_ships.begin(); it != m_ships.end(); it++)
    {
        Dibujable *d = (*it);
        d->escalar(3);
        if (i != 1)
            d->rotar(PI / 12, Vector(1,0,0), false);
        else
            d->rotar(-PI / 12, Vector(1,0,0), false);
        d->posicionar(Vector(0, 1, 0));
        d->setAfectaLuces(true);
        d->setAfectaZbuffer(true);
        d->setAfectaVista(true);
        i++;
    }
    m_deltaXAlineando = new float[m_items.size()];
    m_posXIniciales   = new float[m_items.size()];

    m_deltaXAlineando[0] = 1;
    m_deltaXAlineando[1] = 0;
    m_deltaXAlineando[2] = 1;

    m_posXIniciales[0] = 5;
    m_posXIniciales[1] = 9.5;
    m_posXIniciales[2] = 5;

    i = 0;
    for (vector<MenuItem*>::iterator it = m_items.begin(); it != m_items.end(); it++)
    {
        Dibujable *d = (*it)->getDibujable();
        d->rotar(PI, Vector(0,1,0), false);
        d->posicionar(Vector(m_posXIniciales[i], 10, 0)); i++;
        d->setAfectaLuces(true);
        d->setAfectaZbuffer(true);
        d->setAfectaVista(true);
        d->setColor(Vector(50, 255, 255));
    }
    m_naveInit = false;
    m_goToGame = false;
    SetNaveActual(-1);

    //m_efectoRotEsc->setCurrent(dynamic_cast<Texto3D*>(m_items[0]->getDibujable()), m_deltaXAlineando[0]);
}

MenuNave::~MenuNave()
{
    for (int i = 0; i < m_ships.size(); i++)
        delete m_ships[i];
    delete m_otraLuz;
}

void MenuNave::SetNaveActual(int oldIndex)
{
    if (oldIndex != -1){
        m_escena->removeDibujable(m_items[oldIndex]->getDibujable());
        m_escena->removeDibujable(m_ships[oldIndex]);
    }

    m_escena->addDibujable(m_items[m_selectedIndex]->getDibujable());
    m_escena->addDibujable(m_ships[m_selectedIndex]);
}

void MenuNave::iniciarItems()
{
    // nave ghost
    addTextItem("Ghost");
    Objeto3d *obj = new Objeto3d("GhostM.mdl","Fuselage2.tga");
    obj->setOpacidad(0.5);
    m_ships.push_back(obj);

    // Nave clásica
    addTextItem("J-Fighter");
    m_ships.push_back(new Objeto3d("nave.mdl","nave.tga"));

    // La pesada
    addTextItem("Hummer");
    m_ships.push_back(new Objeto3d("NavePesada.mdl","NF_Coc02_bk.tga"));

    m_ships[1]->escalar(1.3);
    m_ships[2]->escalar(0.7);
}

Menu* MenuNave::onTicCheckKey()
{
    Menu *ret = Menu::onTicCheckKey();

    float deltaT = Engine::getInstancia()->getDt();
    float rotarItem = deltaT * PI / 10;

    //TODO: Rotacion Naves
    /// mmm, la hummer es muy pesada, me parece que voy a tener que rotar solo al actual
    /*for (vector<Dibujable*>::iterator it = m_ships.begin() ; it != m_ships.end(); it++)
    {
        (*it)->rotar(rotarItem, Vector(0,1,0),false);
    }*/
    m_ships[m_selectedIndex]->rotar(rotarItem, Vector(0,1,0),false);
    return ret;
}

void MenuNave::handleIndexChanged(int last)
{
    SetNaveActual(last);
}

Menu* MenuNave::onEnterPressed()
{
    Menu * ret = MainMenu3D::getInstance();
    switch (m_selectedIndex){
                case 1:
                    Settings::getInstance()->setNave(new NavePichi(new Objeto3d("nave.mdl","nave.tga"), Vector()));
                    break;
                case 0:
                    Settings::getInstance()->setNave(new NaveGhostM(new Objeto3d("GhostM.mdl","Fuselage2.tga"), Vector()));
                    break;
                case 2:
                    Settings::getInstance()->setNave(new NavePesada(new Objeto3d("NavePesada.mdl","NF_Coc02_bk.tga"), Vector()));
                    break;
    }
    if (m_goToGame)
    {
        //ModoJuego::getInstance()->setNavePrinc(Settings::getInstance()->getNave());
        ret =  MenuJuego::getInstance();
    }
    return ret;
}

void MenuNave::checkCustomKeys()
{
    Teclado *t = Teclado::getInstancia();
    if (t->spTecla(GLUT_KEY_LEFT))
    {
        t->setSpTecla(GLUT_KEY_LEFT, false);
        if (m_selectedIndex == 0)
            m_selectedIndex = m_items.size() - 1;
        else
            m_selectedIndex -= 1;
    }
    else if (t->spTecla(GLUT_KEY_RIGHT))
    {
        t->setSpTecla(GLUT_KEY_RIGHT, false);
        if (m_selectedIndex ==  m_items.size() - 1)
            m_selectedIndex = 0;
        else
            m_selectedIndex += 1;
    }
}

void MenuNave::OnLeave()
{
    if (m_goToGame && CtrlJuego::getInstance()->getModo() == ModoMenu3D::getInstance())
        m_goToGame = false;
}
