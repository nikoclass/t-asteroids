#include "Malla.h"

#include <math.h>

Malla::Malla(FILE *f,bool optimizarVertices){
    cantVertices=0;
    cantTriangulos=0;
    nombre="";
    cargarMalla(f,optimizarVertices);
}

void Malla::destruirMalla(){
    if(cantVertices>0){
        delete[] vertices;
        delete[] normales;
        delete[] coordTextura;
        vertices=NULL;
        normales=NULL;
        coordTextura=NULL;
        cantVertices=0;
    }

    if(cantTriangulos>0){
        delete[] triangulos;
        triangulos=NULL;
        cantTriangulos=0;
    }

}


int Malla::verificarSingularidad(Vector *vertices,Vector *coordText,int vertice,int &Indice,ArbolVertices* &A,double tolerancia){
//Devuelve el indice del vertice creado si no existia, y lo ingresa al arbol
//Sino devuelve el numero de vertice nuevo al que corresponde la repeticion

    if(!A){ //El arbol es vacio, debo crear uno y agregar el vertice
        A=new ArbolVertices;
        A->IndiceViejo=vertice;
        A->IndiceNuevo=Indice;

        for(int i=0;i<27;i++)
            A->hijos[i]=NULL;

        Indice++;
        return Indice-1;

    }
    else{ //Debo investigar si son iguales o sino a la rama a la que debe ir
        int Rama=0,Exponente=1;

        //if (!(tolerancia>0 && Distancia2(vertices[vertice],vertices[A->IndiceViejo])<tolerancia)){
        if (!(tolerancia>0 && vertices[vertice].distancia2(vertices[A->IndiceViejo])<tolerancia)){

            //por cada coordenada realiza las comparaciones necesarias
            for(int i=0;i<3;i++){
                //Si la coordenada [i] del vertice es menor que la del nodo del arbol
                if(vertices[vertice][i]<vertices[A->IndiceViejo][i])
                    Rama+=Exponente;
                else if(vertices[vertice][i]>vertices[A->IndiceViejo][i])
                    Rama+=2*Exponente;

                Exponente*=3;
            }
        }

        if(Rama==0 && coordText[vertice].distancia2(coordText[A->IndiceViejo])<tolerancia)
            return A->IndiceNuevo;
        else
            return verificarSingularidad(vertices,coordText,vertice,Indice,A->hijos[Rama],tolerancia);

    }
}


void Malla::pasarArbolAArrayYDestruir(ArbolVertices* &A,Vector *viejos,Vector *nuevos,Vector *viejasCoordText,Vector *nuevasCoordText){

    if(!A)//no existe arbol, salir
        return;
    else{
        for(int i=0;i<27;i++)
            pasarArbolAArrayYDestruir(A->hijos[i],viejos,nuevos,viejasCoordText,nuevasCoordText);

        nuevos[A->IndiceNuevo]=viejos[A->IndiceViejo];
        nuevasCoordText[A->IndiceNuevo]=viejasCoordText[A->IndiceViejo];

        delete A;
    }
}


void Malla::optimizarVertices(float tolerancia,int &cantVertices,int cantTriangulos,Vector* &Vertices,Vector* &CoordText,Triangulo* Triangulos){

    //Reorganiza la estructura de caras y vertices para eliminar
    //aquellos vertices que se encuentran exactamente en el mismo punto

    int nueva_cant_vertices=0,i,cont;
    ArbolVertices *Singulares=NULL;

    tolerancia*=tolerancia;//La elevo al cuadrado para facilitar los calculos

    for(cont=0;cont<cantTriangulos;cont++) //Para cada triangulo del objeto
        for(i=0;i<3;i++) //Cada triangulo tiene 3 vertices, entonces procesar a los 3
            Triangulos[cont].t[i]=verificarSingularidad(Vertices,CoordText,Triangulos[cont].t[i],nueva_cant_vertices,Singulares,tolerancia);

    Vector *nuevos_puntos=new Vector[nueva_cant_vertices];
    Vector *nuevas_coordText=new Vector[nueva_cant_vertices];

    pasarArbolAArrayYDestruir(Singulares,Vertices,nuevos_puntos,CoordText,nuevas_coordText);

    delete[] Vertices;
    delete[] CoordText;

    cantVertices=nueva_cant_vertices;
    Vertices=nuevos_puntos;
    CoordText=nuevas_coordText;

}




int Malla::moverseHasta(FILE* f,const char c){
//Mueve el puntero de archivo hasta la posicion posterior a la primera
//ocurrencia del caracter c y devuelve true en caso de encontrarlo
//De lo contrario se posiciona en donde empezo la busqueda
    fpos_t pos;
    fgetpos(f,&pos);

    while (!feof(f) && fgetc(f)!=c);

    if (feof(f)){
        fsetpos(f,&pos);
        return 0;
    }else
        return 1;

}

int Malla::moverseHasta(FILE* f,const char* c){
//Sobrecarga de esta ultima, pero aceptando strings
    fpos_t pos,pos2;
    fgetpos(f,&pos);

    int len=strlen(c),i;

    while(!feof(f)){ //Mientras no sea el fin
        fgetpos(f,&pos2);
        i=0;
        while(!feof(f) && i<len && fgetc(f)==c[i]){//Carga hasta len caracteres desde el archivo
            i++;
        }
        if (feof(f)){ //Si llega al final, reposiciona y devuelve false
            fsetpos(f,&pos);
            return false;
        }else if (i==len)//Si encontro, devuelve true
            return true;
        else    //Sino vuelve a la posicion antes de entrar al boocle
            fsetpos(f,&pos2);

        fgetc(f); //Saltea uno para recomenzar el boocle con un caracter nuevo
    }
    return false;
}

