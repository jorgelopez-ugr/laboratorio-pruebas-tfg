/*	
=======================================================
	G. Arroyo, J.C. Torres
	Dpto. Lenguajes y Sistemas Informticos
	(Univ. de Granada)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details
 http://www.gnu.org/copyleft/gpl.html

=======================================================

	 mouse.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "lab.hh"


#define __mouse__ 
#include "mouse.hh"

int MOUSE_LEFT_DOWN=0;
int MOUSE_MIDDLE_DOWN=0;
int MOUSE_RIGHT_DOWN=0;
int MOUSE_X, MOUSE_Y;



int ISINTERACTING;

/**

Procedimiento para gestionar los eventos de pulsacion de los botones del raton.

Argumentos:

boton: Codigo glut del boton que ha manipulado el usuario.

estado: Estado en el que esta el boton que se ha pulsado.

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.

**/

 void clickRaton( int boton, int estado, int x, int y )
{
ISINTERACTING=1;
if(boton==GLUT_LEFT_BUTTON && estado==GLUT_DOWN) {
	MOUSE_LEFT_DOWN=1;
	MOUSE_X=x;
	MOUSE_Y=y;
	}
else if(boton==GLUT_MIDDLE_BUTTON && estado==GLUT_DOWN) {
	MOUSE_MIDDLE_DOWN=1;
	MOUSE_X=x;
	MOUSE_Y=y;
	}
else if(boton==GLUT_RIGHT_BUTTON && estado==GLUT_DOWN) {
	MOUSE_RIGHT_DOWN=1;
	MOUSE_X=x;
	MOUSE_Y=y;
	}
else {	
	MOUSE_LEFT_DOWN=0;
	MOUSE_MIDDLE_DOWN=0;
	MOUSE_RIGHT_DOWN=0;
	ISINTERACTING=0;
	}
glutPostRedisplay();
}


/**

Procedimiento para gestionar los eventos de movimiento del raton.

NUEVA IMPLEMENTACIÓN: Controla la rotación de la cámara libre

Argumentos:

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.

**/

void RatonMovido( int x, int y )
{
    // Sensibilidad del ratón
    float sensitivity = 0.2f;
    
    if(MOUSE_LEFT_DOWN) {
        // Calcular el desplazamiento del ratón
        int deltaX = x - MOUSE_X;
        int deltaY = y - MOUSE_Y;
        
        // Rotar la cámara
        rotarCamara(deltaX * sensitivity, -deltaY * sensitivity);
    }
    else if(MOUSE_RIGHT_DOWN) {
        // Zoom = mover adelante/atrás
        int deltaY = y - MOUSE_Y;
        moverCamaraAdelante(-deltaY * 0.5f);
    }
    else if(MOUSE_MIDDLE_DOWN) {
        // Botón central = mover lateral y vertical
        int deltaX = x - MOUSE_X;
        int deltaY = y - MOUSE_Y;
        
        moverCamaraLateral(-deltaX * 0.5f);
        moverCamaraVertical(deltaY * 0.5f);
    }
    
    MOUSE_X = x;
    MOUSE_Y = y;
    glutPostRedisplay();
}
