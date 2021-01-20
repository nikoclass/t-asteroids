#include "Menu.h"


#include "SoundManager.h"
#include "Logger.h"
#include "Teclado.h"

using std::string;

Menu::Menu(Menu * owner, bool useEffect, string modeloTitulo)
    : m_useEffect(useEffect)
{
    m_rotar = true;
    m_mFront.setDiffuse(Vector(0,0,0.6));
    m_mFront.setShininess(15);
    m_mFront.setSpecular(Vector(1,1,1));

    m_mBack.setDiffuse(Vector(0.3,0.5,0.2));
    m_mBack.setSpecular(Vector(1,1,1));

    m_efectoRotEsc = new EfectoRotEsc(m_mFront,m_mBack);

    m_owner = owner;

    m_modeloTitulo = modeloTitulo;
    m_selectedIndex  = 0; //valor por default, por las dudas :)

    m_escena = new Escena();
    m_escena->setFactorAmbiente(0.5);

    m_camara = new Camara("principal");
    m_camara->setPosicion(Vector(0,0,-15));
    m_camara->mirarHacia(Vector(0,1,1));

    m_escena->addCamara(m_camara);

    //m_soundName = "./Sonidos/wave.mp3"; // las clases que heredan puede sobreescribir :)
    m_soundName = "./Sonidos/Cruentus.mp3";

    m_hiddenItemCount = 0;

    m_started = false;
}

void Menu::Init(bool showTitle)
{
    m_posXIniciales = 0;
    m_deltaXAlineando = 0;

    iniciarItems(); // clases hijas

    this->m_universo = new Multiobjeto3d("universo.mdl");
    Objeto3d* objFrente = dynamic_cast<Objeto3d*>(m_universo->getObjeto("frente"));
    if (!objFrente)
        return;
    objFrente->cargarTextura("sp07_FT.tga");
    objFrente->setModoTextura(Reemplazar);
    objFrente->setAfectaDetrasCamara(false);

    Objeto3d* objAbajo = dynamic_cast<Objeto3d*>(m_universo->getObjeto("abajo"));
    if (!objAbajo)
        return;
    objAbajo->cargarTextura("sp07_DN.tga");
    objAbajo->setModoTextura(Reemplazar);
    objAbajo->setAfectaDetrasCamara(false);

    Objeto3d* objArriba = dynamic_cast<Objeto3d*>(m_universo->getObjeto("arriba"));
    if (!objArriba)
        return;
    objArriba->cargarTextura("sp07_UP.tga");
    objArriba->setModoTextura(Reemplazar);
    objArriba->setAfectaDetrasCamara(false);

    Objeto3d* objAtras = dynamic_cast<Objeto3d*>(m_universo->getObjeto("atras"));
    if (!objAtras)
        return;
    objAtras->cargarTextura("sp07_BK.tga");
    objAtras->setModoTextura(Reemplazar);
    objAtras->setAfectaDetrasCamara(false);

    Objeto3d* objDerecha = dynamic_cast<Objeto3d*>(m_universo->getObjeto("derecha"));
    if (!objDerecha)
        return;
    objDerecha->cargarTextura("sp07_RT.tga");
    objDerecha->setModoTextura(Reemplazar);
    objDerecha->setAfectaDetrasCamara(false);

    Objeto3d* objIzquierda = dynamic_cast<Objeto3d*>(m_universo->getObjeto("izquierda"));
    if (!objIzquierda)
        return;
    objIzquierda->cargarTextura("sp07_LF.tga");
    objIzquierda->setModoTextura(Reemplazar);
    objIzquierda->setAfectaDetrasCamara(false);

    m_universo->escalar(10000);
    m_universo->setAfectaLuces(false);
    m_universo->setAfectaDetrasCamara(false);

    m_escena->addDibujable(m_universo);

    if (showTitle)
    {
        m_titulo = new Objeto3d(m_modeloTitulo.data(),"envmapnave.tga");
        m_titulo->setModoGeneracionCoordTextura(Enviromental);
        m_titulo->setAfectaVista(false);
        m_titulo->setAfectaLuces(false);
        m_escena->addDibujable(m_titulo);
    }
    else m_titulo=NULL;

    m_luzUno = new Luz();
    m_luzUno->setPos(Vector(-20,-20,0));
    m_luzUno->setIntensidad(0.8);
    m_escena->addLuz(m_luzUno);

    m_luzDos = new Luz();
    m_luzDos->setIntensidad(0.6);
    m_luzDos->setDistanciaMaxima(50);
    m_escena->addLuz(m_luzDos);
}


