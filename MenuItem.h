#ifndef MENUITEM_H_INCLUDED
#define MENUITEM_H_INCLUDED

#include "Dibujable.h"
#include <vector>
using namespace std;

class MenuItem
{
private:
    /// Dibujable al que estará asociado, ya no estará atado a ser solo un Texto2D
    Dibujable* m_dibujable;
    /// Robado de Opcion, puede ser que necesite representar sub-opciones
    vector<MenuItem*> m_subItems;

    float m_restaurar;
    bool m_hidden;

public:
    MenuItem(Dibujable* objetoVisible, bool isHidden = false);
    virtual ~MenuItem();

    virtual void setDibujable(Dibujable* _value);
    void setSubItems(vector<MenuItem*> _value);

    void setHidden(bool _value){ m_hidden = _value;}
    bool getHidden() { return m_hidden; }

    /// Inserta un subItem al final de la coleccion de subitems
    void addSubItem(MenuItem* _value);

    virtual Dibujable* getDibujable() const { return m_dibujable; }

    vector<MenuItem*> getSubItems()  const { return m_subItems; }

    virtual void resaltar(float value = 1.5);
    virtual void restaurar();

};


#endif // MENUITEM_H_INCLUDED
