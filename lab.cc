#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "lab.hh"
#include "visual.hh" 
#include <iostream>
#include <vector>
#include <gdal/gdal_priv.h>

// Declaramos la malla global
Mesh mallaDEM;

// Variable global para modo de visualización
int modoVisualizacion = 2; // 0=puntos, 1=wireframe, 2=sólido

// Variable global para iluminación
bool iluminacionActiva = false; // Empieza desactivada para mejor rendimiento

// Función para leer archivo GeoTIFF con datos DEM
bool leerDEMdesdeTIFF(const char* nombreArchivo, int& ancho, int& alto, 
                       float& escala, std::vector<float>& alturas, int submuestreo = 1) {
    
    // Registrar todos los drivers de GDAL
    GDALAllRegister();
    
    // Abrir el archivo
    GDALDataset* dataset = (GDALDataset*) GDALOpen(nombreArchivo, GA_ReadOnly);
    if (dataset == nullptr) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return false;
    }
    
    std::cout << "Leyendo archivo: " << nombreArchivo << std::endl;
    
    // Obtener dimensiones
    ancho = dataset->GetRasterXSize();
    alto = dataset->GetRasterYSize();
    int numBandas = dataset->GetRasterCount();
    
    std::cout << "Dimensiones originales: " << ancho << " x " << alto << std::endl;
    
    // OPTIMIZACIÓN: Aplicar submuestreo si el DEM es muy grande
    if (submuestreo > 1) {
        std::cout << "Aplicando submuestreo 1:" << submuestreo << std::endl;
        ancho = ancho / submuestreo;
        alto = alto / submuestreo;
        std::cout << "Dimensiones reducidas: " << ancho << " x " << alto << std::endl;
    }
    
    int totalPuntos = ancho * alto;
    std::cout << "Total de puntos a procesar: " << totalPuntos << std::endl;
    
    if (totalPuntos > 1000000) {
        std::cout << "¡ADVERTENCIA! Más de 1 millón de puntos. El rendimiento puede ser lento." << std::endl;
    }
    
    std::cout << "Número de bandas: " << numBandas << std::endl;
    
    // Obtener información georreferenciada (para calcular escala)
    double geoTransform[6];
    if (dataset->GetGeoTransform(geoTransform) == CE_None) {
        escala = (float) fabs(geoTransform[1]) * submuestreo; // Ajustar por submuestreo
        std::cout << "Resolución del píxel: " << escala << " metros/píxel" << std::endl;
    } else {
        escala = 1.0f * submuestreo; // Valor por defecto si no hay georreferenciación
        std::cout << "No se encontró georreferenciación, usando escala = " << escala << std::endl;
    }
    
    // Leer la primera banda (normalmente los datos de elevación están en la banda 1)
    GDALRasterBand* banda = dataset->GetRasterBand(1);
    
    // Información de la banda
    int hasNoData;
    double noDataValue = banda->GetNoDataValue(&hasNoData);
    if (hasNoData) {
        std::cout << "Valor NoData: " << noDataValue << std::endl;
    }
    
    // Reservar espacio para los datos
    alturas.resize(ancho * alto);
    
    // Leer datos con submuestreo - GDAL se encarga de la reducción
    std::cout << "Leyendo datos..." << std::endl;
    int anchoOriginal = dataset->GetRasterXSize();
    int altoOriginal = dataset->GetRasterYSize();
    
    CPLErr err = banda->RasterIO(GF_Read, 
                                  0, 0,                    // Offset inicial
                                  anchoOriginal, altoOriginal,  // Tamaño del bloque a leer
                                  alturas.data(),          // Buffer destino
                                  ancho, alto,             // Tamaño del buffer (submuestreado)
                                  GDT_Float32,             // Tipo de conversión
                                  0, 0);                   // Sin pixel/line spacing
    
    if (err != CE_None) {
        std::cerr << "Error al leer datos del raster" << std::endl;
        GDALClose(dataset);
        return false;
    }
    
    // Calcular estadísticas
    float minAltura = alturas[0], maxAltura = alturas[0];
    float suma = 0.0f;
    int numValidos = 0;
    
    for (int i = 0; i < ancho * alto; i++) {
        float valor = alturas[i];
        
        // Manejar valores NoData (reemplazar con 0 o interpolación)
        if (hasNoData && fabs(valor - noDataValue) < 1.0) {
            alturas[i] = 0.0f; // Asignar altura 0 a valores inválidos
            continue;
        }
        
        // Actualizar estadísticas solo con valores válidos
        if (numValidos == 0) {
            minAltura = valor;
            maxAltura = valor;
        } else {
            if (valor < minAltura) minAltura = valor;
            if (valor > maxAltura) maxAltura = valor;
        }
        suma += valor;
        numValidos++;
    }
    
    if (numValidos == 0) {
        std::cerr << "Error: No hay datos válidos en el archivo" << std::endl;
        GDALClose(dataset);
        return false;
    }
    
    float promedio = suma / numValidos;
    
    std::cout << "Estadísticas de elevación:" << std::endl;
    std::cout << "  Mínimo: " << minAltura << " m" << std::endl;
    std::cout << "  Máximo: " << maxAltura << " m" << std::endl;
    std::cout << "  Promedio: " << promedio << " m" << std::endl;
    std::cout << "  Rango: " << (maxAltura - minAltura) << " m" << std::endl;
    
    // Cerrar el dataset
    GDALClose(dataset);
    
    std::cout << "Archivo DEM cargado exitosamente!" << std::endl;
    return true;
}

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
    mallaDEM.generarDesdeDEM(ancho, alto, escala, alturas, false); // false = sin normales para rendimiento
}



