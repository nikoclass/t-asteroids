#ifndef MODO_H_
#define MODO_H_


class Modo
{

public:
    Modo();
    virtual ~Modo();

    virtual void tic(void)=0;
    virtual void Enter(){}
    virtual void Leave(){}
};

#endif /*MODO_H_*/
