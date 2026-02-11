#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "lab.hh"
#include "visual.hh" 
#include <iostream>
#include <vector>

// Declaramos la malla global
Mesh mallaDEM;

// Datos random generados de prueba
void initModel() { 
    // Función que genera un terreno sintético (senoidal).
    
    int ancho = 100;
    int alto = 100;
    float escala = 0.5f; // Distancia entre puntos
    std::vector<float> alturas;
    alturas.resize(ancho * alto);

    for(int z = 0; z < alto; z++) {
        for(int x = 0; x < ancho; x++) {
            // Generar onda senoidal 3D
            float valX = (float)x / 10.0f;
            float valZ = (float)z / 10.0f;
            float y = sin(valX) * cos(valZ) * 2.0f; 
            
            // Si tienes un array de datos, aquí harías: 
            // y = misDatos[z * ancho + x];
            
            alturas[z * ancho + x] = y;
        }
    }

    std::cout << "Generando malla..." << std::endl;
    mallaDEM.generarDesdeDEM(ancho, alto, escala, alturas);
}

void Dibuja(void) {
    glPushMatrix(); 
    glClearColor(0.2, 0.2, 0.2, 1.0); // Fondo gris oscuro para ver mejor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    transformacionVisualizacion(); // Cámara

    // Luz simple
    GLfloat pos0[] = {10.0, 10.0, 10.0, 1}; // Luz arriba y a la derecha
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    
    // Material
    float matAmb[] = {0.2, 0.2, 0.2, 1.0};
    float matDif[] = {0.8, 0.5, 0.2, 1.0}; // Color tierra
    float matSpec[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

    // Dibujar DEM
    mallaDEM.drawSmooth(); // O drawFlat() si quieres ver los polígonos

    glPopMatrix();
    glutSwapBuffers();
}

void idle(int v) {
    glutPostRedisplay();
    glutTimerFunc(30, idle, 0);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Visor DEM OpenGL");

    initModel();

    glutDisplayFunc(Dibuja);
    glutReshapeFunc(inicializaVentana);
    glutKeyboardFunc(letra);
    glutSpecialFunc(especial);
    glutMouseFunc(clickRaton);
    glutMotionFunc(RatonMovido);
    glutTimerFunc(30, idle, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // Importante si escalas

    std::cout << "Controles:\n Ratón Izq: Rotar\n Ratón Der: Zoom (arriba/abajo)\n" << std::endl;

    glutMainLoop();
    return 0;
}
