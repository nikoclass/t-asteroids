#include "MenuItem.h"

MenuItem::MenuItem(Dibujable* objetoVisible, bool isHidden)
{
    this->m_dibujable = objetoVisible;
    m_hidden = isHidden;
    m_restaurar = 0;
}

MenuItem::~MenuItem(){
    if (m_subItems.size() > 0){
        for (vector<MenuItem*>::iterator it = m_subItems.begin(); it != m_subItems.end(); it++)
        delete (*it);
    }
    delete m_dibujable;
}

void MenuItem::setDibujable(Dibujable* _value)
{
    this->m_dibujable = _value;
}

void MenuItem::setSubItems(vector<MenuItem*> _value)
{
    this->m_subItems = _value;
}

void MenuItem::addSubItem(MenuItem* _value)
{
    this->m_subItems.push_back(_value);
}

void MenuItem::resaltar(float value)
{
    m_restaurar = value;
    m_dibujable->escalar(value);
}

void MenuItem::restaurar()
{
    if (m_restaurar){
        m_dibujable->escalar(1 / m_restaurar);
        m_restaurar = 0;
    }
}

