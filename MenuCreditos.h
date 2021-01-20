#ifndef MENUCREDITOS_H_INCLUDED
#define MENUCREDITOS_H_INCLUDED


#include "Menu.h"
#include "MainMenu3D.h"

class MenuCreditos
    : public Menu
{
    private:
        static MenuCreditos* s_instance;
        MenuCreditos();

        vector<Luz*>
            m_lightsRight,
            m_lightsLeft;

    protected:
        Menu* onEnterPressed(){ return m_owner; };
        void iniciarItems();
        Menu* onTicCheckKey();
        void handleIndexChanged(int last);
    public:
        static MenuCreditos* getInstance();
        virtual ~MenuCreditos();
};

#endif // MENUCREDITOS_H_INCLUDED
