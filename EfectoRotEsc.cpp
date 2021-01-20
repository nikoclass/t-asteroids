#include "EfectoRotEsc.h"

struct DXPair
{
    Texto3D* text;
    float deltaX;
};


EfectoRotEsc::EfectoRotEsc(const Material  & materialFront, const Material  & materialBack,float highLightSize, int fractionCount, float totalRotationAngle)
{
    this->m_highLightSize = highLightSize;
    this->m_fractionCount = fractionCount;
    this->m_rotationAngle = totalRotationAngle;

    this->m_rotationFraction = m_rotationAngle / fractionCount;

    // inicio mi vector de factor de escalado
    m_highLightFractions = new float[m_fractionCount];
    // calculo la fraccion lineal de escalado
    float diffFraction =  (m_highLightSize - 1) / m_fractionCount;
    // el "valor" que tendria el escalado en cada paso
    float hlFractionValue = 1 + diffFraction;
    // paso inicial para el escalado
    m_highLightFractions[0] = hlFractionValue;
    for (int i = 1; i < m_fractionCount; i++)
    {
        m_highLightFractions[i] = m_highLightFractions[i-1] + diffFraction;
    }

    m_currentItem = 0;
    m_lastItem = 0;
    m_restoringIndex  = -1;
    m_currentIndex = -1;
    m_materialFront = materialFront;
    m_materialBack = materialBack;
}

EfectoRotEsc::~EfectoRotEsc()
{
    delete [] m_highLightFractions;
}

void EfectoRotEsc::setCurrent(Texto3D* dib, float deltaX)
{
    if (m_currentItem && m_currentItem->text == dib)
        return;
    if (m_lastItem) // cuando va muy rapido restaura el mas viejo de una
    {
        Texto3D * text = m_lastItem->text;
        while (m_restoringIndex >= 0)
        {
            text->rotar(-m_rotationFraction, Vector(1,0,0), false);
            text->setEscala(m_highLightFractions[m_restoringIndex]);
            if (m_lastItem->deltaX){
                text->trasladar(Vector(-m_lastItem->deltaX, 0, 0));
            }
            m_restoringIndex -= 1;
        }

        int len = text->getTexto().size();
        for(int i = 0; i < len; i++)
            text->getCaracter(i)->setMaterial(m_materialBack);
        delete m_lastItem;
    }

    m_lastItem = m_currentItem;

    if (m_lastItem) // restaura el material del elemento anterior (la primera vez no hay)
    {
        Texto3D* text = m_lastItem->text;
        int len = text->getTexto().size();
        for(int i = 0; i < len; i++)
            text->getCaracter(i)->setMaterial(m_materialBack) ;
    }

    m_restoringIndex = m_currentIndex - 1;
    m_currentIndex = 0;
    m_currentItem = new DXPair;
    m_currentItem->text = dib;
    m_currentItem->deltaX = deltaX / m_fractionCount;

    // seteo el material para el elemento actual
    int len = dib->getTexto().size();
    for(int i = 0; i < len; i++)
        dib->getCaracter(i)->setMaterial(m_materialFront);
}

void EfectoRotEsc::tic()
{
    if (m_currentItem)
    {
        if (m_currentIndex < m_fractionCount)
        {
            m_currentItem->text->rotar(m_rotationFraction, Vector(1,0,0), false);
            m_currentItem->text->setEscala(m_highLightFractions[m_currentIndex]);
            if (m_currentItem->deltaX){
                m_currentItem->text->trasladar(Vector(m_currentItem->deltaX, 0, 0));
            }
            m_currentIndex += 1;
        }
        int len = m_currentItem->text->getTexto().size();
        Objeto3d *letra;
        Vector pos;
        float tt=Engine::getInstancia()->getTiempoTotal();

        for(int i = 0;i < len; i++){
            letra = m_currentItem->text->getCaracter(i);
            pos = letra->getPosicion();
            pos[Z] = cos(tt*4-i*0.5)*0.2;
            letra->posicionar(pos);
        }
    }
    if (m_lastItem && m_restoringIndex >= 0)
    {
        m_lastItem->text->rotar(-m_rotationFraction, Vector(1,0,0), false);
        m_lastItem->text->setEscala(m_highLightFractions[m_restoringIndex]);
        if (m_lastItem->deltaX){
            m_lastItem->text->trasladar(Vector(-m_lastItem->deltaX, 0, 0));
        }
        m_restoringIndex -= 1;
    }
}

void EfectoRotEsc::setMaterialFront(const Material& m)
{
    m_materialFront = m;
}

void EfectoRotEsc::setMaterialBack(const Material& m)
{
    m_materialBack = m;
}

