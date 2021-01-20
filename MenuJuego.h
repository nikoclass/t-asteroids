#ifndef MENUJUEGO_H
#define MENUJUEGO_H

#include "CtrlJuego.h"

#include "Menu.h"
#include "EfectoRotEsc.h"
#include "MainMenu3D.h"


class MenuJuego : public Menu
{
public:
    virtual ~MenuJuego();

    static MenuJuego* getInstance();
protected:
    void iniciarItems();
    Menu * onEnterPressed();
private:
    static MenuJuego* s_instance;
    MenuJuego();
};

#endif // MENUJUEGO_H
