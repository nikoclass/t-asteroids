#include "Textura.h"


#include <stdio.h>
#include <iostream>

using namespace std;

#define CARGANDO 1
#define TERMINO_CARGA 2
#define PUEDO_MOSTRAR 3


Textura::Textura(const char *ruta,bool forzarTransparencia){
    this->nombre=string(ruta);
    string base=ManejadorTexturas::getInstancia()->getRuta();
    this->nombre.erase(0,base.size());
	bpp=largo=ancho=id=0;

    dibujo=NULL;
    cargarTGA(ruta,forzarTransparencia);


    //CargadorTexturaAsincrono *cta=new CargadorTexturaAsincrono(this,ruta,forzarTransparencia);
    //cta->start(); //Al final de este metodo, el objeto cta se destruye (de este stack)
                //pero el thread sigue corriendo y sigue el objeto en el otro stack
}

Textura::~Textura(){

}

void Textura::cargarTGA(const char *ruta,bool forzarTransparencia){

    this->estado=CARGANDO;

    GLubyte  cabezeraTGA[12]= {
                                  0,0,2,0,0,0,0,0,0,0,0,0
                              };  // Cabezera de un tga sin compresion
    GLubyte         compararTGA[12];                                        // Aca vamos a comprar la cabezera
    GLubyte  cabezera[6];      // Los 6 bytes importantes
    GLuint  bytesporpunto;      // Cuantos bytes hay por punto
    GLuint  tamanoimagen;      // Aca guardaremos el tama�o de la imagen
    GLuint  temp;       // Variable temporal, y una para usar con el for
    GLuint  tipo=GL_RGBA;      // Nuestro typo por defecto, lo veremos mas abajo



    FILE *archivo=fopen(ruta, "rb"); // Cargamos nuestro archivo en memoria
    if( archivo == NULL ||   // Existe nuestro archivo??
            fread(compararTGA,1,sizeof(compararTGA),archivo)!=sizeof(compararTGA)  ||      // Hay 12 bytes para leer??
            memcmp(cabezeraTGA,compararTGA,sizeof(compararTGA))!=0  || // Son iguales??
            fread(cabezera,1,sizeof(cabezera),archivo)!=sizeof(cabezera)) {
        if(archivo==NULL) {
        	cout << "[ERROR] Textura::cargarTGA - falta: " << ruta << endl;
            return;  // No se abrio el archivo
        }
        else {
            fclose(archivo);
            return;
        }
    }
    /* Ahora hay que leer la cabezera del archivo, para saber cuanto es el largo, ancho, y los bytes por puntos,
    para eso aca hay una ilustracion de la cabezera :
    6 bytes -> xxxxxxx xxxxxxx xxxxxxx xxxxxxx xxxxxxx xxxxxxx
        |--- Largo ---| |---Ancho-----| |-bpp-|
    El dato del largo se guarda en el cabezera[0] y cabezera[1], para leerlo hay que multiplicar cabezera[0] por 256 y  sumarselo a cabezera[1], para leer ancho y bpp es el mismo procedimiento */
    this->largo=256*cabezera[1]+ cabezera[0];
    this->ancho=256*cabezera[3]+ cabezera[2];

    /* Ahora vemos si hay datos no validos, como largo o ancho iguales menores a 0 o iguales a 0 */
    if( this->largo <= 0 || // Largo mayor que 0??
            this->ancho <= 0 || // Ancho mayor que 0??
            (cabezera[4]!=24 && cabezera[4]!=32)) { // bpp es 24 o 32?? (solo se cargan 24 y 32 bpp)
        //printf("Datos invalidos\n");
        fclose(archivo);
        return;
    }

    this->bpp=cabezera[4]; // Aca se guardan los bits por punto
    bytesporpunto=cabezera[4]/8; // Aca los bytes por punto (1 byte = 8 bits)
    tamanoimagen=this->largo * this->ancho * bytesporpunto;      // Esta es la memoria que nesecitaremos para guardar los datos de la textura

    /*Ahora reservamos espacio en memoria para nuestra textura, luego leemos la textura del archivo */
    //imagen->dibujo = (GLubyte *)malloc(tamanoimagen); // Reservamos la memoria necesaria para nuestra textura
    dibujo = new GLubyte[tamanoimagen];

    if(dibujo== NULL ||  // Se logro reservar la memoria???
            fread(dibujo, 1, tamanoimagen, archivo) != tamanoimagen ) { // Se lee, y se comprueba que lo leido es de la misma longitud que la asignada a a dibujo.
        if(dibujo != NULL) {
            //printf("Error leyendo imagen\n");
            free(dibujo);
        }
        else{
            //printf("Error asignando memoria\n");
        }
        fclose(archivo);
        return;
    }
    /* El formato tga guarda las imagenes en BGR, y opengl usa RGB,por lo cambiamos de lugares */
    for(int i=0; i< (int)tamanoimagen; i+=bytesporpunto) {
        temp=dibujo[i];    // Guardamos el primer valor
        dibujo[i] = dibujo[i + 2]; // Asignamos el nuevo primer valor
        dibujo[i + 2] = temp;   // Asignamos el ultimo valor

        if(bpp==32 && forzarTransparencia){
            char r=dibujo[i],g=dibujo[i+1],b=dibujo[i+2];
            dibujo[i+3]=(r+g+b)/3;
        }
    }

    fclose (archivo); // Cerramos el archivo


    //terminoCarga=true;
    this->estado=TERMINO_CARGA;
}

