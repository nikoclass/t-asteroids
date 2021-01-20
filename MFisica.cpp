#include "MFisica.h"

MFisica* MFisica::instance=NULL;
MFisica::MFisica(){
    colisionar=true;
    mutex=new SeccionCritica();
}
MFisica* MFisica::getInstance(){
	if(!instance)MFisica::instance=new MFisica;
	return MFisica::instance;
}
MFisica::~MFisica(){

}
void MFisica::addColisionable(Colisionable*in){
    mutex->entrar();
	this->colisionables.push_back(in);
	mutex->salir();
}
void MFisica::removeColisionable(Colisionable*out){
    mutex->entrar();
	for (unsigned int i=0;i<this->colisionables.size();i++){
		if( this->colisionables[i] == out){
			this->colisionables.erase(this->colisionables.begin()+i);

		}
	}
	mutex->salir();
}

void MFisica::ticColisionables(float dt){

	vector<Colisionable*>aux=this->colisionables;
	this->colisionables.clear();
	for (unsigned int i=0;i<aux.size();i++){

		if(aux[i]->isDead()){ //puede morir despues de un bang
		     //delete aux[i]; //Esto es una inmundicia pero es lo que hay por ahora
		     aux[i]->~Colisionable();
		}
		else {
		    aux[i]->tic(dt);

		    if(aux[i]->isDead()){ //o puede morir despues de un tic
                //delete aux[i]; ///ojo con el snapshot que el cliente recibe antes del paquete del tic!!
                aux[i]->~Colisionable();
            }else{

                this->colisionables.push_back(aux[i]);

                if(colisionar){
                    Esfera esfera=aux[i]->getEsfera();

                    for(unsigned int j=i+1;j<aux.size();j++){
                        if(!aux[j]->isDead() && aux[j]->getEsfera().intersecta(esfera)){
                            aux[i]->bang(aux[j]);
                            aux[j]->bang(aux[i]);
                        }
                    }
                }
            }
		}
	}

}
void MFisica::addAnimable(Animable*in){
    mutex->entrar();
	this->animables.push_back(in);
	mutex->salir();
}

void MFisica::removeAnimable(Animable*out){
    mutex->entrar();
	for (unsigned int i=0;i<this->animables.size();i++){
		if( this->animables[i] == out){
			this->animables.erase(this->animables.begin()+i);
		}
	}
	mutex->salir();
}

void MFisica::ticAnimables(float dt){
	vector<Animable*>aux=this->animables;
	this->animables.clear();
	for (unsigned int i=0;i<aux.size();i++){
		if(aux[i]->isDead()) delete aux[i];
		else {
			this->animables.push_back(aux[i]);
			aux[i]->tic(dt);
		}
	}
}

void MFisica::tic(float dt){
    mutex->entrar();

	//cout << "MFisica::tic(float dt) begin" << endl;
	this->ticAnimables(dt);
	//cout << "MFisica::tic(float dt) middle" << endl;
	this->ticColisionables(dt);
	//cout << "MFisica::tic(float dt) end" << endl;

    mutex->salir();
}

vector<Colisionable*> MFisica::getColisionablesByCondition(Condition* cond)
{
    mutex->entrar();

    vector<Colisionable*> ret;
    unsigned int c_size = colisionables.size();
    for (unsigned int i = 0; i < c_size; i++)
        if (cond->cumple(colisionables[i]))
            ret.push_back(colisionables[i]);

    mutex->salir();

    return ret;
}

bool MFisica::existeColsionable(Colisionable*col){
    mutex->entrar();

    unsigned int c_size = colisionables.size();
    unsigned int i = 0;
    while (i < c_size && col != colisionables[i])
        i++;

    mutex->salir();
    return i< c_size;
}

void MFisica::setColisionar(bool c){
    colisionar=c;
}
