#ifndef EFECTOROTESC_H
#define EFECTOROTESC_H

#include "Dibujable.h"
#include "Texto3D.h"

/********************************************
* Clase que encapsulara los efectos de
* escalado y rotacion de los items del menu
********************************************/

#ifndef PI
    #include "Matematicas.h"
#endif

#define ERE_DEFAULT_HIGHLIGHT_SIZE 1.8
#define ERE_DEFAULT_FRACTION_COUNT 50
#define ERE_DEFAULT_ROTATION_ANGLE 2*PI

struct DXPair;

class EfectoRotEsc
{
    public:
        // Constructor de la clase, cada menu deberá tener una instancia asociada
        // Param: highLightSize: Escalado total de los Dibujables
        // Param: fractionCount: Cantidad de pasos en los que se quiere ejecutar el efecto
        // Param: totalRotationAngle: Angulo total de rotacion (Una vuelta completa por lo general)
        EfectoRotEsc(const Material  & materialFront = Material(),
                     const Material  & materialBack = Material(),
                     float highLightSize      = ERE_DEFAULT_HIGHLIGHT_SIZE,
                     int   fractionCount      = ERE_DEFAULT_FRACTION_COUNT,
                     float totalRotationAngle = ERE_DEFAULT_ROTATION_ANGLE);

        virtual ~EfectoRotEsc();

        // Setea el item actual como el seleccionado
        // Param: dib: El objeto dibujable que se quiere resaltar
        // Param: deltaX: Para mantener el item centrado con respecto al eje Y
        void setCurrent(Texto3D* dib, float deltaX = 0);

        void tic();

        Material getMaterialFront() const { return m_materialFront; }
        Material getMaterialBack() const { return m_materialBack; }

        // Se refresca en el proximo cambio
        void setMaterialFront(const Material&);
        void setMaterialBack(const Material&);

    protected:
    private:
        float m_highLightSize,       // escalado total
            * m_highLightFractions;  // guarda todos los escalados intermedios
        int m_fractionCount;         // cantidad de fracciones
        float m_rotationAngle,       // angulo total de rotacion
              m_rotationFraction;    // fraccion del angulo de rotacion (m_rotationAngle / m_fractionCount)

        Material m_materialFront,
                 m_materialBack;


        DXPair * m_currentItem,       // Item que se esta escalando
               * m_lastItem;          // Item que se esta restaurando

        int m_currentIndex;         // Para cuando esta resaltando
        int m_restoringIndex;       // Para cuando esta restaurando
};

#endif // EFECTOROTESC_H
