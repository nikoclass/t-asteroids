#ifndef MENUCONFIG3D_H
#define MENUCONFIG3D_H

#include "Menu.h"
#include "TextBox.h"
#include "Settings.h"


class MenuConfig3D
    : public Menu
{
public:
    static MenuConfig3D* getInstance();
    virtual ~MenuConfig3D();
    void OnEnter();
protected:
    void iniciarItems();
    Menu* onEnterPressed();
    void handleIndexChanged(int last);
    Menu* onTicCheckKey();
    void checkCustomKeys();
private:
    static MenuConfig3D* s_instance;
    MenuConfig3D();

    std::string *m_strCameras;
    int m_indexCamera;
    bool m_indexCameraChanged;
    void handleIndexCamera();
    void handleIndexText();

    TextBox
        * m_txtUser,
        * m_txtCamera,
        * m_txtGameVol,
        * m_txtMusicVol;
};

#endif // MENUCONFIG3D_H