int Textura::getBpp(){
    return bpp;
}
int Textura::getLargo(){
    return largo;
}
int Textura::getAncho(){
    return ancho;
}

void Textura::toGL(){

    if(this->estado==CARGANDO){

        glDisable(GL_TEXTURE_2D);
        return;

    }else if(this->estado==TERMINO_CARGA){

        GLuint  tipo=GL_RGBA;
        /* Listo, terminamos con el codigo de carga, volvemos a opengl, ahora hay que asignarle a la textura un ID, luego  decirle a opengl cuales son el largo, el ancho y los bpp */

        glGenTextures( 1 , &this->id);    // Crea un ID para la textura, buscando un id que este vacio

        glBindTexture(GL_TEXTURE_2D, this->id);       // Seleccionamos nuestra textura
        if(this->bpp ==24)
            tipo= GL_RGB; // Si nuestra textura es de 24 bits, entonces se crea una textura rgb, sino una rgba
        else
            tipo=GL_RGBA;

        /* Ahora creamos nuestra textura, entrando el largo, ancho y tipo */
        ///glTexImage2D(GL_TEXTURE_2D, 0, tipo, this->ancho, this->largo, 0, tipo, GL_UNSIGNED_BYTE, dibujo);
        gluBuild2DMipmaps(GL_TEXTURE_2D,4,this->ancho, this->largo, tipo, GL_UNSIGNED_BYTE, dibujo);

        /* Ahora le decimos a opengl como queremos que se vea nuestra textura, MAG_FILTER es cuando la textura es mas grande que el lugar donde la asignamos,
         y MIG_FILTER, es cuando la textura es mas peque�a que el lugar donde la asignamos, GL_LINEAR es para que se vea bien tanto cerca como lejos, pero ocupa bastante procesador. Otra opcion el GL_NEARES, que ocupa menos procesador */
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        //OpenGl tiene en su memoria el dibujo, no es necesario mantenerla en el programa
        delete [] dibujo;
        dibujo=NULL;

        this->estado=PUEDO_MOSTRAR;
    }

    Escena *e=Engine::getInstancia()->getEscena();

    if(id>0 && e->getTexturas() && e->getTipoRenderizado()!=Wireframe){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,this->id);
    }else
        glDisable(GL_TEXTURE_2D);
}
string Textura::getNombre(){
	return this->nombre;
}