Menu* Menu::tic()
{
    Menu* ret = onTicCheckKey();

    m_efectoRotEsc->tic();

    float tt = Engine::getInstancia()->getTiempoTotal();
    m_luzUno->setPos(Vector(cos(tt),cos(tt + 1),sin(tt * 1.3)) * 10);

    m_luzDos->setPos(Vector(cos(tt*1.3),cos(0.7*tt + 1) * 10,0));

    if (m_rotar)
        m_universo->rotar(Engine::getInstancia()->getDt() / 10,Vector(1,0,-1),false);

    if(this->m_titulo){
        this->m_titulo->identidad();
        this->m_titulo->posicionar(Vector(0,1,-2.0));
        this->m_titulo->rotar(cos(Engine::getInstancia()->getTiempoReal())*0.25,Vector(0,1,0));
   	}
   	return ret;
}

Menu* Menu::onTicCheckKey()
{
    Teclado *t = Teclado::getInstancia();

    if (t->tecla(13))
    {
        t->setTecla(13,false);
        return onEnterPressed();
    }
    if (t->tecla(27) && m_owner)
    {
        t->setTecla(27, false);
        if(m_owner)
            return m_owner;
    }
    int selected = m_selectedIndex;
    if (t->spTecla(GLUT_KEY_DOWN))
    {
        t->setSpTecla(GLUT_KEY_DOWN,false);
        if (m_selectedIndex == m_items.size() - 1)
            m_selectedIndex = 0;
        else
            m_selectedIndex += 1;
    }
    if (t->spTecla(GLUT_KEY_UP))
    {
        t->setSpTecla(GLUT_KEY_UP,false);
        if (m_selectedIndex == 0)
            m_selectedIndex = m_items.size() - 1;
        else
            m_selectedIndex -= 1;
    }
    checkCustomKeys();
    if (selected != m_selectedIndex)
    {
        handleIndexChanged(selected);
        if (m_useEffect){
            // En el caso de que el item contenga un texto3d le aplico el efectito
            Texto3D * t = dynamic_cast<Texto3D*>(m_items[m_selectedIndex]->getDibujable());
            if (t)  // hay que respeta los gailain
            {
                m_efectoRotEsc->setCurrent(t, m_deltaXAlineando[m_selectedIndex]);
            }
        }
    }
    return this;
}

void Menu::OnEnter()
{
    // Una simple medida de prevencion
    if (m_useEffect)
    {
        if (!m_started) // Esto no se puede hacer en el contructor ya que aun no se han inicializado los deltas
        {
            if (!m_posXIniciales){
                m_posXIniciales = new float[m_items.size()];
                for (int i = 0; i < m_items.size(); i++)
                    m_posXIniciales[i] = 0;
            }
            if (!m_deltaXAlineando){
                m_deltaXAlineando = new float[m_items.size()];
                for (int i = 0; i < m_items.size(); i++)
                    m_deltaXAlineando[i] = 0;
            }

            for (int i = 0; i < m_items.size(); i++)
            {
                if (Texto3D* t = dynamic_cast<Texto3D*>(m_items[i]->getDibujable()))
                {
                    int len = t->getTexto().size();
                    for (int j = 0; j < len; j++)
                        t->getCaracter(j)->setMaterial(m_mBack);
                }
            }

            m_started = true;
        }

        if (Texto3D* t = dynamic_cast<Texto3D*>(m_items[m_selectedIndex]->getDibujable()))
            m_efectoRotEsc->setCurrent(t, m_deltaXAlineando[m_selectedIndex]);
    }
    if (Engine::getInstancia()->getEscena() != m_escena)
        Engine::getInstancia()->setEscena(m_escena);
}

void Menu::addTextItem(const string & text)
{
    m_items.push_back(new MenuItem(new Texto3D(text, "charset3dSpider.mdl")));
}

Menu::~Menu()
{
    for (vector<MenuItem*>::iterator it = m_items.begin(); it != m_items.end(); it++)
        delete (*it);
}
