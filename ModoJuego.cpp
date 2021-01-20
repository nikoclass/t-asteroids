#include "ModoJuego.h"

#include "NavePichi.h"
#include "NaveGhostM.H"
#include "NavePesada.h"
#include "CondTargeteable.h"
#include "Targeteable.h"

#include "Teclado.h"
#include "Mouse.h"

#include "Logger.h"

ModoJuego* ModoJuego::instance=NULL;
int ModoJuego::count =0;

ModoJuego::ModoJuego()
{
    this->escena=new Escena();

    this->pausa=false;
    //this->anguloPausa=0;

    this->cabina=false;
    this->camara2view=0;
    //ASTEROIDES
    this->asteroidsCount=0;

    this->camara1=new Camara("principal");
    this->camara1->setPosicion(Vector(1000,500,0));
    //cout << "ModoJuego cons" << endl;
    //this->camara2=new Camara("secundaria");
    //this->camara2->setAnchoAlto(0.3,0.3);
    //this->camara2->setXY(0.7,0);

    escena->addCamara(this->camara1);
    //escena->addCamara(this->camara2);

//    this->mejorAsteroide=NULL;
//    this->distMejorAsteroide=0;

    this->naveClientes=new HashTable();
    this->cliente=NULL;

    x=0;
    y=0;
    m_userName = NULL;

    m_started = false;

    modoCliente = false;
    modoSingle = true;
    m_isOver = false;
    m_waitKilled = false;

    this->addEscenografia();

    ControladorRed::getInstancia()->addObserver(this);
}

void ModoJuego::addAsteroide(Asteroide*a)
{
    this->asteroides[a]=a;
}

bool ModoJuego::isAsteroide(Asteroide*a)
{
    map<Asteroide*,Asteroide*>::iterator i;
    i=this->asteroides.find(a);
    return i!=this->asteroides.end();
}

void ModoJuego::removeAsteroide(Asteroide*a)
{
    map<Asteroide*,Asteroide*>::iterator i;
    i=this->asteroides.find(a);
    this->asteroides.erase(i);
}

ModoJuego* ModoJuego::getInstance()
{
    if (!ModoJuego::instance)
    {
        //ModoJuego::instance=(ModoJuego*)1;
        //count++;
        //cout << "entro en el getinstanci primero " << count << endl;
        ModoJuego::instance=new ModoJuego();
        //cout << "Esto es una prueba, deberia haber creao una navey esto aparecer solo una vez";
    }
    return ModoJuego::instance;
}

ModoJuego::~ModoJuego()
{
    delete this->objvida;
    delete this->objmisil;
    delete this->objaster;
    delete this->objvel;

    delete this->textoVida;
    delete this->textoMisil;
    delete this->textoAster;
    delete this->textoVel;

    delete this->objtarget;
}
void ModoJuego::reset()
{
    if (this->modoCliente || this->modoSingle)
    {
        vector<Colisionable*> ccs = MFisica::getInstance()->getColisionables();
        for (int i = 0;i < ccs.size(); i++)
            if (ccs[i]->getModelo()->getEscena() ==this->escena)
                delete ccs[i];

        escena->removeDibujable(textoVida);
        delete this->textoVida;
        escena->removeDibujable(textoMisil);
        delete this->textoMisil;
        escena->removeDibujable(textoAster);
        delete this->textoAster;
        escena->removeDibujable(textoVel);
        delete this->textoVel;
    }
}

void ModoJuego::ini()
{
    internalAddEscene();

    int c=Settings::getInstance()->getCantAsteroides();
    if (modoCliente)
        c=0;

    for (int i = 1; i <= c; i++)
    {
        crearAsteroide(i%13+1);
    }
}

