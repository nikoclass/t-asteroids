#include "ObserversConexion.h"


void SubjectConexion::notifyError(string message)
{
    set<ObserverConexion*>::iterator it = m_observers.begin();
    for (; it != m_observers.end(); it++)
        (*it)->onError(message);
}

void SubjectConexion::notifyOK(string message)
{
    set<ObserverConexion*>::iterator it = m_observers.begin();
    for (; it != m_observers.end(); it++)
        (*it)->onOK(message);
}

void SubjectConexion::notifyClienteLeave(int numCliente){
    set<ObserverConexion*>::iterator it = m_observers.begin();
    for (; it != m_observers.end(); it++)
        (*it)->onClientLeave(numCliente);
}

void SubjectConexion::addObserver(ObserverConexion* o)
{
    m_observers.insert(o);
}

void SubjectConexion::removeObserver(ObserverConexion* o)
{
    m_observers.erase(o);
}
