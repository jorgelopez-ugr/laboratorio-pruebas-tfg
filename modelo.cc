#include "modelo.hh"
#include "file_ply_stl.hh"
#include <math.h>
#include <iostream>

// --- Implementación de clases auxiliares ---
Vertice Cara::getVertice(int ident){
    if (ident == 1) return vertice1;
    if (ident == 2) return vertice2;
    if (ident == 3) return vertice3;
    return Vertice(0,0,0);
}

// --- Lógica del Mesh ---

// ESTA ES LA FUNCIÓN CLAVE PARA TU LABORATORIO
void Mesh::generarDesdeDEM(int ancho, int alto, float escala, 
                           const std::vector<float>& datosAltura,
                           bool calcularNormales) {
    vectVertices.clear();
    vectCaras.clear();
    normalCaras.clear();
    normalVertices.clear();

    std::cout << "  [1/3] Generando vértices..." << std::endl;
    
    // 1. Generar Vértices
    // Centramos la malla en (0,0,0) para que la cámara gire bien alrededor
    float offsetX = (ancho - 1) * escala / 2.0f;
    float offsetZ = (alto - 1) * escala / 2.0f;

    for (int i = 0; i < alto; ++i) {      // Z
        for (int j = 0; j < ancho; ++j) { // X
            float x = (j * escala) - offsetX;
            float z = (i * escala) - offsetZ;
            // Obtenemos la altura del vector linealizado
            float y = datosAltura[i * ancho + j]; 

            vectVertices.push_back(x);
            vectVertices.push_back(y);
            vectVertices.push_back(z);
        }
    }

    std::cout << "  [2/3] Generando triángulos..." << std::endl;
    
    // 2. Generar Índices (Triángulos)
    // Conectamos cada punto con sus vecinos para formar quads y luego triángulos
    for (int i = 0; i < alto - 1; ++i) {
        for (int j = 0; j < ancho - 1; ++j) {
            int topLeft     = i * ancho + j;
            int topRight    = i * ancho + (j + 1);
            int bottomLeft  = (i + 1) * ancho + j;
            int bottomRight = (i + 1) * ancho + (j + 1);

            // Triángulo 1
            vectCaras.push_back(topLeft);
            vectCaras.push_back(bottomLeft);
            vectCaras.push_back(topRight);

            // Triángulo 2
            vectCaras.push_back(topRight);
            vectCaras.push_back(bottomLeft);
            vectCaras.push_back(bottomRight);
        }
    }

    // 3. Calcular Iluminación (Normales) - OPCIONAL para rendimiento
    if (calcularNormales) {
        std::cout << "  [3/3] Calculando normales (esto puede tardar)..." << std::endl;
        calcTodasNormalesCara();
        calcTodasNormalesVert();
    } else {
        std::cout << "  [3/3] Omitiendo cálculo de normales (modo optimizado)" << std::endl;
    }
    
    std::cout << "\n  ✓ DEM Generado: " << vectVertices.size()/3 << " vértices y " 
              << vectCaras.size()/3 << " triángulos." << std::endl;
}

// --- NUEVOS MÉTODOS DE VISUALIZACIÓN OPTIMIZADOS ---

void Mesh::drawPoints() {
    // Visualizar como nube de puntos (MUY RÁPIDO)
    glPointSize(2.0f); // Tamaño de los puntos
    glBegin(GL_POINTS);
    for (size_t i = 0; i < vectVertices.size(); i += 3) {
        glVertex3f(vectVertices[i], vectVertices[i+1], vectVertices[i+2]);
    }
    glEnd();
}

