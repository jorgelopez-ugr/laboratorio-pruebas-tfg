#ifndef MODELO_H
#define MODELO_H

#include <vector>
#include <GL/glut.h>

void Dibuja (void);
void idle (int v);
void initModel ();

// Clases auxiliares básicas
class Objeto3D { 
public: 
    virtual void draw() = 0; 
}; 

class Vertice {
private:
    float x, y, z;
public:
    Vertice() : x(0), y(0), z(0) {}
    Vertice(float valx, float valy, float valz) : x(valx), y(valy), z(valz) {}
    float getX() { return x; }
    float getY() { return y; }
    float getZ() { return z; }
};

class Cara {
private:
    Vertice vertice1, vertice2, vertice3;
public:
    Cara() {}
    Cara(Vertice v1, Vertice v2, Vertice v3) : vertice1(v1), vertice2(v2), vertice3(v3) {}
    Vertice getVertice(int ident); // Cambié char a int por comodidad
};

class Mesh : public Objeto3D {
private:
    std::vector<int> vectCaras;       // Índices de triángulos
    std::vector<float> vectVertices;  // x, y, z, x, y, z...
    std::vector<float> normalCaras;
    std::vector<float> normalVertices;

public:
    void draw() {}; // Dummy
    void drawFlat();
    void drawSmooth();
    
    // NUEVOS MÉTODOS OPTIMIZADOS (sin iluminación)
    void drawPoints();      // Visualización como nube de puntos
    void drawWireframe();   // Visualización como malla de aristas
    void drawSolid();       // Visualización sólida sin iluminación
    
    // MÉTODOS DE CÁLCULO
    void calcTodasNormalesCara();
    void calcTodasNormalesVert();
    std::vector<float> calcNormalCara(Cara cara1);

    // NUEVO: Método para convertir datos DEM a malla
    // ancho y alto: número de puntos en la rejilla
    // escala: separación entre puntos
    // datosAltura: vector plano con los valores Z
    // calcularNormales: true para calcular normales (lento), false para omitir (rápido)
    void generarDesdeDEM(int ancho, int alto, float escala, 
                         const std::vector<float>& datosAltura, 
                         bool calcularNormales = true);
    
    // Mantenemos esto por compatibilidad, pero no lo usaremos
    void cargaPLY(char *rutaArchivo); 
};

#endif
