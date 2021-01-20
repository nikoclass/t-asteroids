#include "Thread.h"

Thread::Thread()
{
    hThread = 0;
}

void Thread::start()
{
    //Si ya esta corriendo espero a que termine
    waitForEnd();

    _threadObj = this;
    DWORD threadID;
    hThread = CreateThread(0, 0, threadProc, _threadObj, 0, &threadID);
}

bool Thread::isRunning()
{
    return hThread!=0;
}

unsigned long __stdcall Thread::threadProc(void* ptr){
    ((Thread*)ptr)->run();
    return 0;
}


void Thread::waitForEnd(){
    if(!hThread) return;

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    hThread=0;
}
