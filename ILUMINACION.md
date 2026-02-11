# 🎨 Sistema de Visualización y Sombreado

## ✅ Cambios Realizados

### 1. **Modos de Visualización Corregidos**

**Problema anterior:** Las teclas W y S estaban sobrecargadas (movimiento de cámara + cambio de modo)

**Solución:** Ahora usamos mayúsculas (Shift) para cambiar modos:

| Tecla | Función |
|-------|---------|
| `w` (minúscula) | Mover cámara adelante |
| `s` (minúscula) | Mover cámara atrás |
| `Shift+W` | Modo WIREFRAME (aristas) |
| `Shift+S` | Modo SÓLIDO |
| `Shift+P` | Modo PUNTOS |

### 2. **Sistema de Iluminación para Ver Pendientes** 💡

**Nueva funcionalidad:** Toggle de iluminación con tecla `L`

- **Iluminación OFF (por defecto):** Color plano, máximo rendimiento
- **Iluminación ON (tecla L):** Sombreado que revela el relieve del terreno

#### ¿Cómo funciona?

Cuando activas la iluminación con `L`:

1. Se calcula cómo la luz (simula el sol) incide en cada superficie
2. Las pendientes orientadas hacia la luz aparecen más brillantes
3. Las pendientes alejadas de la luz aparecen más oscuras
4. Esto crea **contraste de sombras** que revela la topografía real

#### Configuración de la Luz:

```cpp
Posición: (1.0, 2.0, 1.0) - Luz diagonal desde arriba
Tipo: Direccional (simula luz solar)
Color: Cálido y natural
```

## 🎮 Controles Actualizados

### Movimiento de Cámara (minúsculas)
- `w` = Adelante
- `s` = Atrás
- `a` = Izquierda
- `d` = Derecha
- `q` = Subir
- `e` = Bajar
- `r` = Resetear cámara

### Modos de Visualización (Shift + letra)
- `Shift+P` = Modo PUNTOS
- `Shift+W` = Modo WIREFRAME
- `Shift+S` = Modo SÓLIDO

### Iluminación
- `L` = Toggle ON/OFF iluminación
  - OFF: Color plano verde (rápido)
  - ON: Sombreado con relieve (realista)

## 🌄 Comparación Visual

### Sin Iluminación (L desactivada)
```
- Color uniforme verde
- No se perciben pendientes
- Máximo rendimiento
- Bueno para: navegación rápida, análisis de estructura
```

### Con Iluminación (L activada)
```
- Sombreado realista
- Pendientes claramente visibles
- Sombras revelan valles y colinas
- Contraste muestra inclinaciones
- Bueno para: análisis topográfico, presentaciones
```

## 📊 Impacto en Rendimiento

| Modo | Iluminación | FPS Estimado | Uso |
|------|-------------|--------------|-----|
| Puntos | OFF | ~200 | Vista rápida |
| Wireframe | OFF | ~120 | Estructura |
| Sólido | OFF | ~60 | Navegación rápida |
| Sólido | ON | ~50 | **Análisis topográfico** ⭐ |

**Recomendación:** Usa iluminación ON para análisis, OFF para navegación.

## 🎓 ¿Por Qué Se Ven las Pendientes?

### Física de la Iluminación:

1. **Superficies perpendiculares a la luz** → Máximo brillo
2. **Superficies paralelas a la luz** → Oscuras (sombra)
3. **Superficies inclinadas** → Brillo intermedio

### En el Terreno DEM:

- **Laderas orientadas al sol:** Brillantes, verdes claros
- **Laderas opuestas al sol:** Oscuras, verdes oscuros
- **Valles y barrancos:** Sombras pronunciadas
- **Crestas y cimas:** Muy iluminadas

Esto crea **contraste visual** que nuestro cerebro interpreta como relieve 3D.

## 💡 Tips de Uso

### Para Análisis Topográfico:

1. **Activa iluminación** con `L`
2. **Usa modo sólido** (Shift+S)
3. **Vuela alrededor** con w/a/s/d
4. **Observa cambios de luz** al rotar
5. Las áreas más oscuras = pendientes pronunciadas

### Para Identificar Características:

