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
modulo visual.c
    Inicialización de ventana X
    Transformación de visualización
    Transformación de proyección
 
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "lab.hh"




//================================================ VARIABLES  

// ====== CÁMARA LIBRE (Free Camera) ======

// Posición de la cámara en el espacio 3D
float cam_posX = 0.0f;
float cam_posY = 100.0f; // Empezamos elevados para ver el terreno
float cam_posZ = 200.0f;

// Dirección hacia donde mira la cámara (vector normalizado)
float cam_dirX = 0.0f;
float cam_dirY = -0.3f;
float cam_dirZ = -1.0f;

// Vector "arriba" de la cámara
float cam_upX = 0.0f;
float cam_upY = 1.0f;
float cam_upZ = 0.0f;

// Ángulos de rotación de la cámara (en grados)
float cam_yaw = 0.0f;   // Rotación horizontal (izq-der)
float cam_pitch = -15.0f; // Rotación vertical (arriba-abajo)

// Velocidad de movimiento
float cam_speed = 5.0f;
float cam_speed_multiplier = 1.0f; // Shift para ir más rápido

// Sistema de cámara antiguo (mantener por compatibilidad)
float view_rotx = 30, view_roty = 45,view_rotz = 0;
float D = 10;


/**

Tamaño de la ventana X

**/


float anchoVentana, altoVentana;




/** 	void setCamara()

Cambia los parámetros de la cámara en el modulo visual

**/
void setCamara (float ax, float ay, float az, float d)
{
  view_rotx = ax;
  view_roty = ay;
  view_rotz = az;
  D = d;
}


void getCamara (float &ax, float &ay, float &az, float &d)
{
  ax = view_rotx;
  ay = view_roty;
  az = view_rotz;
  d = D;
}

// ====== FUNCIONES DE CÁMARA LIBRE ======

void actualizarDireccionCamara() {
    // Convertir ángulos a radianes
    float yawRad = cam_yaw * M_PI / 180.0f;
    float pitchRad = cam_pitch * M_PI / 180.0f;
    
    // Calcular la dirección de la cámara basada en los ángulos
    cam_dirX = cos(pitchRad) * sin(yawRad);
    cam_dirY = sin(pitchRad);
    cam_dirZ = -cos(pitchRad) * cos(yawRad);
    
    // Normalizar el vector dirección
    float length = sqrt(cam_dirX * cam_dirX + cam_dirY * cam_dirY + cam_dirZ * cam_dirZ);
    if (length > 0) {
        cam_dirX /= length;
        cam_dirY /= length;
        cam_dirZ /= length;
    }
}

void moverCamaraAdelante(float distancia) {
    cam_posX += cam_dirX * distancia;
    cam_posY += cam_dirY * distancia;
    cam_posZ += cam_dirZ * distancia;
}

void moverCamaraLateral(float distancia) {
    // Vector derecha = dirección × arriba
    float rightX = cam_dirY * cam_upZ - cam_dirZ * cam_upY;
    float rightY = cam_dirZ * cam_upX - cam_dirX * cam_upZ;
    float rightZ = cam_dirX * cam_upY - cam_dirY * cam_upX;
    
    // Normalizar
    float length = sqrt(rightX * rightX + rightY * rightY + rightZ * rightZ);
    if (length > 0) {
        rightX /= length;
        rightY /= length;
        rightZ /= length;
    }
    
    cam_posX += rightX * distancia;
    cam_posY += rightY * distancia;
    cam_posZ += rightZ * distancia;
}

void moverCamaraVertical(float distancia) {
    // Mover en dirección del vector "arriba"
    cam_posX += cam_upX * distancia;
    cam_posY += cam_upY * distancia;
    cam_posZ += cam_upZ * distancia;
}

void rotarCamara(float deltaYaw, float deltaPitch) {
    cam_yaw += deltaYaw;
    cam_pitch += deltaPitch;
    
    // Limitar pitch para no dar la vuelta completa
    if (cam_pitch > 89.0f) cam_pitch = 89.0f;
    if (cam_pitch < -89.0f) cam_pitch = -89.0f;
    
    actualizarDireccionCamara();
}

void resetearCamara() {
    cam_posX = 0.0f;
    cam_posY = 100.0f;
    cam_posZ = 200.0f;
    cam_yaw = 0.0f;
    cam_pitch = -15.0f;
    actualizarDireccionCamara();
}

/** 	void transformacionVisualizacion()

NUEVA IMPLEMENTACIÓN: Cámara libre (free camera) tipo FPS
Permite moverse libremente por el espacio 3D

**/
void transformacionVisualizacion ()
{
    // Calcular el punto al que miramos
    float lookAtX = cam_posX + cam_dirX;
    float lookAtY = cam_posY + cam_dirY;
    float lookAtZ = cam_posZ + cam_dirZ;
    
    // Usar gluLookAt para posicionar la cámara
    gluLookAt(
        cam_posX, cam_posY, cam_posZ,        // Posición de la cámara
        lookAtX, lookAtY, lookAtZ,           // Punto al que miramos
        cam_upX, cam_upY, cam_upZ            // Vector "arriba"
    );
}

/**	void fijaProyeccion()

Fija la transformacion de proyeccion en funcion del tamaño de la ventana y del tipo de proyeccion

**/
void fijaProyeccion ()
{
  float calto;			// altura de la ventana corregida

  if (anchoVentana > 0)
    calto = altoVentana / anchoVentana;
  else
    calto = 1;

  glFrustum (-1, 1, -calto, calto, 1.5, 1500);

  glMatrixMode (GL_MODELVIEW);
// A partir de este momento las transformaciones son de modelado.       
  glLoadIdentity ();

}


/**	void inicializaVentana(GLsizei ancho,GLsizei alto)

Inicializa el viewport para que ocupe toda la ventana X, y llama a fijaProyeccion.

**/

void inicializaVentana (GLsizei ancho, GLsizei alto)
{
  altoVentana = alto;
  anchoVentana = ancho;

  glViewport (0, 0, ancho, alto);	// Establecemos el Viewport usando la nueva anchura y altura de la ventana X

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  fijaProyeccion ();		// Cargamos la transformacion de proyeccion

}
