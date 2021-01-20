#ifndef MENUNAVE_H
#define MENUNAVE_H

#include "Menu.h"
#include "MainMenu3D.h"
#include "MenuJuego.h"

class MenuNave : public Menu
{
public:
    static MenuNave* getInstance();
    virtual ~MenuNave();
    void goToGame() { m_goToGame = true; }

protected:
	Menu* onTicCheckKey();
    Menu* onEnterPressed();

    void iniciarItems();
    void checkCustomKeys();
    void handleIndexChanged(int last);
    void OnLeave();

private:
    static MenuNave *s_instance;
    MenuNave();

	void SetNaveActual(int index);

    // para usar de titulo
	vector<Dibujable*> m_ships;

	bool
        m_naveInit,
        m_goToGame;

    Luz * m_otraLuz;
};

#endif // MENUNAVE_H