void ModoJuego::internalAddEscene()
{
    this->textoVida=new Texto2D("");
    this->textoVida->posicionar(Vector(-0.79,-0.61,-1));
    this->textoVida->escalar(0.03);
    this->textoVida->setAfectaZbuffer(false);
    this->textoVida->setAfectaVista(false);
    this->textoVida->setCamara("principal");

    this->textoMisil=new Texto2D("");
    this->textoMisil->posicionar(Vector(-0.79,-0.55,-1));
    this->textoMisil->escalar(0.03);
    this->textoMisil->setAfectaZbuffer(false);
    this->textoMisil->setAfectaVista(false);
    this->textoMisil->setCamara("principal");

    this->textoAster=new Texto2D("");
    this->textoAster->posicionar(Vector(-0.62,-0.51,-1));
    this->textoAster->escalar(0.03);
    this->textoAster->setAfectaZbuffer(false);
    this->textoAster->setAfectaVista(false);
    this->textoAster->setCamara("principal");

    this->textoVel=new Texto2D("");
    this->textoVel->posicionar(Vector(-0.62,-0.55,-1));
    this->textoVel->escalar(0.03);
    this->textoVel->setAfectaZbuffer(false);
    this->textoVel->setAfectaVista(false);
    this->textoVel->setCamara("principal");

    /// lo qué????
    this->textoMisil->trasladar(Vector(0,-0.1,0));
    this->textoAster->trasladar(Vector(0,-0.1,0));
    this->textoVel->trasladar(Vector(0,-0.1,0));

    escena->addDibujable(this->textoVel);
    escena->addDibujable(this->textoAster);
    escena->addDibujable(this->textoMisil);
    escena->addDibujable(this->textoVida);


    particulasFlotantes=new SistemaParticulas(1000,"flare1.tga");
    particulasFlotantes->setTamanio(1.5);
    for(int i=0; i<particulasFlotantes->getCantParticulas();i++){
        Vector v(Random(-500,500),Random(-500,500),Random(-500,500));
        particulasFlotantes->setParticula(i,v);
    }
    escena->addDibujable(particulasFlotantes);

}



Asteroide* ModoJuego::crearAsteroide(int id)
{
    char master[30],taster[30];
    sprintf(master,"asteroide_%d.mdl",id);
    sprintf(taster,"asteroide_%d.tga",id);

    int level =(int) Random(5,15);

    Vector posAss=500*Vector(Random(-1,1),Random(-1,1),Random(-1,1));
    Asteroide* ass=new Asteroide(this->asteroidsCount,level,new Objeto3d(master,taster),posAss);
    ass->setVelocidad(30*Vector(Random(-1,1),Random(-1,1),Random(-1,1)));
    this->asteroidsCount++;
    return ass;
}


Nave * ModoJuego::dameNueva(Nave* n)
{
    Nave * _newNave = NULL;
    if(dynamic_cast<NavePichi*>(n))
        _newNave = new NavePichi(new Objeto3d("nave.mdl","nave.tga"),Vector());
    if(dynamic_cast<NaveGhostM*>(n))
        _newNave = new NaveGhostM(new Objeto3d("GhostM.mdl","Fuselage2.tga"), Vector());;
    if(dynamic_cast<NavePesada*>(n))
        _newNave = new NavePesada(new Objeto3d("NavePesada.mdl","NF_Coc02_bk.tga"), Vector());;

    return _newNave;
}

