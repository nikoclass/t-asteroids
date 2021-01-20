#include "PaqueteRed.h"

#include "Logger.h"

PaqueteRed::PaqueteRed(const char *datos,int tamanio){

    if(tamanio>0){
        reservado=tamanio;
        this->tamanio=tamanio;
    }else{
        reservado=10;
        this->tamanio=tamanio=0;
    }

    this->datos=new char[reservado];
    memcpy(this->datos,datos,tamanio);
    posicion=0;
}

PaqueteRed::~PaqueteRed(){
    delete[] datos;
}

char* PaqueteRed::getDatos(){
    return datos;
}
int PaqueteRed::getTamanio(){
    return tamanio;
}

void PaqueteRed::reiniciarLectura(){
    posicion=0;
}

void PaqueteRed::verificarReservado(int tamanio_agregado){
    if(tamanio+tamanio_agregado+1>=reservado){
        reservado=2*(tamanio+tamanio_agregado+1);
        datos=(char*)realloc(datos,reservado);
    }
}

void PaqueteRed::addInt(int i){
    verificarReservado(sizeof(int));
    datos[tamanio]=tipo_int;
    tamanio++;

    memcpy(&datos[tamanio],&i,sizeof(int));
    tamanio+=sizeof(int);
}

void PaqueteRed::addFloat(float f){
    verificarReservado(sizeof(float));
    datos[tamanio]=tipo_float;
    tamanio++;

    memcpy(&datos[tamanio],&f,sizeof(float));
    tamanio+=sizeof(float);
}

void PaqueteRed::addVector(Vector v){
    verificarReservado(sizeof(float)*3);
    datos[tamanio]=tipo_vector;
    tamanio++;

    float f;

    f=v[X];
    memcpy(&datos[tamanio],&f,sizeof(float));
    tamanio+=sizeof(float);

    f=v[Y];
    memcpy(&datos[tamanio],&f,sizeof(float));
    tamanio+=sizeof(float);

    f=v[Z];
    memcpy(&datos[tamanio],&f,sizeof(float));
    tamanio+=sizeof(float);
}

void PaqueteRed::addString(string s){
    s=s.substr(0,250); //lo trunco
    unsigned char tam_string=s.size();
    verificarReservado(1+tam_string); //+1 porque se guarda el largo del string

    datos[tamanio]=tipo_string;
    tamanio++;
    datos[tamanio]=tam_string;
    tamanio++;

    memcpy(&datos[tamanio],s.data(),tam_string);
    tamanio+=tam_string;
}

void PaqueteRed::addPaquete(PaqueteRed* p){
    int tam_paquete=p->getTamanio();
    verificarReservado(tam_paquete+4); //+4 porque guarda el tamaño del paquete

    datos[tamanio]=tipo_paquete;
    tamanio++;

    memcpy(&datos[tamanio],&tam_paquete,sizeof(int));
    tamanio+=sizeof(int);

    memcpy(&datos[tamanio],p->getDatos(),tam_paquete);
    tamanio+=tam_paquete;
}


int PaqueteRed::getInt(){

    if(!tipoBien(tipo_int)) return 0;

    int res;
    memcpy(&res,&datos[posicion],sizeof(int));
    posicion+=sizeof(int);

    return res;
}

float PaqueteRed::getFloat(){

    if(!tipoBien(tipo_float)) return 0.0;

    float res;
    memcpy(&res,&datos[posicion],sizeof(float));
    posicion+=sizeof(float);

    return res;
}

Vector PaqueteRed::getVector(){

    if(!tipoBien(tipo_vector)) return Vector();

    float x,y,z;

    memcpy(&x,&datos[posicion],sizeof(float));
    posicion+=sizeof(float);

    memcpy(&y,&datos[posicion],sizeof(float));
    posicion+=sizeof(float);

    memcpy(&z,&datos[posicion],sizeof(float));
    posicion+=sizeof(float);

    return Vector(x,y,z);
}
string PaqueteRed::getString(){

    if(!tipoBien(tipo_string)) return "";

    unsigned char tam_string=datos[posicion];
    posicion++;

    string res(&datos[posicion],tam_string);
    posicion+=tam_string;

    return res;
}

