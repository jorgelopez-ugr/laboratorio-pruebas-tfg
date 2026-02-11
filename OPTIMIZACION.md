# Guía de Optimización del Visor DEM

## 🚀 Cambios realizados para mejorar el rendimiento

### 1. **Submuestreo de datos**
El archivo DEM original puede tener millones de puntos. El submuestreo reduce la resolución:

```cpp
int submuestreo = 5; // En lab.cc, línea ~185
```

**Valores recomendados:**
- `1` = Resolución completa (LENTO, solo para DEMs pequeños < 500×500)
- `2` = Mitad de resolución (Bueno para visualización)
- `5` = 1/5 de resolución (RÁPIDO, valor por defecto)
- `10` = 1/10 de resolución (MUY RÁPIDO, para previsualización)

**Tu DEM:** 1598×1393 píxeles → Con submuestreo 5 = 319×278 = ~88K puntos

### 2. **Eliminación de iluminación**
- ❌ Eliminado cálculo de normales (ahorra ~30-50% tiempo de carga)
- ❌ Deshabilitado GL_LIGHTING (mejor framerate)
- ✅ Color sólido simple

### 3. **Modos de visualización**

| Tecla | Modo | Rendimiento | Uso |
|-------|------|-------------|-----|
| **P** | Puntos | ⚡⚡⚡ ULTRA RÁPIDO | Nube de puntos, vista general |
| **W** | Wireframe | ⚡⚡ MUY RÁPIDO | Ver estructura de malla |
| **S** | Sólido | ⚡ RÁPIDO | Visualización completa |

### 4. **Gestión de valores NoData**
Los valores -9999 (sin datos) se reemplazan por 0 para evitar deformaciones.

## 📊 Tabla de rendimiento estimado

| Resolución original | Submuestreo | Vértices | Triángulos | FPS estimado |
|---------------------|-------------|----------|------------|--------------|
| 1598×1393 | 1 | 2.2M | 4.4M | < 5 fps ⚠️ |
| 1598×1393 | 2 | 550K | 1.1M | ~15 fps |
| 1598×1393 | 5 | 88K | 176K | ~60 fps ✅ |
| 1598×1393 | 10 | 22K | 44K | > 100 fps |

## 🔧 Cómo ajustar el submuestreo

Edita `lab.cc`, línea ~185:

```cpp
int submuestreo = 5; // Cambia este valor
```

**Valores para diferentes casos:**
- **Presentación/demostración:** `submuestreo = 5` o `10`
- **Análisis visual general:** `submuestreo = 3` o `4`
- **Análisis detallado:** `submuestreo = 2`
- **Máxima calidad** (solo si tu GPU es potente): `submuestreo = 1`

Luego recompila:
```bash
make
```

## 💡 Consejos adicionales

### Si sigue siendo lento:
1. **Aumenta el submuestreo** a 10 o más
2. **Usa modo PUNTOS** (tecla P) en lugar de sólido
3. Recorta tu DEM a un área más pequeña con GDAL:
   ```bash
   gdal_translate -srcwin 0 0 500 500 entrada.tif salida.tif
   ```

### Para mejorar la calidad visual:
1. Activa iluminación en `lab.cc`:
   ```cpp
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   ```
2. Habilita cálculo de normales:
   ```cpp
   mallaDEM.generarDesdeDEM(ancho, alto, escala, alturas, true);
   //                                                      ^^^^
   ```
3. Usa `drawSmooth()` en lugar de `drawSolid()`

### Para mejor navegación:
- **Ratón izquierdo**: Arrastra para rotar
- **Ratón derecho**: Arrastra arriba/abajo para zoom
- **+/-**: Zoom con teclado
- **Flechas**: Rotar con teclado

## 🎨 Cambiar colores

En `lab.cc`, función `Dibuja()`:

```cpp
// Color del terreno
glColor3f(0.3, 0.7, 0.4); // Verde (R, G, B)

// Otros ejemplos:
// glColor3f(0.8, 0.5, 0.2); // Tierra/arena
// glColor3f(0.5, 0.5, 0.5); // Gris
// glColor3f(0.2, 0.4, 0.8); // Azul agua
```

## 📈 Estadísticas de tu DEM actual

```
Archivo: DEM_PuertoLaMata_08-04-2024.tif
Dimensiones: 1598 × 1393 píxeles
Resolución: ~2.5 metros/píxel
Rango altitud: 0 - 1743 metros
Submuestreo 1:5 → 88,682 vértices, 176,172 triángulos
Rendimiento esperado: ~60 fps (modo sólido)
```

## 🐛 Solución de problemas

### Pantalla negra o no se ve nada
- Aumenta el zoom (ratón derecho hacia abajo o tecla `-`)
- Los valores NoData pueden estar causando problemas de escala

### Muy lento incluso con submuestreo
- Tu GPU puede ser antigua. Prueba modo PUNTOS (tecla P)
- Aumenta submuestreo a 10 o 20

### Terreno deformado o con picos raros
- Valores NoData mal manejados
- Verifica las estadísticas al cargar el archivo
