#include "Engine.h"

#include <iostream>

using namespace std;


Objeto3d *nave,*luz,*esfera;
Multiobjeto3d *universo,*tierra_luna;
Camara *cam,*cam2;
Luz *l,*l2;

bool pausa=false;

static void display(void)
{

    ///Solo para probar ahora!!
    Teclado* teclado=Teclado::getInstancia();
    Mouse* mouse=Mouse::getInstancia();
    Engine *eng=Engine::getInstancia();


    double t=eng->getTiempoTotal();

    if(pausa){
        if(teclado->tecla('p') && eng->getTimeScale()==0){
            pausa=false;
            eng->setTimeScale(1,2);
            mouse->setCursor(GLUT_CURSOR_NONE);
        }

    }else{
        if(teclado->tecla('p') && eng->getTimeScale()==1){
            eng->setTimeScale(0,0.5);
            pausa=true;
            mouse->setCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    if(teclado->tecla(27))
        exit(0);

    //float dtr=eng->getDtReal();
    float dt=eng->getDt();

    if(teclado->tecla('w') or teclado->tecla(GLUT_KEY_UP))
        nave->trasladar(-4*nave->getEjeZ()*dt);

    if(teclado->tecla('a')){
        nave->rotar(dt,nave->getEjeZ());
    }

    if(teclado->tecla('s') or teclado->tecla(GLUT_KEY_DOWN))
        nave->trasladar(4*nave->getEjeZ()*dt);

    if(teclado->tecla('d')){
        nave->rotar(-dt,nave->getEjeZ());
    }

    Vector x=nave->getEjeX();
    Vector y=nave->getEjeY();
    nave->rotar(MantenerEntre(-4,-mouse->getVelX(),4)*dt,y);
    nave->rotar(MantenerEntre(-2,-mouse->getVelY(),2)*dt,x);


    Vector posCamara(0,2,5);
    posCamara=nave->getTransformacion()*posCamara;
    Vector dif=2*dt*(posCamara-cam->getPosicion());
    //float norma=dif.norma();
    /*if(norma>0.1)
        dif=dif/norma*0.1;
*/

    cam->setPosicion(cam->getPosicion()+dif);
    cam->mirarHacia(nave->getPosicion());
    cam->setUp(nave->getEjeY());


    l2->setPos(cam->getPosicion());

    universo->rotar(dt/5,Vector(1,1,0));


    tierra_luna->getObjeto("tierra")->rotar(dt/4,Vector(0,1,0));
    tierra_luna->getObjeto("luna")->rotar(dt/3,Vector(1,1,0));
    tierra_luna->getObjeto("luna")->rotar(dt/6,Vector(0,1,0),true);


    double ts=eng->getTimeScale();
    ts*=ts*ts*ts;

    luz->identidad();
    luz->escalar(Vector(3*ts,2*ts,6));
    luz->trasladar(Vector(0,0.2,-1.5));
    luz->aplicarTransformacion(nave->getTransformacion());

    luz->setOpacidad(ts);

    l->setPos(Vector(cos(t)*50,0,sin(t)*50));


    if(!pausa)
        mouse->setPos(400,300);

}


int mainx(int argc, char *argv[])
{
    Engine *engine=Engine::getInstancia();
    engine->inicializar(argc,argv,"Asteroids 3D");

    Escena *escena=engine->getEscena();

    cam=new Camara("principal");
    cam2=new Camara("secundaria");

    cam2->setAnchoAlto(0.3,0.3);
    cam2->setXY(0.7,0);
    //cam2->setFov(30);

    escena->addCamara(cam);
    escena->addCamara(cam2);


    nave=new Objeto3d("nave.mdl","nave.tga");
    nave->posicionar(Vector(0,0,-4));
    nave->getMaterial().setDiffuse(Vector(1,1,1));
    nave->getMaterial().setSpecular(Vector(1,1,1));

    //nave->setDibujarEsferaEnvolvente(true);

    //nave->setModoGeneracionCoordTextura(Enviromental);
    //nave->setModoTextura(Reemplazar);


    Objeto3d *o;

    luz=new Objeto3d("luz2.mdl","luz2.tga");
    luz->setAfectaLuces(false);
    luz->setModoTransparencia(Aditivo);
    luz->setCullFace(false);
    luz->setColor(1,1,0.9);


    l=new Luz();
    l->setColor(Vector(1,1,0.9));
    //l->setPos(Vector(10,0,0));

    l2=new Luz();
    l2->setColor(Vector(0,1,0));
    l2->setDistanciaMaxima(2);

    escena->addLuz(l);
    escena->addLuz(l2);



    universo=new Multiobjeto3d("universo.mdl");

    o=((Objeto3d *)universo->getObjeto("frente"));
    o->cargarTextura("sp07_ft.tga");
    o->setModoTextura(Reemplazar);

    o=((Objeto3d *)universo->getObjeto("abajo"));
    o->cargarTextura("sp07_dn.tga");
    o->setModoTextura(Reemplazar);

    o=((Objeto3d *)universo->getObjeto("arriba"));
    o->cargarTextura("sp07_up.tga");
    o->setModoTextura(Reemplazar);

    o=((Objeto3d *)universo->getObjeto("atras"));
    o->cargarTextura("sp07_bk.tga");
    o->setModoTextura(Reemplazar);

    o=((Objeto3d *)universo->getObjeto("derecha"));
    o->cargarTextura("sp07_rt.tga");
    o->setModoTextura(Reemplazar);

    o=((Objeto3d *)universo->getObjeto("izquierda"));
    o->cargarTextura("sp07_lf.tga");
    o->setModoTextura(Reemplazar);

    universo->escalar(30);
    universo->setAfectaLuces(false);


    tierra_luna=new Multiobjeto3d("tierra_luna.mdl",true);
    o=((Objeto3d *)tierra_luna->getObjeto("tierra"));
    o->cargarTextura("tierra.tga");

    o=((Objeto3d *)tierra_luna->getObjeto("luna"));
    o->cargarTextura("luna.tga");

    o->posicionar(Vector(3,0,0));

    tierra_luna->escalar(3);
    tierra_luna->posicionar(Vector(1,-1,-10));
    //tierra_luna->setDibujarEsferaEnvolvente(true);

    escena->addDibujable(universo);
    escena->addDibujable(nave);
    escena->addDibujable(tierra_luna);
    escena->addDibujable(luz);

    //escena->setLuzAmbiente(Vector(0,0,0));

    Mouse::getInstancia()->setCursor(GLUT_CURSOR_NONE);

    engine->loop(display);


    return EXIT_SUCCESS;
}
