#ifndef SERVIDORCHAT_H
#define SERVIDORCHAT_H
class ServidorChat;


#include "ServidorRed.h"

class ServidorChat : public ServidorRed
{

    private:

    public:
        ServidorChat();
        virtual ~ServidorChat();

        void recibirPaqueteServidor(PaqueteRed* paquete);
        string getNombreServidor();

};

#endif // SERVIDORCHAT_H