void Mesh::drawWireframe() {
    // Visualizar solo las aristas (RÁPIDO)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vectCaras.size(); i += 3) {
        if (i+2 >= vectCaras.size()) break;
        
        int idVert1 = vectCaras[i]   * 3;
        int idVert2 = vectCaras[i+1] * 3;
        int idVert3 = vectCaras[i+2] * 3;

        glVertex3f(vectVertices[idVert1], vectVertices[idVert1+1], vectVertices[idVert1+2]);
        glVertex3f(vectVertices[idVert2], vectVertices[idVert2+1], vectVertices[idVert2+2]);
        glVertex3f(vectVertices[idVert3], vectVertices[idVert3+1], vectVertices[idVert3+2]);
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restaurar modo normal
}

void Mesh::drawSolid() {
    // Visualizar sólido sin iluminación (MODERADAMENTE RÁPIDO)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vectCaras.size(); i += 3) {
        if (i+2 >= vectCaras.size()) break;
        
        int idVert1 = vectCaras[i]   * 3;
        int idVert2 = vectCaras[i+1] * 3;
        int idVert3 = vectCaras[i+2] * 3;

        glVertex3f(vectVertices[idVert1], vectVertices[idVert1+1], vectVertices[idVert1+2]);
        glVertex3f(vectVertices[idVert2], vectVertices[idVert2+1], vectVertices[idVert2+2]);
        glVertex3f(vectVertices[idVert3], vectVertices[idVert3+1], vectVertices[idVert3+2]);
    }
    glEnd();
}

void Mesh::drawSmoothLit() {
    // Visualizar con iluminación suave usando normales por vértice
    // Esto permite ver el relieve y las pendientes del terreno
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vectCaras.size(); i += 3) {
        if (i+2 >= vectCaras.size()) break;
        
        int idVert1 = vectCaras[i]   * 3;
        int idVert2 = vectCaras[i+1] * 3;
        int idVert3 = vectCaras[i+2] * 3;
        
        // Especificar normal por vértice para sombreado suave
        if (!normalVertices.empty()) {
            glNormal3f(normalVertices[idVert1], normalVertices[idVert1+1], normalVertices[idVert1+2]);
        }
        glVertex3f(vectVertices[idVert1], vectVertices[idVert1+1], vectVertices[idVert1+2]);
        
        if (!normalVertices.empty()) {
            glNormal3f(normalVertices[idVert2], normalVertices[idVert2+1], normalVertices[idVert2+2]);
        }
        glVertex3f(vectVertices[idVert2], vectVertices[idVert2+1], vectVertices[idVert2+2]);
        
        if (!normalVertices.empty()) {
            glNormal3f(normalVertices[idVert3], normalVertices[idVert3+1], normalVertices[idVert3+2]);
        }
        glVertex3f(vectVertices[idVert3], vectVertices[idVert3+1], vectVertices[idVert3+2]);
    }
    glEnd();
}

// --- Mantenemos tu código de dibujo existente ---

void Mesh::drawFlat(){
  glShadeModel(GL_FLAT);
  glBegin (GL_TRIANGLES);
  for (size_t i = 0 ; i < vectCaras.size() ; i+=3){
      // Chequeo de seguridad simple
      if (i+2 >= vectCaras.size()) break;
      
      int idVert1 = vectCaras[i]   * 3;
      int idVert2 = vectCaras[i+1] * 3;
      int idVert3 = vectCaras[i+2] * 3;

      glNormal3f(normalCaras[i], normalCaras[i+1], normalCaras[i+2]);
      glVertex3f(vectVertices[idVert1], vectVertices[idVert1+1], vectVertices[idVert1+2]);
      glVertex3f(vectVertices[idVert2], vectVertices[idVert2+1], vectVertices[idVert2+2]);
      glVertex3f(vectVertices[idVert3], vectVertices[idVert3+1], vectVertices[idVert3+2]);
  }
  glEnd ();
}