void ModoJuego::tic(void)
{
    ControladorRed* ctrlRed = ControladorRed::getInstancia();
    Settings *settings = Settings::getInstance();
    Mouse* mouse = Mouse::getInstancia();
    Engine* eng = Engine::getInstancia();
    Teclado* t = Teclado::getInstancia();

    // Suspende el procesado asincronico de paquetes de red y otras
    // tareas que pueden ser conflictivas ejecutarlas concurrentemente aqui
    ctrlRed->suspender();

    mouse->setCursor(GLUT_CURSOR_NONE);

    if(!modoCliente && m_waitKilled)
    {
        if (this->nave_princ){
            m_tempShip = dameNueva(this->nave_princ);
            this->nave_princ->setDead(true);
            this->nave_princ = NULL;
        }
        if (t->tecla(13) || mouse->izquierdo()){
            t->setTecla(13,false);
            this->nave_princ = m_tempShip;
            m_waitKilled = false;
            m_tempShip = 0;
        }
    }

    if (t->tecla(27))
    {
        t->setTecla(27,false);
        CtrlJuego::getInstance()->setModo(ModoMenu3D::getInstance());
        //return;
    }

    float ti = eng->getTiempoTotal();
    float dt = eng->getDt();
    float dtr = eng->getDtReal();
    float vel= settings->getVelocidad();
    float ts = eng->getTimeScale()/vel;

    PaqueteRed *p;

    if(this->nave_princ)
    {
        if (settings->isPressed(TECLA_UP) or t->spTecla(GLUT_KEY_UP)) this->nave_princ->moverAdelante(dt);
        if (settings->isPressed(TECLA_DOWN) or t->spTecla(GLUT_KEY_DOWN)) this->nave_princ->moverAtraz(dt);
        if (settings->isPressed(TECLA_UP_LEFT)) this->nave_princ->girarDerecha(dt);
        if (settings->isPressed(TECLA_UP_RIGHT)) this->nave_princ->girarIzquierda(dt);
        if (settings->isPressed(TECLA_LEFT)or t->spTecla(GLUT_KEY_LEFT)) this->nave_princ->moverIzq(dt);
        if (settings->isPressed(TECLA_RIGHT)or t->spTecla(GLUT_KEY_RIGHT)) this->nave_princ->moverDer(dt);

        if(cliente)
        {
            p=new PaqueteRed();

            p->addString("Cliente Accion");
            p->addInt(ControladorRed::getInstancia()->getNumCliente());

            p->addFloat(dt);

            p->addVector(this->nave_princ->getPosicion());
            p->addVector(this->nave_princ->getVelocidad());

            p->addInt(settings->isPressed(TECLA_UP) or t->spTecla(GLUT_KEY_UP));
            p->addInt(settings->isPressed(TECLA_DOWN) or t->spTecla(GLUT_KEY_DOWN));
            p->addInt(settings->isPressed(TECLA_UP_LEFT));
            p->addInt(settings->isPressed(TECLA_UP_RIGHT));
            p->addInt(settings->isPressed(TECLA_LEFT)or t->spTecla(GLUT_KEY_LEFT));
            p->addInt(settings->isPressed(TECLA_RIGHT)or t->spTecla(GLUT_KEY_RIGHT));
        }

        if (t->tecla('p') || t->tecla('P') && this->nave_princ->isViva())
        {
            this->pausa=!this->pausa;
            if (this->pausa) eng->setTimeScale(0,1);
            else eng->setTimeScale(vel,3);
            t->setTecla('p',false);
            t->setTecla('P',false);
        }

        /*if (t->tecla('x'))
        {
                t->setTecla('x',false);
                this->nave_princ->getModelo()->rotar(PI/2,this->nave_princ->getModelo()->getEjeX());
        }

        if (t->tecla('z'))
        {
            t->setTecla('z',false);
            this->nave_princ->teletransportar();
        }
        */
        if (t->tecla('t'))
        {
            t->setTecla('t',false);
            this->nave_princ->targeting();
        }
        if (t->tecla('9'))
        {
            t->setTecla('9',false);
            this->nave_princ->setEnvairomental();
        }
        if (t->tecla('8'))
        {
            t->setTecla('8',false);
            this->nave_princ->recargarSecundaria();
        }
        if (t->tecla('7'))
        {
            t->setTecla('7',false);
            this->nave_princ->balasRojas();
        }

        this->nave_princ->rotar(mouse->getVelX(),mouse->getVelY(),dt);

        if(!cliente){ //Servidor
            if (mouse->izquierdo() or t->tecla(' '))this->nave_princ->dispararPrimaria();
            if (mouse->derecho())this->nave_princ->dispararSecundaria();
        }
        else { //Cliente
            p->addFloat(mouse->getVelX());
            p->addFloat(mouse->getVelY());

            p->addInt(mouse->izquierdo() or t->tecla(' '));
            p->addInt(mouse->derecho());

            cliente->enviarPaqueteCliente(p);
        }
        if(this->nave_princ->isViva())
            this->nave_princ->disparar();
    }

    Nave *naveClientes;

    list<Hasheable*> listaNavesClientes=this->naveClientes->obtenerElementos();
    list<Hasheable*>::iterator iter;
    for(iter=listaNavesClientes.begin();iter!=listaNavesClientes.end();iter++) {
        naveClientes=dynamic_cast<Nave*>(*iter);
        if(naveClientes && naveClientes->isViva())
            naveClientes->disparar();
    }

    MFisica::getInstance()->tic(dt);

    this->ticEscenografia();

    if (this->nave_princ)
    {
        if (this->nave_princ->isViva())
        {
            this->textoPausa->setVisible(false);
            if (this->pausa)
            {
                this->textoPausa->setVisible(true);
                this->textoPausa->setOpacidad(1-ts);
                float dtr=eng->getDtReal();
                this->anguloPausa[RHO]=10;
                this->anguloPausa[PHI]+=mouse->getVelX()*dtr*0.1;
                this->anguloPausa[THETA]+=mouse->getVelY()*dtr*0.1;
                Vector posCamara(0,5,15);
                posCamara=this->nave_princ->getModelo()->getTransformacion()*Matriz4x4::RotacionY(this->anguloPausa[PHI])*Matriz4x4::RotacionX(this->anguloPausa[THETA])*posCamara;

                Vector dif=5*dtr*(posCamara-camara1->getPosicion());
                camara1->setPosicion(camara1->getPosicion()+dif);
                camara1->mirarHacia(this->nave_princ->getModelo()->getPosicion());
                camara1->setUp(this->nave_princ->getModelo()->getEjeY());
            }
            else if (!this->cabina)
            {
                this->textoPausa->setVisible(false);
                Vector posCamara(0,3,8);
                posCamara = this->nave_princ->getModelo()->getTransformacion() * posCamara;
                Vector dif=4*dtr*(posCamara-camara1->getPosicion());
                if (t->tecla('w') or t->spTecla(GLUT_KEY_UP)) dif*=2;
                camara1->setPosicion(camara1->getPosicion()+dif);
                camara1->mirarHacia(this->nave_princ->getModelo()->getPosicion());
                camara1->setUp(this->nave_princ->getModelo()->getEjeY());
            }
            else
            {
                this->textoPausa->setVisible(false);
                Vector posCamara(0,2,-5);
                posCamara=this->nave_princ->getModelo()->getTransformacion() * posCamara;
                camara1->setPosicion(posCamara);
                Vector hacia(0,0,-50);
                camara1->mirarHacia(this->nave_princ->getModelo()->getTransformacion()*hacia);
                camara1->setUp(this->nave_princ->getModelo()->getEjeY());
            }
        }
        else
        { //La nave no esta viva
            //if (this->modoCliente || this->modoSingle)
           // {
                this->textoPausa->setVisible(true);
                this->textoPausa->setTexto("OUTCH!");
                this->textoPausa->setOpacidad(1);
                this->textoPausa->posicionar(Vector(-0.3,0.35,-1));
                Vector posCamara=Vector(0,0,100);
                posCamara=this->nave_princ->getModelo()->getTransformacion()*posCamara;
                Vector dif=dtr*(posCamara-camara1->getPosicion());
                camara1->setPosicion(camara1->getPosicion()+dif*3);
                camara1->mirarHacia(this->nave_princ->getModelo()->getPosicion());
                camara1->setUp(this->nave_princ->getModelo()->getEjeY());

                //m_isOver = 1;
            //}
            if (this->modoCliente || this->modoSingle)
            {
                m_isOver = 1;
            }
            else // Dios nunca muere
            {
                m_waitKilled = true;
                /*Nave * nave;
                if (nave = dynamic_cast<NaveGhostM*>(this->nave_princ))
                    this->nave_princ = new NaveGhostM(new Objeto3d("GhostM.mdl","Fuselage2.tga"), Vector());
                else if (nave = dynamic_cast<NavePesada*>(this->nave_princ))
                    this->nave_princ = new NavePesada(new Objeto3d("NavePesada.mºdl","NF_Coc02_bk.tga"), Vector());
                else if (nave = dynamic_cast<NavePichi*>(this->nave_princ))
                    this->nave_princ = new NavePichi(new Objeto3d("nave.mdl","nave.tga"), Vector());
                Settings::getInstance()->setNave(nave_princ);
                */
            }
        }
        if ( !modoCliente && this->asteroides.size()<=0)
        {
            this->textoPausa->setVisible(true);
            this->textoPausa->setTexto("YOU WIN");
            this->textoPausa->setOpacidad(1);
            this->textoPausa->posicionar(Vector(-0.5,0.35,-1));
            this->anguloPausa[PHI]+=0.2*dtr;

            Vector posCamara=Vector(0,5,10);
            posCamara=this->nave_princ->getModelo()->getTransformacion()*Matriz4x4::RotacionY(this->anguloPausa[PHI])*posCamara;

            Vector dif=dtr*(posCamara-camara1->getPosicion());
            camara1->setPosicion(camara1->getPosicion()+dif);
            camara1->mirarHacia(this->nave_princ->getModelo()->getPosicion());
            camara1->setUp(this->nave_princ->getModelo()->getEjeY());
            eng->setTimeScale(0,1);
        }
    }

    if (mouse->getX()!=400 || mouse->getY()!=300)
        mouse->setPos(400,300);



    p=new PaqueteRed();

    p->addFloat(eng->getTimeScale());
    p->addInt(this->pausa);
    enviarPaqueteServidor(p);

    ctrlRed->enviarTodosLosPaquetes();

    ctrlRed->reanudar();
}


