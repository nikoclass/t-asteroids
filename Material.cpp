#include "Material.h"

Material::Material(){
    this->diffuse   = Vector(1.0,1.0,1.0);
    this->specular  = Vector(1.0,1.0,1.0);
    this->shininess = 10.0;
}
Material::Material(Vector a, Vector b, float c){
    this->diffuse   = a;
    this->specular  = b;
    this->shininess = c;
}
Material::~Material(){
}
void Material::setDiffuse(Vector v){
    this->diffuse = v;
}
void Material::setSpecular(Vector v){
    this->specular = v;
}
void Material::setEmission(Vector v){
    this->emission = v;
}
void Material::setShininess(float v){
    this->shininess = v;
}
Vector Material::getDiffuse(){
    return this->diffuse;
}
Vector Material::getSpecular(){
    return this->specular;
}
Vector Material::getEmission(){
    return this->emission;
}
float Material::getShininess(){
    return this->shininess;
}
void Material::toGL(Vector color,float opacidad){
    Escena *e=Engine::getInstancia()->getEscena();
    float fDifuso=e->getFactorDifuso();
    float fAmbiente=e->getFactorAmbiente();
    float fEspecular=e->getFactorEspecular();

    const GLfloat mat_ambient[4]    = { 0.2*fAmbiente, 0.2*fAmbiente, 0.2*fAmbiente, opacidad };
    const GLfloat mat_diffuse[4]    = { diffuse[X]*fDifuso*color[RED], diffuse[Y]*fDifuso*color[GREEN], diffuse[Z]*fDifuso*color[BLUE], opacidad };
    const GLfloat mat_specular[4]    = { specular[X]*fEspecular, specular[Y]*fEspecular, specular[Z]*fEspecular, opacidad };
    const GLfloat mat_emission[4]    = { emission[X], emission[Y], emission[Z], opacidad };
    const GLfloat high_shininess[1] = { shininess };

    //glColor4f(color[RED],color[GREEN],color[BLUE],opacidad);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  mat_emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);

}
