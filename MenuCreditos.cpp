#include "MenuCreditos.h"
#include <sstream>

#include "Texto2D.h"

MenuCreditos* MenuCreditos::s_instance=NULL;

MenuCreditos* MenuCreditos::getInstance()
{
	if(!s_instance)
        s_instance = new MenuCreditos();
	return s_instance;
}

MenuCreditos::MenuCreditos()
    : Menu(MainMenu3D::getInstance(), false)
{
    Init(true);

    m_posXIniciales = new float[3];
    m_posXIniciales[0] = 9.3;
    m_posXIniciales[1] =  0;
    m_posXIniciales[2] = 9.5;
    m_posXIniciales[3] = 0;
    m_posXIniciales[4] = 7.5;
    m_posXIniciales[5] = 0;

    Material m;
    m.setDiffuse(Vector(0.2,0,0.5));
    m.setShininess(20);
    m.setSpecular(Vector(1,1,1));

    float y = 4.5;
    float dy = 3;
    Luz * luz;
    Dibujable* dib;
    for (int i = 0; i < m_items.size() - 4; i++)
    {
        dib = m_items[i]->getDibujable();
        m_escena->addDibujable(dib);
        if(!(i%2))
        {
            Texto3D * d = (Texto3D*) dib;
            d->setOpacidad(0.7);
            d->posicionar(Vector(m_posXIniciales[i], y, 0));
            d->rotar(PI, Vector(0,1,0));
            d->escalar(1.2);

            for (int j = 0; j < d->getTexto().size(); j++)
            {
                d->getCaracter(j)->setMaterial(m);
            }

            luz = new Luz();
            luz->setIntensidad(0.3);
            luz->setEscena(m_escena);
            luz->setLensFlares(true);
            luz->setDistanciaMaxima(50);
            luz->setPos(Vector(m_posXIniciales[i] + 1.5, y, 0.5));
            m_escena->addLuz(luz);
            m_lightsLeft.push_back(luz);

            luz = new Luz();
            luz->setIntensidad(0.3);
            luz->setEscena(m_escena);
            luz->setLensFlares(true);
            luz->setDistanciaMaxima(50);
            luz->setPos(Vector(-m_posXIniciales[i] - 1.5, y, 0.5));
            m_escena->addLuz(luz);
            m_lightsRight.push_back(luz);
            y -= dy;
        }
        else
        {
            dib->escalar(0.8);
            dib->setColor(0.85, 0.85, 0.85);
            dib->posicionar(Vector(m_posXIniciales[i], y + 1.5, 1.0));
        }
    }

    // Agradecimientos
    y = -5.5;
    dy = 0.9;
    for (int h = 4; h > 0; h--){
        dib = m_items[m_items.size() - h]->getDibujable();
        dib->posicionar(Vector(0, y, 1));
        dib->escalar(dy - 0.1);
        dib->setColor(0.85, 0.85, 0.85);
        m_escena->addDibujable(dib);
        if(dy > 0.8) dy = 0.8;
        y -= dy;
    }
}

void MenuCreditos::iniciarItems()
{
    addTextItem("Nicolas Bagnasco");
    m_items.push_back(new MenuItem(new Texto2D("nikoclass@gmail.com")));
    addTextItem("Luciano de Munno");
    m_items.push_back(new MenuItem(new Texto2D("madlink@gmail.com")));
    addTextItem("Juan Ramirez");
    m_items.push_back(new MenuItem(new Texto2D("juanantonio.ram@gmail.com")));


    Texto2D *txt = new Texto2D("Agradecimientos especiales", .7, .7,  "charset3.tga" );
    m_items.push_back(new MenuItem( txt ));

    txt = new Texto2D("Guillermo Winkler", .7, .7, "charset3.tga" );
    m_items.push_back(new MenuItem( txt ));

    txt = new Texto2D("Ignacio Burgeno", .7, .7, "charset3.tga" );
    m_items.push_back(new MenuItem( txt ));

    txt = new Texto2D("Luis Marcone", .7, .7, "charset3.tga" );
    m_items.push_back(new MenuItem( txt ));
}

Menu* MenuCreditos::onTicCheckKey()
{
    // Para ver que meto
/*
    float tt = Engine::getInstancia()->getTiempoTotal();

    Vector dir( (cos(tt) * 5), sin(tt * 1.3) * 10, ( tt * cos(tt)));
    for (int i = 0; i < m_lightsLeft.size(); i++)
    {
        m_lightsLeft[i]->setDireccion(dir);
        m_lightsRight[i]->setDireccion(dir);
    }
*/
    return Menu::onTicCheckKey();
}

void MenuCreditos::handleIndexChanged(int last)
{
}

MenuCreditos::~MenuCreditos()
{
    for (int i = 0; i < m_lightsLeft.size(); i++)
    {
        delete m_lightsLeft[i];
        m_lightsLeft.clear();
        delete m_lightsRight[i];
        m_lightsRight.clear();
    }
}