PaqueteRed* PaqueteRed::getPaquete(){
    if(!tipoBien(tipo_paquete)) return new PaqueteRed();

    int tam_paquete;
    memcpy(&tam_paquete,&datos[posicion],sizeof(int));
    posicion+=sizeof(int);

    PaqueteRed *res=new PaqueteRed(&datos[posicion],tam_paquete);
    posicion+=tam_paquete;

    return res;
}

TiposDatosPaquete PaqueteRed::getTipoProximoDato(){
    if(posicion>=tamanio)
        return tipo_invalido;

    return (TiposDatosPaquete)datos[posicion];
}

bool PaqueteRed::eof(){
    return (posicion>=tamanio);
}

bool PaqueteRed::tipoBien(TiposDatosPaquete tipoEsperado){

    if(posicion>=tamanio){
        cout<<"Error en PaqueteRed: se esperaba "<<tipo2string(tipoEsperado)<<" pero se llego al final del paquete"<<endl;
        return false;
    }

    TiposDatosPaquete tipoRecibido=(TiposDatosPaquete)datos[posicion];

    if(tipoRecibido!=tipoEsperado){
        cout<<"Error en PaqueteRed: se esperaba "<<tipo2string(tipoEsperado)<<" pero se recibio "<<tipo2string(tipoRecibido)<<endl;
        return false;
    }else{
        posicion++;
        return true;
    }
}

char* PaqueteRed::tipo2string(TiposDatosPaquete tipo){
    switch(tipo){
        case tipo_int: return "Int";
        case tipo_float: return "Float";
        case tipo_vector: return "Vector";
        case tipo_string: return "String";
        case tipo_paquete: return "PaqueteRed";
        default: return "Tipo desconocido";
    }
}


void PaqueteRed::mostrarContenido(int nivel){
    TiposDatosPaquete td=tipo_invalido;
    int i;
    Vector v;
    PaqueteRed *p;

    this->reiniciarLectura();

    for(i=0;i<nivel;i++) cout<<"\t";

    cout<<"{"<<endl;

    while(!this->eof()){

        td=this->getTipoProximoDato();

        if(td!=tipo_paquete)
            for(i=0;i<nivel;i++) cout<<"\t";


        if(td==tipo_int)
            cout<<"Int="<<this->getInt()<<endl;
        else if(td==tipo_float)
            cout<<"Float="<<this->getFloat()<<endl;
        else if(td==tipo_string)
            cout<<"String="<<this->getString()<<endl;
        else if(td==tipo_vector){
            v=this->getVector();
            cout<<"Vector=("<<v[X]<<","<<v[Y]<<","<<v[Z]<<")"<<endl;
        }else if(td==tipo_paquete){
            p=this->getPaquete();
            p->mostrarContenido(nivel+1);
            delete p;
        }else
            cout<<"Tipo Invalido"<<endl;

    }

    for(i=0;i<nivel;i++) cout<<"\t";
    cout<<"}"<<endl;

    this->reiniciarLectura();
}

void PaqueteRed::mostrarContenido(){
    mostrarContenido(0);
}

bool PaqueteRed::chequearConsistencia(){
    TiposDatosPaquete td=tipo_invalido;
    PaqueteRed *p;
    bool consistente;

    this->reiniciarLectura();


    while(!this->eof()){

        td=this->getTipoProximoDato();


        if(td==tipo_int)
            this->getInt();
        else if(td==tipo_float)
            this->getFloat();
        else if(td==tipo_string)
            this->getString();
        else if(td==tipo_vector)
                this->getVector();
        else if(td==tipo_paquete){
            p=this->getPaquete();
            consistente=p->chequearConsistencia();
            delete p;

            if(!consistente)
                return false;

        }else
            return false;

    }

    this->reiniciarLectura();

    return true;
}
