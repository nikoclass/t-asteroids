#ifndef MODOMENU3D_H_INCLUDED
#define MODOMENU3D_H_INCLUDED

#include "Modo.h"
#include "Menu.h"

/// Ahora los menus se manejaran como estados del ModoMenu, no como modos
class ModoMenu3D : public Modo
{
private:
    /// Singleton, solo va a haber un ModoMenu, el cual va a pasar por varios estados (distintos Menu)
    static  ModoMenu3D* s_instance;
    /// Menu al que se encuentra asociado
    Menu* m_currentMenu;

public:
    static ModoMenu3D* getInstance();
    ModoMenu3D();

    virtual void tic(); // override de Modo
    void Enter(); // override de Modo
    void Leave(); // override de Modo

    virtual ~ModoMenu3D(){};
};

#endif // MODOMENU3D_H_INCLUDED
