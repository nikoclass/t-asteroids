#ifndef MENUINTRO_H_INCLUDED
#define MENUINTRO_H_INCLUDED

#include "ModoMenu.h"
#include "Teclado.h"
#include "CtrlJuego.h"
#include "Opcion.h"

#include "ModoMenu3D.h"

#include <vector>

class MenuIntro : public ModoMenu{
    private:
        static MenuIntro* instance;
        MenuIntro();
        float time;
    public:
        static MenuIntro* getInstance();
        virtual ~MenuIntro();

        void inicializarMenus();
        void mytic(void);
};

#endif // MENUINTRO_H_INCLUDED
