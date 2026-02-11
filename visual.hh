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

	visual.h
*/



//======================== visual.c

// ====== CÁMARA LIBRE ======

/** Actualiza el vector de dirección de la cámara basado en los ángulos **/
void actualizarDireccionCamara();

/** Mueve la cámara adelante/atrás en la dirección que mira **/
void moverCamaraAdelante(float distancia);

/** Mueve la cámara lateralmente (strafe) **/
void moverCamaraLateral(float distancia);

/** Mueve la cámara verticalmente (arriba/abajo) **/
void moverCamaraVertical(float distancia);

/** Rota la cámara (cambiar hacia dónde mira) **/
void rotarCamara(float deltaYaw, float deltaPitch);

/** Resetea la cámara a la posición inicial **/
void resetearCamara();

// ====== FUNCIONES ANTIGUAS (compatibilidad) ======

/** 	void setCamara()

Cambia los parámetros de la cámara en el modulo visual

**/
void setCamara (float ax, float ay, float az, float d);

void getCamara (float &ax, float &ay, float &az, float &d);

/** 	void transformacionVisualizacion()

Fija la transformacion de visualizacion en funcion de los angulos de rotacion view_rotx,
view_roty y view_rotz y el desplazamiento de la camara d.

**/
void transformacionVisualizacion ();

/**	void fijaProyeccion()

Fija la transformacion de proyeccion en funcion del tamaño de la ventana y del tipo de proyeccion

**/
void fijaProyeccion ();


/**	void inicializaVentana(GLsizei ancho,GLsizei alto)

Inicializa el viewport para que ocupe toda la ventana X, y llama a fijaProyeccion.

**/

void inicializaVentana (GLsizei ancho, GLsizei alto);
