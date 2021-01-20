#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Escena.h"
#include "Texto3D.h"
#include "Sprite.h"
#include "Logger.h"


#include <vector>

/****************************************************
* la idea es hacer ºun rectangulo negro
* que contenga un texto 3D adentro
* y si es posible que tenga un buen bordecito en 3D
*****************************************************/
class TextBox
{
    public:
        // El ancho no se esta usando en este momento
        TextBox(float ancho, Vector posicion = Vector(), string charSet = "charset3dSpider.mdl");
        virtual ~TextBox();

        void tic();

        // User estas funciones para agregar/quitar el TextBox a la escena
        void addToEscena(Escena *);
        // No olvidar llamarla antes de hacer el delete
        void removeFromEscena(Escena *);

        // Coordenada de la esquina superior izquierada del textbox
        void setPosicion(Vector);
        void setAncho(float);
        void setFocus(bool);
        void setTexto(std::string);
        void setMaxTextLength(int);
        void setReadOnly(bool);

        /// Dejar como false si se quiere soportar todo lo que halla en la tabla ascii
        /// Si está en true, se aceptan letras, numeros, el guion y el punto
        /// Esto surje de la idea de controlar por ejemplo el nombre de la maquina sea formalmente valido
        /// NOTA: Si esto vale true se haceptan inputs con ascii entre 32 y 255 :)
        void setOnlyStandardDNSChars(bool);

        void setNumeric(bool);

        // Getters
        Vector getPosicion() { return m_posicion; };
        float getAlto() { return m_alto; };
        float getAncho() { return m_ancho; };
        bool getOnlyStandardDNSChars() { return m_onlyStandardChars; }
        bool isNumeric() { return m_isNumeric; }
        bool isFocused() { return m_isFocused; }
        bool isReadOnly() { return m_readOnly; }
        string getTexto() { return m_texto->getTexto(); };
        int getTextLength() { return m_textLen; }
        int getMaxLength() { return m_maxTextLen; }

    protected:
    private:

        Texto3D * m_texto;

        Vector m_posicion;

        int
            m_textLen,
            m_maxTextLen;
        float
            m_alto,
            m_ancho;
        bool
            m_onlyStandardChars,
            m_isNumeric,
            m_isFocused,
            m_readOnly;

        void appendChar(char);
        void backSpace();
        void restoreText();
};

#endif // TEXTBOX_H
