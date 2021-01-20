#include "ModoMenu3D.h"

#include "CtrlJuego.h"
#include "MainMenu3D.h"


ModoMenu3D* ModoMenu3D::s_instance = NULL;

ModoMenu3D* ModoMenu3D::getInstance()
{
    if (!s_instance)
        s_instance = new ModoMenu3D();
    return s_instance;
}

ModoMenu3D::ModoMenu3D()
{
    this->m_currentMenu = MainMenu3D::getInstance();
}



void ModoMenu3D::tic()
{
    Menu * nextMenu = m_currentMenu->tic();
    if (nextMenu != m_currentMenu)
    {
        m_currentMenu->OnLeave();
        m_currentMenu = nextMenu;
        if (CtrlJuego::getInstance()->getModo() == this) /// uuuh, tenia terrible bug, cuando se cambia el modo del jeugo en un tic de menu
            m_currentMenu->OnEnter();     ///y se devuelve otro menu, se cagaba todo! quedaba la escena del menbu en vez de la del juego!!
    }
}

void ModoMenu3D::Enter()
{
    SoundManager * sm = SoundManager::getInstance();
    string s_name = m_currentMenu->getSoundName();
    SoundState state = sm->GetSoundState(s_name);
    switch (state)
    {
    case ST_PAUSED:
        sm->Resume(s_name);
        break;
    case ST_NONE:
        //sm->Play(s_name, true);
        sm->PlayMusic(s_name, true);
        break;
    default:
        break;
    }
    m_currentMenu->OnEnter();
}


void ModoMenu3D::Leave()
{
    m_currentMenu->OnLeave();
    SoundManager::getInstance()->Pause(m_currentMenu->getSoundName());
}
