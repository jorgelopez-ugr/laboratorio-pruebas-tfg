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

=======================================================/
modulo entradaTeclado.c
	Gestion de eventos de teclado
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "lab.hh"

// Variable externa del modo de visualización
extern int modoVisualizacion;

// Variables externas de la cámara libre
extern float cam_speed;
extern float cam_speed_multiplier;

// Variable para controlar iluminación
extern bool iluminacionActiva;


/** 

Imprime en la consola las instrucciones del programa

**/
void printHelp ()
{

  printf ("\n\n     Prácticas de Informática Gráfica	Curso 2023-2024");
  printf ("\n\n Dpto. Lenguajes y Sistemas Informaticos");
  printf ("\n E.T.S.I. Informatica		Univ. de Granada ");
  printf ("\n");
  printf ("\n Opciones: \n\n");
  printf ("=== CÁMARA LIBRE ===\n");
  printf ("W/A/S/D: Mover adelante/izq/atrás/der\n");
  printf ("Q/E: Subir/Bajar\n");
  printf ("Ratón Izq + Arrastrar: Mirar alrededor\n");
  printf ("Ratón Der + Arrastrar: Zoom adelante/atrás\n");
  printf ("Ratón Central + Arrastrar: Pan lateral/vertical\n");
  printf ("R: Resetear cámara\n");
  printf ("[ / ]: Reducir/Aumentar velocidad de movimiento\n\n");
  printf ("=== VISUALIZACIÓN ===\n");
  printf ("P (Shift+p): Modo PUNTOS\n");
  printf ("W (Shift+w): Modo WIREFRAME\n");
  printf ("S (Shift+s): Modo SOLIDO\n");
  printf ("L: Toggle iluminación (ver pendientes)\n");
  printf ("1/2/3: Modos alternativos OpenGL\n\n");
  printf ("=== GENERAL ===\n");
  printf ("h, H: Imprime informacion de ayuda \n");
  printf ("+,-: Mover cámara adelante/atrás \n");
  // Anyade la informacion de las opciones que introduzcas aqui !!       

  printf ("\n Escape: Salir");
  printf ("\n\n\n");
}




/* @teclado ---------------------------------------------------------------- */

/** 		void letra (unsigned char k, int x, int y)

Este procedimiento es llamado por el sistema cuando se pulsa una tecla normal
El codigo k es el ascii de la letra

Para anyadir nuevas ordenes de teclado coloca el correspondiente case.

Parametros de entrada:

k: codigo del caracter pulsado

x:

y:

**/

float rotxCamara = 30, rotyCamara = 45;
float dCamara = 10;


void letra (unsigned char k, int x, int y)
{

  switch (k)
    {
    case 'h':
    case 'H':
      printHelp ();		// H y h imprimen ayuda
      break;
    
    // ====== CONTROLES DE CÁMARA LIBRE ======
    case 'w':
      moverCamaraAdelante(cam_speed * cam_speed_multiplier);
      break;
    case 's':
      moverCamaraAdelante(-cam_speed * cam_speed_multiplier);
      break;
    case 'a':
      moverCamaraLateral(-cam_speed * cam_speed_multiplier);
      break;
    case 'd':
      moverCamaraLateral(cam_speed * cam_speed_multiplier);
      break;
    case 'q':
      moverCamaraVertical(cam_speed * cam_speed_multiplier); // Subir
      break;
    case 'e':
      moverCamaraVertical(-cam_speed * cam_speed_multiplier); // Bajar
      break;
    case 'r':
      resetearCamara();
      printf("Cámara reseteada\n");
      break;
      
    // Ajustar velocidad
    case '[':
      cam_speed *= 0.5f;
      if (cam_speed < 0.1f) cam_speed = 0.1f;
      printf("Velocidad: %.1f\n", cam_speed);
      break;
    case ']':
      cam_speed *= 2.0f;
      if (cam_speed > 100.0f) cam_speed = 100.0f;
      printf("Velocidad: %.1f\n", cam_speed);
      break;
      
    // ====== CONTROLES ANTIGUOS ======
    case '+':			// acerca la cámara (antigua)
      moverCamaraAdelante(5.0f);
      break;
    case '-':			// aleja la cámara (antigua)
      moverCamaraAdelante(-5.0f);
      break;
    case 27:			// Escape  Terminar
      exit (0);

    case '1':
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    case '2':
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    case '3':
      glPointSize(3.0f);
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      break;
    
    // MODOS DE VISUALIZACIÓN DEM (usar mayúsculas)
    case 'P':
      modoVisualizacion = 0; // PUNTOS
      printf("Modo: PUNTOS\n");
      break;
    case 'W':
      modoVisualizacion = 1; // WIREFRAME
      printf("Modo: WIREFRAME\n");
      break;
    case 'S':
      modoVisualizacion = 2; // SÓLIDO
      printf("Modo: SOLIDO\n");
      break;
    
    // Toggle iluminación para ver pendientes
    case 'l':
    case 'L':
      iluminacionActiva = !iluminacionActiva;
      if (iluminacionActiva) {
        printf("Iluminación: ACTIVADA (ver pendientes)\n");
      } else {
        printf("Iluminación: DESACTIVADA (color plano)\n");
      }
      break;
      
    default:
      return;
    }
  glutPostRedisplay ();		// Actualizar la imagen
}

/**		void especial(int k, int x, int y)
Este procedimiento es llamado por el sistema cuando se pulsa una tecla
especial. El codigo k esta definido en glut mediante constantes

ACTUALIZADO: Ahora mueve la cámara libre

**/
void especial (int k, int x, int y)
{
  float speed = cam_speed * cam_speed_multiplier;
  
  switch (k)
    {
    case GLUT_KEY_UP:
      moverCamaraAdelante(speed);
      break;
    case GLUT_KEY_DOWN:
      moverCamaraAdelante(-speed);
      break;
    case GLUT_KEY_LEFT:
      moverCamaraLateral(-speed);
      break;
    case GLUT_KEY_RIGHT:
      moverCamaraLateral(speed);
      break;
    case GLUT_KEY_PAGE_UP:
      moverCamaraVertical(speed);
      break;
    case GLUT_KEY_PAGE_DOWN:
      moverCamaraVertical(-speed);
      break;
    default:
      return;
    }
  glutPostRedisplay ();		// Actualiza la imagen
}
