#include "SeccionCritica.h"

SeccionCritica::SeccionCritica(){
    InitializeCriticalSection(&seccionCritica);
}

SeccionCritica::~SeccionCritica(){
    DeleteCriticalSection(&seccionCritica);
}

void SeccionCritica::entrar(){
    EnterCriticalSection(&seccionCritica);
}

bool SeccionCritica::intentarEntrar(){
    return TryEnterCriticalSection(&seccionCritica);
}

void SeccionCritica::salir(){
    LeaveCriticalSection(&seccionCritica);
}