void Dibuja(void) {
    glPushMatrix(); 
    glClearColor(0.1, 0.1, 0.15, 1.0); // Fondo oscuro azulado
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    transformacionVisualizacion(); // Cámara

    // Configurar iluminación si está activa
    if (iluminacionActiva) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        
        // Luz direccional desde arriba y diagonal (simula el sol)
        GLfloat luz_posicion[] = {1.0f, 2.0f, 1.0f, 0.0f}; // 0.0 = direccional
        GLfloat luz_ambiente[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Luz ambiente suave
        GLfloat luz_difusa[] = {0.8f, 0.8f, 0.7f, 1.0f};    // Luz principal cálida
        GLfloat luz_especular[] = {0.5f, 0.5f, 0.5f, 1.0f}; // Brillos suaves
        
        glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion);
        glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
        glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
        
        // Material del terreno
        GLfloat mat_ambiente[] = {0.3f, 0.35f, 0.25f, 1.0f};  // Verde oscuro
        GLfloat mat_difuso[] = {0.4f, 0.6f, 0.3f, 1.0f};      // Verde terreno
        GLfloat mat_especular[] = {0.2f, 0.2f, 0.2f, 1.0f};   // Poco brillo
        GLfloat mat_shininess = 10.0f;                         // Superficie mate
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambiente);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_difuso);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_especular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
        
        glEnable(GL_COLOR_MATERIAL); // Permitir que glColor afecte al material
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    } else {
        glDisable(GL_LIGHTING);
        // Color sólido simple (sin iluminación)
        glColor3f(0.3, 0.7, 0.4); // Verde terreno
    }

    // Dibujar DEM según modo de visualización
    switch(modoVisualizacion) {
        case 0: // PUNTOS
            mallaDEM.drawPoints();
            break;
        case 1: // WIREFRAME (aristas)
            mallaDEM.drawWireframe();
            break;
        case 2: // SÓLIDO
            if (iluminacionActiva) {
                mallaDEM.drawSmoothLit(); // Con normales para iluminación
            } else {
                mallaDEM.drawSolid(); // Sin normales, más rápido
            }
            break;
    }

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

    // Verificar si se pasó un archivo por argumento
    if (argc > 1) {
        // Cargar DEM desde archivo TIFF
        std::cout << "======================================" << std::endl;
        std::cout << "Cargando DEM desde: " << argv[1] << std::endl;
        std::cout << "======================================" << std::endl;
        
        int ancho, alto;
        float escala;
        std::vector<float> alturas;
        
        // SUBMUESTREO: Ajusta este valor según necesites
        // 1 = resolución completa (lento)
        // 2 = mitad de resolución (2x más rápido)
        // 4 = cuarto de resolución (4x más rápido)
        // 10 = décima parte (10x más rápido)
        int submuestreo = 5; // Valor inicial conservador
        
        std::cout << "\nOPCIONES DE RENDIMIENTO:" << std::endl;
        std::cout << "  - Submuestreo actual: 1:" << submuestreo << std::endl;
        std::cout << "  - Para cambiar, modifica la variable 'submuestreo' en main()" << std::endl;
        std::cout << std::endl;
        
        if (leerDEMdesdeTIFF(argv[1], ancho, alto, escala, alturas, submuestreo)) {
            std::cout << "\nGenerando malla 3D..." << std::endl;
            // SIEMPRE calculamos normales ahora, pero solo se usan si activas iluminación
            mallaDEM.generarDesdeDEM(ancho, alto, escala, alturas, true); // true = calcular normales
            std::cout << "¡Malla generada exitosamente!" << std::endl;
        } else {
            std::cerr << "Error al cargar el archivo DEM. Usando datos sintéticos." << std::endl;
            initModel();
        }
    } else {
        // Si no se pasa archivo, usar datos sintéticos
        std::cout << "No se especificó archivo DEM. Usando terreno sintético." << std::endl;
        std::cout << "Uso: " << argv[0] << " <archivo.tif>" << std::endl;
        initModel();
    }

    glutDisplayFunc(Dibuja);
    glutReshapeFunc(inicializaVentana);
    glutKeyboardFunc(letra);
    glutSpecialFunc(especial);
    glutMouseFunc(clickRaton);
    glutMotionFunc(RatonMovido);
    glutTimerFunc(30, idle, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE); // Normalizar automáticamente las normales
    glShadeModel(GL_SMOOTH); // Sombreado suave
    // La iluminación se activa/desactiva con la tecla L

    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    std::cout << "       🎮 CONTROLES DE CÁMARA LIBRE 🎮" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    std::cout << "\n📹 MOVIMIENTO:" << std::endl;
    std::cout << "  w/s         = Adelante / Atrás" << std::endl;
    std::cout << "  a/d         = Izquierda / Derecha" << std::endl;
    std::cout << "  q/e         = Subir / Bajar" << std::endl;
    std::cout << "  Flechas     = Mover (alternativo)" << std::endl;
    std::cout << "  PgUp/PgDn   = Subir/Bajar (alternativo)" << std::endl;
    std::cout << "\n🖱️  RATÓN:" << std::endl;
    std::cout << "  Botón Izq + Arrastrar   = Mirar alrededor" << std::endl;
    std::cout << "  Botón Der + Arrastrar   = Zoom (adelante/atrás)" << std::endl;
    std::cout << "  Botón Central + Arrastrar = Pan (lateral/vertical)" << std::endl;
    std::cout << "\n⚙️  AJUSTES:" << std::endl;
    std::cout << "  [  /  ]     = Reducir/Aumentar velocidad" << std::endl;
    std::cout << "  r           = Resetear cámara" << std::endl;
    std::cout << "\n🎨 VISUALIZACIÓN:" << std::endl;
    std::cout << "  Shift+P     = Modo PUNTOS" << std::endl;
    std::cout << "  Shift+W     = Modo WIREFRAME" << std::endl;
    std::cout << "  Shift+S     = Modo SÓLIDO" << std::endl;
    std::cout << "  L           = Toggle Iluminación (ver pendientes) 💡" << std::endl;
    std::cout << "\n❓ AYUDA:" << std::endl;
    std::cout << "  H           = Mostrar ayuda completa" << std::endl;
    std::cout << "  ESC         = Salir" << std::endl;
    std::cout << "\n💡 TIP: Activa la iluminación (L) para ver el relieve!" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" << std::endl;

    glutMainLoop();
    return 0;
}