void ModoJuego::addEscenografia()
{
    internalAddEscene();
    this->objvida=new Objeto3d("podervida.mdl");
    this->objvida->posicionar(Vector(-0.83,-0.6,-1));
    this->objvida->escalar(0.02);
    this->objvida->setColor(1,0,0);
    this->objvida->setAfectaVista(false);
    this->objvida->setCamara("principal");
    escena->addDibujable(this->objvida);

    this->objmisil=new Objeto3d("misil.mdl");
    this->objmisil->posicionar(Vector(-0.83,-0.54,-1));
    this->objmisil->escalar(0.02);
    this->objmisil->rotar(-PI/2,this->objmisil->getEjeY());
    this->objmisil->rotar(PI/4,this->objmisil->getEjeX());
    this->objmisil->setAfectaVista(false);
    this->objmisil->setCamara("principal");
    escena->addDibujable(this->objmisil);

    this->objaster=new Objeto3d("asteroide_1.mdl","asteroide_1.tga");
    this->objaster->posicionar(Vector(-0.66,-0.5,-1));
    this->objaster->escalar(0.03);
    this->objaster->setColor(184,134,11);
    this->objaster->setAfectaVista(false);
    this->objaster->setCamara("principal");
    escena->addDibujable(this->objaster);

    this->objvel=new Objeto3d("fuego.mdl","fuego2.tga");
    this->objvel->posicionar(Vector(-0.66,-0.54,-1));
    this->objvel->escalar(0.03);
    this->objvel->rotar(-PI/2,this->objvel->getEjeY());
    this->objvel->rotar(PI/4,this->objvel->getEjeX());
    this->objvel->setAfectaVista(false);
    this->objvel->setCamara("principal");
    escena->addDibujable(this->objvel);

    this->objtarget=new Sprite("target.tga");
    this->objtarget->posicionar(Vector(-0.51,-0.64,-1));
    this->objtarget->escalar(0.05);
    this->objtarget->setColor(1,0,0);
    this->objtarget->setAfectaVista(false);
    this->objtarget->setCamara("principal");
    escena->addDibujable(this->objtarget);

    this->textoTarget=new Texto2D("");
    this->textoTarget->posicionar(Vector(-0.47,-0.65,-1));
    this->textoTarget->escalar(0.03);
    this->textoTarget->setAfectaZbuffer(false);
    this->textoTarget->setAfectaVista(false);
    this->textoTarget->setCamara("principal");
    escena->addDibujable(this->textoTarget);

    this->objmisil->trasladar(Vector(0,-0.1,0));

    this->objaster->trasladar(Vector(0,-0.1,0));

    this->objvel->trasladar(Vector(0,-0.1,0));

    universo=new Multiobjeto3d("universo.mdl");
    Objeto3d *o;
    o=((Objeto3d *)universo->getObjeto("frente"));
    o->cargarTextura("sp07_FT.tga");
    o->setModoTextura(Reemplazar);
    o->setAfectaDetrasCamara(false);

    o=((Objeto3d *)universo->getObjeto("abajo"));
    o->cargarTextura("sp07_DN.tga");
    o->setModoTextura(Reemplazar);
    o->setAfectaDetrasCamara(false);

    o=((Objeto3d *)universo->getObjeto("arriba"));
    o->cargarTextura("sp07_UP.tga");
    o->setModoTextura(Reemplazar);
    o->setAfectaDetrasCamara(false);

    o=((Objeto3d *)universo->getObjeto("atras"));
    o->cargarTextura("sp07_BK.tga");
    o->setModoTextura(Reemplazar);
    o->setAfectaDetrasCamara(false);

    o=((Objeto3d *)universo->getObjeto("derecha"));
    o->cargarTextura("sp07_RT.tga");
    o->setModoTextura(Reemplazar);
    o->setAfectaDetrasCamara(false);

    o=((Objeto3d *)universo->getObjeto("izquierda"));
    o->cargarTextura("sp07_LF.tga");
    o->setModoTextura(Reemplazar);
    o->setAfectaDetrasCamara(false);

    universo->escalar(10000);
    universo->setAfectaLuces(false);
    universo->setAfectaDetrasCamara(false);

    textoPausa=new Texto2D("PAUSA",0.7,0.7,"charset3.tga");
    textoPausa->escalar(0.2);
    textoPausa->posicionar(Vector(-0.35,0.4,-1));
    textoPausa->setAfectaZbuffer(false);
    textoPausa->setAfectaVista(false);
    textoPausa->setCamara("principal");

    this->asterCercano=new Texto2D("PELIGRO");
    this->asterCercano->escalar(2);
    this->asterCercano->setAfectaZbuffer(false);

    this->target=new Sprite("target.tga");
    this->target->setColor(1,0,0);
    this->target->setRotable(true);
    this->target->setAfectaZbuffer(false);
    this->target->escalar(5);
    this->target->setOpacidad(0.7);

    char* texturas[9]={"mercurio.tga","venus.tga","luna.tga","marte.tga","jupiter.tga","saturno.tga","urano.tga","neptuno.tga","pluton.tga"};
    double tamanios[9]={80,140,1000,200,450,350*0.5,240,100,40};
    Vector posiciones[9] = {Vector(200,400,2500),Vector(650,-460,1500),Vector(-3000,3000,4000),Vector(300,-1000,700),Vector(100,210,-2000),0.5*Vector(2034,190,-2500),Vector(490,1000,-3000),Vector(-1020,200,-3500),Vector(300,-3040,-4500)};

    int i;

    for (i = 0; i<9; i++)
    {
        this->planetas[i] = new Objeto3d("planeta.mdl",texturas[i]);
        this->planetas[i]->escalar(tamanios[i]);
        this->planetas[i]->posicionar(posiciones[i]);
        this->escena->addDibujable(this->planetas[i]);
    }

    this->anillosaturno=new Objeto3d("anillosaturno.mdl","anillosaturno.tga");
    this->anillosaturno->escalar(tamanios[5]);
    this->anillosaturno->rotar(PI/8,Vector(1,0,0));
    this->anillosaturno->posicionar(posiciones[5]);
    this->anillosaturno->setCullFace(false);

    this->auraLuna=new Sprite("flare2.tga");
    this->auraLuna->posicionar(posiciones[2]);
    this->auraLuna->escalar(60);
    this->auraLuna->setModoTransparencia(Aditivo);

    this->luzSol=new Luz();
    this->luzSol->setColor(Vector(1,1,1));
    this->luzSol->setPos(Vector(0,0,3000));

    this->luzGalaxia=new Luz();
    this->luzGalaxia->setIntensidad(0.2);
    this->luzGalaxia->setColor(Vector(1,1,1));
    this->luzGalaxia->setPos(Vector(3000,0,0));

    this->pointer=new Texto2D("o");
    this->pointer->escalar(0.03);
    this->pointer->trasladar(Vector(0,0,-1));
    this->pointer->setAfectaZbuffer(false);
    this->pointer->setAfectaVista(false);
    this->pointer->setVisible(false);

    escena->addLuz(luzSol);
    escena->addLuz(luzGalaxia);
    escena->addDibujable(textoPausa);
    escena->addDibujable(anillosaturno);
    escena->addDibujable(asterCercano);
    escena->addDibujable(target);
    escena->addDibujable(universo);
    escena->addDibujable(this->pointer);
    escena->addDibujable(auraLuna);

    this->luzSol->setLensFlares(true);
    this->luzGalaxia->setLensFlares(true);
}

