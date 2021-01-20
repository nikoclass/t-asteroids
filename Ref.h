#ifndef REF_H_INCLUDED
#define REF_H_INCLUDED

#include <iostream>
#include <stdexcept>

using namespace std;

///Punteros inteligentes con conteo de referencias
template<class T>
class Ref{

    private:

        struct Wrapper{ //El envoltorio
            T *objeto;
            unsigned char cantRefs;
        };

        Wrapper *wrapper;

    public:

        Ref(T *obj=NULL){ //Constructor comun
            if(obj){
                this->wrapper=new Wrapper;
                this->wrapper->cantRefs=1;
                this->wrapper->objeto=obj;
            }else
                this->wrapper=NULL;
        }

        Ref(const Ref<T> &r){ //Constructor de copia
            this->wrapper=r.wrapper;
            if(r.wrapper!=NULL)
                this->wrapper->cantRefs++; //Incremento referencias
        }


        ~Ref(){ //Destructor
            if(this->wrapper==NULL) return;

            this->wrapper->cantRefs--; //Decremento referencias

            if(this->wrapper->cantRefs==0){ //Debo destruir
                delete this->wrapper->objeto;
                delete this->wrapper;
            }
        }

        //Operadores necesarios de asignacion, igualdad, desigualdad, y existencia

        Ref& operator=(const Ref<T> &r){
            if(this->wrapper!=NULL){ //Si tengo un objeto referenciado

                if(*this==r) return *this;

                this->wrapper->cantRefs--; //Decremento referencias

                if(this->wrapper->cantRefs==0){ //Debo destruir
                    delete this->wrapper->objeto;
                    delete this->wrapper;
                }
            }

            //Ahora asigno
            this->wrapper=r.wrapper;
            if(r.wrapper!=NULL)
                this->wrapper->cantRefs++; //Incremento referencias

            return *this;

        }

        Ref& operator=(T *obj){
            if(this->wrapper!=NULL){ //Si tengo un objeto referenciado

                if(this->wrapper->objeto==obj) return *this;

                this->wrapper->cantRefs--; //Decremento referencias

                if(this->wrapper->cantRefs==0){ //Debo destruir
                    delete this->wrapper->objeto;
                    delete this->wrapper;
                }
            }

            //Ahora asigno
            if(obj){
                this->wrapper=new Wrapper;
                this->wrapper->cantRefs=1;
                this->wrapper->objeto=obj;
            }else
                this->wrapper=NULL;
            return *this;

        }

        bool operator==(const Ref<T> &r)const{
            if(this->wrapper==NULL && r.wrapper==NULL)
                return true;
            else if(this->wrapper!=NULL && r.wrapper!=NULL)
                return (this->wrapper->objeto==r.wrapper->objeto);
            else
                return false;
        }
        //bool operator==(T *obj);

        bool operator!=(const Ref<T> &r)const{
            return (this->wrapper!=r.wrapper);
        }
        //bool operator!=(T *obj);

        bool operator!()const{
            return !this->wrapper;
        }

        T* get()const{
            if(this->wrapper==NULL)
                return NULL;
            else
                return this->wrapper->objeto;
        }

        int getCantRefs()const{
            if(this->wrapper==NULL)
                return 0;
            else
                return this->wrapper->cantRefs;
        }

        T* operator->()const{
            if(this->wrapper==NULL)
                throw runtime_error("NULL");
            else
                return this->wrapper->objeto;
        }

        T& operator*()const{
            if(this->wrapper==NULL)
                throw runtime_error("NULL");
            else
                return *this->wrapper->objeto;
        }

        operator T*(){
            if(this->wrapper==NULL)
                return NULL;
            else
                return this->wrapper->objeto;
        }
};


#endif // REF_H_INCLUDED

