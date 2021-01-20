#include "PrototiposClientes.h"

///Incluir aqui todos los cabezales de objetos clientes

#include "ClienteBalaComun.h"
#include "ClienteNavePichi.h"
#include "ClienteMisil.h"
#include "ClienteAsteroide.h"
#include "ClienteModoJuego.h"
#include "ClienteChat.h"
#include "ClienteNavePesada.h"
#include "ClienteNaveGhost.h"
#include "ClientePoder.h"

void registrarPrototipos(){
    ControladorRed* ctrl=ControladorRed::getInstancia();

    ///Incluir aqui todas las inicializaciones

//NAVES Y OTROS COLISIONABLES IMPORTANTES
    ctrl->registrarPrototipo(new ClienteNavePesada());
    ctrl->registrarPrototipo(new ClienteNavePichi());
    ctrl->registrarPrototipo(new ClienteAsteroide());
    ctrl->registrarPrototipo(new ClienteNaveGhost());
//PODERES Y MUNICIONES
    ctrl->registrarPrototipo(new ClienteBalaComun());
    ctrl->registrarPrototipo(new ClienteMisil());
    ctrl->registrarPrototipo(new ClientePoder());
//OTROS
    ctrl->registrarPrototipo(new ClienteModoJuego());
    ctrl->registrarPrototipo(new ClienteChat());




}