void ModoJuego::ticEscenografia()
{
    Engine* eng=Engine::getInstancia();

    float ti=eng->getTiempoTotal();
    float dt=eng->getDt();
    float dtr=eng->getDtReal();

   //universo->posicionar(this->nave_princ->getPosicion());//se rompe cuando la nave esta en null

    particulasFlotantes->rotar(dt*0.01,Vector(0,1,0));

    for (int i=0;i<9;i++)
    {
        this->planetas[i]->rotar(dt/8,Vector(0.2,1,0.2));
    }

    char charaux[256];
    if(this->nave_princ){
        universo->posicionar(this->nave_princ->getPosicion());

        sprintf(charaux,"%3d",(int)this->nave_princ->getVida());
        this->textoVida->setTexto(charaux);

        sprintf(charaux,"%3g",this->nave_princ->getSecundaria()->getcantMunicion());
        this->textoMisil->setTexto(charaux);

        sprintf(charaux,"%3d",(int)this->nave_princ->getNormaVelocidad());
        this->textoVel->setTexto(charaux);

        if (this->nave_princ->isConTarget())
            sprintf(charaux,"on");
        else
            sprintf(charaux,"off");
        this->textoTarget->setTexto(charaux);
    }

    sprintf(charaux,"%3d",(int)this->asteroides.size());
    this->textoAster->setTexto(charaux);
    this->objvida->rotar(PI/6*dtr,this->objvida->getEjeY());
    this->objmisil->rotar(PI/6*dtr,this->objmisil->getEjeZ());
    this->objaster->rotar(PI/6*dtr,this->objaster->getEjeZ());
    this->objvel->rotar(PI/6*dtr,this->objvel->getEjeZ());
    this->target->setVisible(false);
    this->asterCercano->setVisible(false);

    if(this->nave_princ)
    {
        CondTargeteable* ct = new CondTargeteable(this->nave_princ);
        MFisica::getInstance()->getColisionablesByCondition(ct);

        if (this->nave_princ->isConTarget() && ct->getTarget()) //TODO: sujeto a cambios
        {
            this->target->setVisible(true);
            Vector dif=(ct->getTarget()->getPosicionTarget()- target->getPosicion())*0.1;

            this->target->rotar(2*dt*cos(ti*0.5),Vector(0,0,1));
            this->target->posicionar(target->getPosicion()+dif);


            this->asterCercano->setVisible(true);
            this->asterCercano->setOpacidad(0.5+0.5*cos(5*eng->getTiempoTotal()));
            this->asterCercano->posicionar(ct->getTarget()->getPosicionTarget());
        }

        delete ct;
    }

    Vector posCamara;
    Vector dif;

    //this->escena->removeCamara(this->camara2);
    switch (this->camara2view)
    {
    case 0:

        break;
    case 1:
        break;

    case 2:
        break;
    case 3:
        //this->escena->addCamara(this->camara2);
       // posCamara=Vector(0,40,0);
        //posCamara=this->nave_princ->getModelo()->getTransformacion()*posCamara;
       //dif=2*dt*(posCamara-camara2->getPosicion());
       // this->camara2->setPosicion(this->camara2->getPosicion()+dif	);
        //this->camara2->mirarHacia(this->nave_princ->getPosicion());
        //this->camara2->setUp(Vector(1,0,0));
        break;
    }
}

