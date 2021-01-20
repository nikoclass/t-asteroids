#ifndef SECCIONCRITICA_H
#define SECCIONCRITICA_H
class SeccionCritica;

#include <windows.h>

// Clase para el manejo del problema de la seccion critica

class SeccionCritica{
    private:
        CRITICAL_SECTION seccionCritica;

    public:
        SeccionCritica();
        virtual ~SeccionCritica();

        void entrar();
        bool intentarEntrar();

        void salir();

};

#endif // SECCIONCRITICA_H