void Mesh::drawSmooth(){
  glShadeModel(GL_SMOOTH);
  glBegin (GL_TRIANGLES);
  for (size_t i = 0 ; i < vectCaras.size() ; i+=3){
      if (i+2 >= vectCaras.size()) break;

      int idVert1 = vectCaras[i]   * 3;
      int idVert2 = vectCaras[i+1] * 3;
      int idVert3 = vectCaras[i+2] * 3;
      
      glNormal3f(normalVertices[idVert1], normalVertices[idVert1+1], normalVertices[idVert1+2]);
      glVertex3f(vectVertices[idVert1], vectVertices[idVert1+1], vectVertices[idVert1+2]);

      glNormal3f(normalVertices[idVert2], normalVertices[idVert2+1], normalVertices[idVert2+2]);
      glVertex3f(vectVertices[idVert2], vectVertices[idVert2+1], vectVertices[idVert2+2]);

      glNormal3f(normalVertices[idVert3], normalVertices[idVert3+1], normalVertices[idVert3+2]);
      glVertex3f(vectVertices[idVert3], vectVertices[idVert3+1], vectVertices[idVert3+2]);
  }
  glEnd ();
}

// --- Cálculos de Normales (Tu código original + pequeños fixes de seguridad) ---

std::vector<float> Mesh::calcNormalCara(Cara cara1){
  float v1[3] = {cara1.getVertice(2).getX() - cara1.getVertice(1).getX(),
                 cara1.getVertice(2).getY() - cara1.getVertice(1).getY(),
                 cara1.getVertice(2).getZ() - cara1.getVertice(1).getZ()};

  float v2[3] = {cara1.getVertice(3).getX() - cara1.getVertice(1).getX(),
                 cara1.getVertice(3).getY() - cara1.getVertice(1).getY(),
                 cara1.getVertice(3).getZ() - cara1.getVertice(1).getZ()};

  std::vector<float> normal(3);
  normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
  normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
  normal[2] = v1[0] * v2[1] - v1[1] * v2[0];
  
  float longitud = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
  if (longitud > 0){ normal[0]/=longitud; normal[1]/=longitud; normal[2]/=longitud; }
  return normal;
}

void Mesh::calcTodasNormalesCara(){
  normalCaras.clear();
  for (size_t i = 0 ; i < vectCaras.size() ; i+=3){
    int idVert1 = vectCaras[i]   * 3;
    int idVert2 = vectCaras[i+1] * 3;
    int idVert3 = vectCaras[i+2] * 3;

    Vertice vert1(vectVertices[idVert1], vectVertices[idVert1+1], vectVertices[idVert1+2]);
    Vertice vert2(vectVertices[idVert2], vectVertices[idVert2+1], vectVertices[idVert2+2]);
    Vertice vert3(vectVertices[idVert3], vectVertices[idVert3+1], vectVertices[idVert3+2]);

    std::vector<float> unaNormal = calcNormalCara(Cara(vert1, vert2, vert3));
    normalCaras.insert(normalCaras.end(), unaNormal.begin(), unaNormal.end());
  }
}

void Mesh::calcTodasNormalesVert(){
  normalVertices.assign(vectVertices.size(), 0.0f);

  for (size_t i = 0 ; i < vectCaras.size() ; i+=3){
    int idx[3] = { vectCaras[i]*3, vectCaras[i+1]*3, vectCaras[i+2]*3 };
    
    for(int k=0; k<3; k++) {
        normalVertices[idx[k]]   += normalCaras[i];
        normalVertices[idx[k]+1] += normalCaras[i+1];
        normalVertices[idx[k]+2] += normalCaras[i+2];
    }
  }

  for (size_t i = 0 ; i < normalVertices.size() ; i+=3){
    float len = sqrt(normalVertices[i]*normalVertices[i] + 
                     normalVertices[i+1]*normalVertices[i+1] + 
                     normalVertices[i+2]*normalVertices[i+2]);
    if (len > 0){
      normalVertices[i] /= len;
      normalVertices[i+1] /= len;
      normalVertices[i+2] /= len;
    }
  }
}

// Dummy para mantener compatibilidad si no borraste file_ply_stl
void Mesh::cargaPLY(char *rutaArchivo) {}