//float ModoJuego::getDistMejorAsteroide()
//{
//    return this->distMejorAsteroide;
//}
//void ModoJuego::setDistMejorAsteroide(float dist)
//{
//    this->distMejorAsteroide=dist;
//}
//Asteroide*ModoJuego::getMejorAsteroide()
//{
//    return this->mejorAsteroide;
//}
//void ModoJuego::setMejorAsteroide(Asteroide*a)
//{
//    this->mejorAsteroide=a;
//}
Escena* ModoJuego::getEscena()
{
    return this->escena;
}
void ModoJuego::setCamara2View(int i)
{
    this->camara2view=i;
}
int ModoJuego::getCamara2View()
{
    return this->camara2view;
}
Nave* ModoJuego::getNavePrinc()
{
    return this->nave_princ;
}
void ModoJuego::setNavePrinc(Nave* n)
{
    this->nave_princ = n;
}
Camara* ModoJuego::getCamaraPrincipal()
{
    return this->camara1;
}

void ModoJuego::setUserName(string userName)
{
    if (!m_userName)
    {
        m_userName = new Texto2D(userName);
        m_userName->posicionar(Vector(-0.86, 0.675,-1));
        m_userName->setCamara("principal");
        this->m_userName->setAfectaZbuffer(false);
        this->m_userName->setAfectaVista(false);
        m_userName->escalar(0.025);
        m_userName->setColor(1,1,1);
        escena->addDibujable(m_userName);
    }
    else{
        m_userName->setTexto(userName);
    }
}

