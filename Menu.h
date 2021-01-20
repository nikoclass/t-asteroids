#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <string>

#include "Engine.h"
#include "MenuItem.h"
#include "EfectoRotEsc.h"


/// Superclase para los distintos menus
class Menu
{
private:
    Material m_mBack;
    Material m_mFront;
protected:

    Menu *m_owner;        // un ejemplo seria el menu de configuarcion, que oertenece al menu principal
    // de esta forma no quedan acoplados
    vector<MenuItem*> m_items;

    Camara* m_camara;
    Escena* m_escena;
    Multiobjeto3d* m_universo;
    Objeto3d* m_titulo;

    Luz
        * m_luzUno,
        * m_luzDos;

    string
        m_modeloTitulo,
        m_soundName;
    short
        m_selectedIndex,
        m_hiddenItemCount;

    // Clases derivadas inicializar para poder usar el efecto del Texto3D
    float
        * m_posXIniciales,
        * m_deltaXAlineando;

    bool
        m_rotar,
        m_started;  // Indica si ya se entro al menu por lo menos una vez

    const bool m_useEffect;

    EfectoRotEsc * m_efectoRotEsc;

    // Agregar aca los items del menu
    // Para texto se recomienda agregar cada items usando addTextItem
    virtual void iniciarItems() = 0;

    // desde Menu::onTicCheckKey() se llama a esta funcion
    //cuando se detecta que s eapreto la tecla ENTER
    // de esta forma le avisa a las clases concretas para que sepan que hacer
    virtual Menu* onEnterPressed() = 0;

    // hacer override si se quiere extender la funcionalidad
    // seguramente todos los menus le hagan override por el simple echo de que el menu debe tener animacion
    // es conveniente que se llame a Menu::onTicCheckKey() en alguna parte de la operacion para que quede consistente
    // Bastaria (y es preferible) de todas formas con hacer override de la funcion checkCustomKeys()
    virtual Menu* onTicCheckKey();
    // Se llama luego del onTicCheckKey()
    // Aca deberian las clases hijas chequear otras teclas
    virtual void checkCustomKeys(){};

    // Usar para agregar items como Texto3D
    void addTextItem(const string & text);

    // Usar para manejo customizado de cambio de indexes
    // Ej: queda seleccionado el item cero, pero yo no quiero que se resalte ese
    //entonces redefino y cambio el inidice, como hace el menu principal
    virtual void handleIndexChanged(int last){};

    /******** IMPORTANTE *********
    * Clases derivadas: La llamada a esta fucion debe ser la primera linea en el constructor
    * Esta funcion llama internamente a iniciarItems()
    *****************************/
    void Init(bool showTitle = false); // No me gusta mucho este parametro :|

public:
    Menu(Menu * owner = NULL, bool useEffect = true, std::string modeloTitulo = "Asteroids.mdl");

    virtual Menu* tic();

    Escena *getEscena(){
        return m_escena;
    }
    Camara *getCamara(){
        return m_camara;
    }
    Objeto3d *getTitulo(){
        return m_titulo;
    }
    string getSoundName(){
        return m_soundName;
    }

    // Redefinir para extender funcionalidad
    /// ¡¡¡¡¡ Pero siempre llamar a Menu::OnEnter() o Menu::OnLeave() !!!!!
    virtual void OnLeave(){}
    virtual void OnEnter();

    virtual ~Menu();

};

#endif // MENU_H_INCLUDED
