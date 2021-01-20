#ifndef OBSERVERSCONEXION_H_INCLUDED
#define OBSERVERSCONEXION_H_INCLUDED

#include <iostream>
#include <string>
#include <set>
using namespace std;

/// Patron observer para los mensajes de conexion

// Clase de marca para los observers
class ObserverConexion
{
public:
    ObserverConexion(){}
    virtual ~ObserverConexion(){}
    virtual void onError(std::string message){}
    virtual void onOK(std::string message){}
    virtual void onClientLeave(int numCliente){}
};


// Clase subject
class SubjectConexion
{
private:
    set<ObserverConexion*> m_observers;
protected:
    void notifyError(string message);
    void notifyOK(string message);
    void notifyClienteLeave(int numCliente);
public:
    SubjectConexion(){}
    virtual ~SubjectConexion(){}
    virtual void addObserver(ObserverConexion*);
    virtual void removeObserver(ObserverConexion*);
};


#endif // OBSERVERSCONEXION_H_INCLUDED