- **Valles:** Líneas oscuras continuas
- **Crestas:** Líneas brillantes en alturas
- **Barrancos:** Sombras profundas y estrechas
- **Mesetas:** Áreas de brillo uniforme
- **Pendientes suaves:** Gradiente gradual de luz

### Para Presentaciones:

1. Empieza con iluminación OFF (vista general)
2. Activa iluminación (tecla L) dramáticamente
3. Rota lentamente para mostrar relieve
4. Usa diferentes modos (P/W/S) para contraste

### Para Verificar Datos:

```
¿Los datos tienen inclinación real?

1. Activa iluminación (L)
2. Rota la cámara 360° con el ratón
3. Si ves cambios de sombra → Hay relieve real ✅
4. Si todo es uniforme → Datos planos o corruptos ❌
```

## 🔧 Ajustes Avanzados

### Cambiar Dirección de la Luz

Edita `lab.cc`, función `Dibuja()`:

```cpp
// Luz desde arriba (mediodía)
GLfloat luz_posicion[] = {0.0f, 1.0f, 0.0f, 0.0f};

// Luz desde el lado (amanecer/atardecer)
GLfloat luz_posicion[] = {1.0f, 0.3f, 0.0f, 0.0f};

// Luz diagonal actual (equilibrado)
GLfloat luz_posicion[] = {1.0f, 2.0f, 1.0f, 0.0f};
```

### Cambiar Intensidad

```cpp
// Más contraste (sombras más dramáticas)
GLfloat luz_ambiente[] = {0.1f, 0.1f, 0.1f, 1.0f};  // Menos luz ambiente
GLfloat luz_difusa[] = {1.0f, 1.0f, 1.0f, 1.0f};    // Más luz directa

// Menos contraste (suave)
GLfloat luz_ambiente[] = {0.5f, 0.5f, 0.5f, 1.0f};  // Más luz ambiente
GLfloat luz_difusa[] = {0.5f, 0.5f, 0.5f, 1.0f};    // Menos luz directa
```

### Cambiar Color del Terreno

```cpp
// Terreno marrón/tierra
GLfloat mat_difuso[] = {0.6f, 0.4f, 0.2f, 1.0f};

// Terreno rocoso gris
GLfloat mat_difuso[] = {0.5f, 0.5f, 0.5f, 1.0f};

// Terreno verde actual
GLfloat mat_difuso[] = {0.4f, 0.6f, 0.3f, 1.0f};
```

## 🎯 Casos de Uso Específicos

### Análisis de Erosión
```
1. Iluminación ON
2. Busca áreas oscuras (barrancos)
3. Modo wireframe para ver detalle
```

### Planificación de Rutas
```
1. Iluminación ON
2. Busca gradientes suaves (caminos fáciles)
3. Evita sombras pronunciadas (pendientes fuertes)
```

### Validación de Datos
```
1. Toggle L on/off rápidamente
2. Compara con/sin sombras
3. Verifica coherencia del relieve
```

### Comunicación Visual
```
1. Captura con iluminación ON
2. Muestra contraste de relieve
3. Más impactante que color plano
```

## 🐛 Solución de Problemas

### No veo cambios al activar iluminación
→ Asegúrate de estar en modo SÓLIDO (Shift+S)
→ Los modos puntos y wireframe no usan iluminación

### Todo se ve muy oscuro
→ La cámara está demasiado cerca
→ Aléjate con `s` o sube con `q`

### No veo pendientes claras
→ Rota la cámara (ratón izquierdo)
→ Las sombras cambian según el ángulo de vista
→ Prueba a moverte alrededor del terreno

### Rendimiento bajo con iluminación
→ Normal, el cálculo de normales es costoso
→ Desactiva con `L` para navegación rápida
→ Reduce submuestreo en lab.cc

## 📈 Resultado Final

**Antes:**
- ❌ W/S sobrecargadas
- ❌ No se veían pendientes
- ❌ Color plano sin contraste

**Ahora:**
- ✅ Controles separados (minúsculas vs mayúsculas)
- ✅ Toggle de iluminación (tecla L)
- ✅ Sombreado revela topografía real
- ✅ Contraste visual de pendientes
- ✅ Perfecto para análisis topográfico

**¡La tecla `L` es tu mejor amiga para ver el relieve real del terreno!** 💡
