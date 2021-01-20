#ifndef MAINMENU3D_H_INCLUDED
#define MAINMENU3D_H_INCLUDED

#include "Menu.h"




class MainMenu3D  /// Singleton
    : public Menu
{
private:
    static MainMenu3D *s_instance;
    MainMenu3D();

    bool
        m_gotOnGameText,
        m_gotShip;

protected:
    void iniciarItems();
    Menu* onEnterPressed();
    void checkCustomKeys();
    void handleIndexChanged(int last);

public:
    static MainMenu3D *getInstance();
    void OnEnter();
    virtual ~MainMenu3D();
};

#endif // MAINMENU3D_H_INCLUDED
