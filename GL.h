//Esto es como el factor comun para incluir las librerias de OpenGL


#ifndef GL_H
#define GL_H

#include <ctype.h>
#ifdef __WINDOWS__
	#define GLUT_DISABLE_ATEXIT_HACK
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
using namespace std;

#endif
