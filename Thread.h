
#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

class Thread
{
public:

//hacer override de este metodo
    virtual void run() = 0;


    Thread();

    void start();

///Deshabilitada por cuestiones de seguridad
    /*void stop()
    {
       if (hThread)
           TerminateThread (hThread, 0);
    }*/

    /*
    WaitForSingleObject(hThread, Timeout) //INFINITE (dentro del waitforend)
    CloseHandle(hThread)
    thread->waitforend();
    */

    void waitForEnd();

    bool isRunning();

protected:
    static unsigned long __stdcall threadProc(void* ptr);

    Thread *_threadObj;
    HANDLE    hThread;
};

#endif

