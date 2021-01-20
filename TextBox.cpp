#include "TextBox.h"
#include "MenuRed.h"
#include "Teclado.h"
#include "Mouse.h"      // A futuro si da el tiempo :)


TextBox::TextBox(float ancho, Vector posicion, string charSet)
{
    m_texto = new Texto3D("", charSet);
    m_texto->posicionar(posicion);
    m_texto->escalar(0.7);
    m_texto->rotar(PI, Vector(0,1,0), false);

    m_onlyStandardChars = false;
    m_isNumeric = false;
    m_readOnly = false;
    m_textLen = 0;
    m_maxTextLen = 99999;
}

TextBox::~TextBox()
{
       delete m_texto;
    m_texto = 0;
}

void TextBox::tic()
{
    // Poner aca codigo que se ejecuta siempre en cada tic
    // Independiente si tiene o no foco
    // Por ejemplo algun efectito raro

    if (!m_isFocused || m_readOnly) return;
    // Se va a aceptar solamente una tecla por tic :)  (jejeje)
    Teclado * t = Teclado::getInstancia();

    if (t->tecla(8))   // BackSpace
    {
        t->setTecla(8, false);
        backSpace();
        return;
    }
    for (int i = 48; i < 58; i++) // Numeros
    {
        if (t->tecla(i))
        {
            t->setTecla(i, false);
            appendChar(i);
            return;
        }
    }
    if ( (!m_isNumeric || !m_textLen) && t->tecla('-'))    // El guion
    {
        t->setTecla('-', false);
        appendChar('-');
    }
    if (!m_isNumeric)
    {
        if (!m_onlyStandardChars)
        {
            for (int i = 32; i < 256; i++) // pruebo con todos
            {
                if (t->tecla(i))
                {
                    t->setTecla(i, false);
                    appendChar(i);
                    return;
                }
            }
        }
        else
        {
            for (int i = 65; i < 91; i++) // Mayusculas
            {
                if (t->tecla(i))
                {
                    t->setTecla(i, false);
                    appendChar(i);
                    return;
                }
            }
            for (int i = 97; i < 123; i++) // Minusculas
            {
                if (t->tecla(i))
                {
                    t->setTecla(i, false);
                    appendChar(i);
                    return;
                }
            }
            if (t->tecla('.'))   // El puntito (para la IP
            {
                t->setTecla('.', false);
                appendChar('.');
            }
        }
    }
}

void TextBox::appendChar(char c)
{
    if (m_textLen < m_maxTextLen)
    {
        m_texto->setTexto(m_texto->getTexto() + c);
        m_textLen += 1;
    }
}

void TextBox::backSpace()
{
    if (m_textLen > 0)
    {
        string str = m_texto->getTexto();
        m_texto->setTexto(str.substr(0, str.size() - 1));
        m_textLen-= 1;
    }
}


void TextBox::addToEscena(Escena * e)
{
    e->addDibujable(m_texto);
}

void TextBox::removeFromEscena(Escena * e)
{
    e->removeDibujable(m_texto);
}
/*
void TextBox::setAncho(float a)
{
    m_ancho = a;
}
*/

void TextBox::setPosicion(Vector v)
{
    m_posicion = v;
    m_texto->posicionar(v);
}

void TextBox::setOnlyStandardDNSChars(bool b)
{
    if (m_onlyStandardChars == b)
        return;
    m_onlyStandardChars  = b;
    if (m_onlyStandardChars){
        m_maxTextLen = 63;
        restoreText();
    }
}

void TextBox::setNumeric(bool b)
{
    if (m_isNumeric == b)
        return;
    m_isNumeric = b;
    restoreText();
}

void TextBox::setFocus(bool b)
{
    m_isFocused = b;
}

void TextBox::setTexto(std::string str)
{
    m_texto->setTexto(str);
    m_textLen = str.size();
    restoreText();
}

void TextBox::setMaxTextLength(int i)
{
    if (!m_onlyStandardChars){
        m_maxTextLen = i;
        if (i < m_texto->getTexto().size())
            restoreText();
    }
}

void TextBox::restoreText()
{
    std::string str = m_texto->getTexto();
    if (m_isNumeric)
    {
        if (str.size() > 0)
        {
            int i = 1;
            if((str[0] < 48 || str[0] > 57) && str[0] != '-')
                goto restore;
            for (; i < str.size(); i++)
            {
                if (str[i] < 48 || str[i] > 57)
                {
                    restore:
                    m_texto->setTexto("");
                    m_textLen = 0;
                    return;
                }
            }
        }
    }
    if(m_onlyStandardChars)
    {
        for (int i = 0; i < str.size(); i++)
        {
            if ((str[i] < 'A' && str[i] != 45) || (str[i] > 'Z' && str[i] < 'a') || (str[i] > 'z'))
            {
                m_texto->setTexto("");
                m_textLen = 0;
                return;
            }
        }
    }
    if (m_textLen > m_maxTextLen)
    {
        str= str.substr(0, m_maxTextLen);
        m_textLen = m_maxTextLen;
        m_texto->setTexto(str);
    }
}

void TextBox::setReadOnly(bool b)
{
    if (m_readOnly == b)
        return;
    m_readOnly = b;
    if (m_readOnly)
    {
        int len = m_texto->getTexto().size();
        for (int i = 0; i < len; i++){
            m_texto->getCaracter(i)->setColor(0.7,0.7,0.7);
            m_texto->getCaracter(i)->setOpacidad(0.4);
        }
    }
    else
    {
        int len = m_texto->getTexto().size();
        for (int i = 0; i < len; i++){
            m_texto->getCaracter(i)->setColor(1,1,1);
            m_texto->getCaracter(i)->setOpacidad(1);
        }
    }
}
