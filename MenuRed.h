#ifndef MENURED_H
#define MENURED_H

#include "Menu.h"
#include "Luz.h"
#include "TextBox.h"
#include "ControladorRed.h"
#include "Evento.h"
#include "Texto2D.h"

enum ModoMenuRed
{
    ModoServidor,
    ModoCliente
};

class MenuRed
    : public Menu, ObserverConexion
{
    public:
        static MenuRed * getInstance();
        virtual ~MenuRed();
        void setModo(ModoMenuRed);

        virtual void onError(std::string message);
        virtual void onOK(std::string message);
    protected:
        void iniciarItems();
        Menu* onTicCheckKey();
        Menu* onEnterPressed();
        void handleIndexChanged(int last);
        void OnEnter();
    private:
        MenuRed();
        static MenuRed * s_instance;

        ModoMenuRed m_modoRed;

        TextBox
            * m_txtServer,
            * m_txtPort;

        Luz * m_otraLuz;

        Evento * m_event;
        std::string m_eventData;
        Texto2D * m_eventText;
        bool m_onError;

        bool waitForConnection();
};

#endif // MENURED_H