int Malla::saltearLinea(FILE *f,int veces){
//Posiciona el puntero de archivo inmediatamente despues
//del primer enter
    char c=fgetc(f);

    while (!feof(f) && veces>0){
        c=0;
        while (!feof(f) && c!='\n')
            c=fgetc(f);
        veces--;
    }

    return !feof(f);
}

void Malla::calcularNormales(int cantVertices,int cantTriangulos,Vector *Vertices,Triangulo *Triangulos,Vector *Normales){
    int i,j,k;
    for(i=0;i<cantTriangulos;i++){ //Por cada triangulo
        for(j=0;j<3;j++){ //por cada vertices del triangulo
            k=Triangulos[i].t[j];
            Normales[k]=Normales[k]+Triangulos[i].normal;  //Sumo la normal del triangulo
        }
    }
    for(i=0;i<cantVertices;i++) //Normalizo todas las normales
        Normales[i].normalizar();

}

void Malla::cargarMalla(FILE *f,bool optimizar){

    //Leo nombre del Malla
    char nombre[30];
    fscanf(f,"%s",nombre);
    this->nombre=nombre;

    this->nombre=this->nombre.substr(1,this->nombre.size()-2); //Para quitarle las comillas

    saltearLinea(f);

    //Leo cantidad de vertices
    fscanf(f,"%d",&cantVertices);

    Vector *Vertices=new Vector[cantVertices];
    Vector *CoordText=new Vector[cantVertices];

    float x,y,z,u,v;
    int temp,i;

    for (i = 0; i<cantVertices; i++) {

        fscanf(f,"%d %f %f %f %f %f",&temp,&x,&y,&z,&u,&v);

        Vertices[i][X]=x;
        Vertices[i][Y]=y;
        Vertices[i][Z]=z;

        CoordText[i][U]=u;
        CoordText[i][V]=v;

        saltearLinea(f);
    }

    //Leo cantidad de triangulos y cargo las normales a las caras
    fscanf(f,"%d",&cantTriangulos);
    Triangulo *Triangulos=new Triangulo[cantTriangulos];

    for (i = 0; i<cantTriangulos; i++){
        fscanf(f, "%f %f %f", &x,&y,&z);
        Triangulos[i].normal[X]=x;
        Triangulos[i].normal[Y]=y;
        Triangulos[i].normal[Z]=z;
    }

    //Cargo los indices de los vertices
    for (i = 0; i<cantTriangulos; i++){
        saltearLinea(f);
        fscanf(f, "%d %d %d %d",&temp,&Triangulos[i].t[0],&Triangulos[i].t[1],&Triangulos[i].t[2]);
    }

    saltearLinea(f);


    //Termine de leer el archivo, ahora realizo optimizaciones, calculo de normales, etc
    if(optimizar){
        optimizarVertices(0.001,cantVertices,cantTriangulos,Vertices,CoordText,Triangulos);
    }

    Vector *Normales=new Vector[cantVertices];

    calcularNormales(cantVertices,cantTriangulos,Vertices,Triangulos,Normales);

    //Ahora paso a la memoria del objeto
    this->vertices=new float[3*cantVertices];
    this->normales=new float[3*cantVertices];
    this->coordTextura=new float[2*cantVertices];

    Vector min(9999,9999,9999),max=-min;

    int cont=0,cont2=0;
    for(int i=0;i<cantVertices;i++){
        this->vertices[cont]=Vertices[i][X];
        this->normales[cont]=Normales[i][X];
        cont++;
        this->vertices[cont]=Vertices[i][Y];
        this->normales[cont]=Normales[i][Y];
        cont++;
        this->vertices[cont]=Vertices[i][Z];
        this->normales[cont]=Normales[i][Z];
        cont++;

        this->coordTextura[cont2]=CoordText[i][U];cont2++;
        this->coordTextura[cont2]=CoordText[i][V];cont2++;

        min=Min(min,Vertices[i]);
        max=Max(max,Vertices[i]);
    }

    Vector centro=(min+max)/2;
    float dist=0;
    for(int i=0;i<cantVertices;i++)
        dist=Max(dist,centro.distancia2(Vertices[i]));

    esferaEnvolvente=Esfera(centro,sqrt(dist));

    this->triangulos=new unsigned int[3*cantTriangulos];
    cont=0;
    for(int i=0;i<cantTriangulos;i++){
        this->triangulos[cont]=Triangulos[i].t[0];cont++;
        this->triangulos[cont]=Triangulos[i].t[1];cont++;
        this->triangulos[cont]=Triangulos[i].t[2];cont++;
    }

    delete[] Vertices;
    delete[] Normales;
    delete[] CoordText;
    delete[] Triangulos;

}

string Malla::getNombre(){
    return nombre;
}

Esfera Malla::getEsferaEnvolvente(){
    return esferaEnvolvente;
}

void Malla::toGL(){
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT,0,normales);
    glTexCoordPointer(2,GL_FLOAT,0,coordTextura);

    glDrawElements(GL_TRIANGLES, cantTriangulos*3, GL_UNSIGNED_INT, triangulos);

}

Malla::~Malla(){
    destruirMalla();
}