void ModoJuego::Enter()
{
    SoundManager * sm = SoundManager::getInstance();
    string s_name = "Abduction_Starfleet.mp3";
    SoundState state = sm->GetSoundState(s_name);
    switch (state)
    {
    case ST_PAUSED:
        sm->Resume(s_name);
        break;
    case ST_NONE:
        sm->PlayMusic(s_name, true);
        break;
    }
    if (!m_started)
    {
        this->ini();
        this->nave_princ = Settings::getInstance()->getNave();
        setUserName(Settings::getInstance()->getUserName());
        m_started = true;
    }
    if (Engine::getInstancia()->getEscena() != this->escena)
        Engine::getInstancia()->setEscena(this->escena);
}

void ModoJuego::Leave()
{
    SoundManager::getInstance()->Pause("Abduction_Starfleet.mp3");
    if (m_isOver)
    {
        Settings::getInstance()->setNave(NULL);
        reset();
        nave_princ = 0;
        m_started = false;
        m_isOver = false;
    }
}

void ModoJuego::setModoCliente(bool m)
{
    modoCliente = m;
}

void ModoJuego::recibirPaqueteServidor(PaqueteRed* paquete)
{
    string s=paquete->getString();
    int numCliente=paquete->getInt();

    if (s=="Cliente Nuevo")
    {
        Nave* n;
        string nombreNave=paquete->getString();

        if(nombreNave=="JFighter")
            n=new NavePichi(new Objeto3d("nave.mdl","nave.tga"),Vector());
        else if(nombreNave=="Ghost")
            n=new NaveGhostM(new Objeto3d("GhostM.mdl","Fuselage2.tga"), Vector());
        else if(nombreNave=="Pesada")
            n=new NavePesada(new Objeto3d("NavePesada.mdl","NF_Coc02_bk.tga"), Vector());
        else //por defecto la clasica
            n=new NavePichi(new Objeto3d("nave.mdl","nave.tga"),Vector());


        n->setNumCliente(numCliente);

        naveClientes->add(numCliente,n);

    }
    else if (s=="Cliente Accion")
    {

        float dt=paquete->getFloat();

        Nave *n=dynamic_cast<Nave*>(this->naveClientes->getValue(numCliente));

        if (n)
        {
            n->posicionar(paquete->getVector());
            n->setVelocidad(paquete->getVector());

            if (paquete->getInt()) n->moverAdelante(dt);
            if (paquete->getInt()) n->moverAtraz(dt);
            if (paquete->getInt()) n->girarDerecha(dt);
            if (paquete->getInt()) n->girarIzquierda(dt);
            if (paquete->getInt()) n->moverIzq(dt);
            if (paquete->getInt()) n->moverDer(dt);

            float velx=paquete->getFloat();
            float vely=paquete->getFloat();

            n->rotar(velx,vely,dt);

            if (paquete->getInt()){  // click primaria
                if (n->isViva())
                    n->dispararPrimaria();
                else{
                    Nave * _newNave = dameNueva(n);
                    _newNave->setNumCliente(n->getNumCliente());
                    this->naveClientes->remove(numCliente);
                    this->naveClientes->add(numCliente, _newNave);
                    n->setDead(true);
                }
            }

            if (paquete->getInt() && n->isViva())   // click secundaria
                n->dispararSecundaria();
        }
        else
        {
            LOG(LogLevel::Error,"No existe una nave para el cliente pedido");
        }

    }

}

void ModoJuego::onClientLeave(int numCliente){
    Nave *nave=dynamic_cast<Nave*>(this->naveClientes->getValue(numCliente));
    if(nave)
        nave->setDead(true);
    this->naveClientes->remove(numCliente);
}

string ModoJuego::getNombreServidor()
{
    return "ModoJuego";
}
