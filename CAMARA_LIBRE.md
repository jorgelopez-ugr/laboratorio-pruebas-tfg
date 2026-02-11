# 🎮 Sistema de Cámara Libre - Guía Completa

## 🎯 ¿Qué cambió?

**ANTES (Cámara orbital):**
- ❌ La cámara giraba alrededor del origen (0,0,0)
- ❌ Incómodo para terrenos altos o alejados del origen
- ❌ Solo rotación, sin libertad de movimiento

**AHORA (Cámara libre tipo FPS):**
- ✅ Cámara completamente libre en el espacio 3D
- ✅ Movimiento en todas direcciones (WASD + QE)
- ✅ Rotación intuitiva con el ratón
- ✅ Perfecto para explorar terrenos DEM grandes

## 🎮 Controles Completos

### 📹 **Movimiento de la Cámara**

| Tecla | Acción |
|-------|--------|
| **W** | Avanzar en la dirección que miras |
| **S** | Retroceder |
| **A** | Moverse a la izquierda (strafe) |
| **D** | Moverse a la derecha (strafe) |
| **Q** | Subir verticalmente |
| **E** | Bajar verticalmente |

**Alternativa con flechas:**
- ⬆️ Flecha arriba = Avanzar
- ⬇️ Flecha abajo = Retroceder
- ⬅️ Flecha izquierda = Moverse a la izquierda
- ➡️ Flecha derecha = Moverse a la derecha
- **PgUp** = Subir
- **PgDn** = Bajar

### 🖱️ **Control con Ratón**

| Botón | Acción |
|-------|--------|
| **Botón izquierdo + arrastrar** | Mirar alrededor (como en juegos FPS) |
| **Botón derecho + arrastrar** | Zoom: mover adelante/atrás rápido |
| **Botón central + arrastrar** | Pan: mover lateral y verticalmente |

### ⚙️ **Ajustes de Velocidad**

| Tecla | Acción |
|-------|--------|
| **[** | Reducir velocidad de movimiento (×0.5) |
| **]** | Aumentar velocidad de movimiento (×2) |
| **R** | Resetear cámara a posición inicial |

**Velocidad inicial:** 5 unidades por pulsación
**Rango:** 0.1 - 100 unidades

### 🎨 **Modos de Visualización**

| Tecla | Modo | Descripción |
|-------|------|-------------|
| **P** | Puntos | Nube de puntos (ultra rápido) |
| **1** | Wireframe | Solo aristas (muy rápido) |
| **2** | Sólido | Superficies completas (por defecto) |

### ❓ **Otros Controles**

| Tecla | Acción |
|-------|--------|
| **H** | Mostrar ayuda completa en consola |
| **+** | Avanzar (antiguo control) |
| **-** | Retroceder (antiguo control) |
| **ESC** | Salir del programa |

## 🚀 Consejos de Uso

### Para explorar el terreno DEM:

1. **Empieza con vista general:**
   - Presiona `R` para resetear la cámara
   - Usa `E` para bajar y acercarte al terreno
   - Usa `W/A/S/D` para moverte por el terreno

2. **Examinar detalles:**
   - Reduce la velocidad con `[` para movimientos precisos
   - Usa el ratón para mirar alrededor
   - Cambia a modo wireframe (`1`) para ver la estructura

3. **Vista aérea:**
   - Presiona `Q` varias veces para subir
   - Mira hacia abajo arrastrando el ratón
   - Modo puntos (`P`) para vista general rápida

4. **Navegación rápida:**
   - Aumenta la velocidad con `]`
   - Usa botón derecho del ratón para zoom rápido
   - `W` para avanzar velozmente

## 🎯 Ejemplos de Uso

### Sobrevolar el terreno:
```
1. Presiona Q varias veces para elevarte
2. Mira hacia abajo (ratón izq + arrastrar abajo)
3. Usa W/A/S/D para sobrevolar
```

### Explorar un valle:
```
1. Acércate al valle con W
2. Presiona E para bajar
3. Mira alrededor con el ratón
4. Muévete lateralmente con A/D
```

### Vista panorámica:
```
1. Presiona R para resetear
2. Sube alto con Q (mantén presionado)
3. Cambia a modo puntos (P)
4. Rota con el ratón para ver todo el terreno
```

### Recorrido cinematográfico:
```
1. Ajusta velocidad media (ni muy rápido ni lento)
2. Usa WASD para moverte suavemente
3. Gira lentamente con el ratón
4. Modo sólido (2) para mejor visualización
```

## 🔧 Parámetros Técnicos

### Posición inicial de la cámara:
```cpp
Posición: (0, 100, 200)
Mirando hacia: (0, 0, 0) aprox.
Pitch (inclinación): -15°
Yaw (rotación): 0°
```

### Características:
- **Sensibilidad del ratón:** 0.2° por píxel
- **Velocidad base:** 5 unidades
- **Límite de pitch:** ±89° (no puedes dar vueltas completas)
- **Sin límite de yaw:** rotación horizontal infinita

### Para modificar estos valores:

Edita `visual.cpp`:

```cpp
// Posición inicial (líneas 16-18)
float cam_posY = 100.0f;  // Altura inicial

// Velocidad (línea 32)
float cam_speed = 5.0f;   // Cambia este valor

// Sensibilidad del ratón en mouse.cpp (línea ~97)
float sensitivity = 0.2f;  // Menor = más preciso
```

## 🆘 Solución de Problemas

### Me perdí / No veo el terreno
→ Presiona **R** para resetear la cámara

### La cámara se mueve muy lento
→ Presiona **]** varias veces para aumentar velocidad
→ O usa botón derecho del ratón para zoom rápido

### La cámara se mueve muy rápido
→ Presiona **[** varias veces para reducir velocidad

### No puedo ver bien el terreno
→ Prueba diferentes alturas con Q/E
→ Cambia el modo de visualización (P/1/2)
→ Ajusta la velocidad para movimientos más precisos

### El ratón es muy sensible
→ Edita `mouse.cpp`, línea ~97: reduce `sensitivity = 0.1f;`

### Quiero empezar más cerca del terreno
→ Edita `visual.cpp`, línea 17: reduce `cam_posY = 50.0f;`

## 📊 Comparación de Sistemas

| Característica | Cámara Antigua | Cámara Libre |
|----------------|----------------|--------------|
| Tipo | Orbital (arcball) | Primera persona (FPS) |
| Centro | Origen (0,0,0) | Libre |
| Movimiento | Solo rotación + zoom | 6 grados de libertad |
| Terrenos grandes | ❌ Incómodo | ✅ Perfecto |
| Exploración | ❌ Limitada | ✅ Total |
| Controles | Ratón únicamente | Teclado + Ratón |
| Velocidad ajustable | ❌ No | ✅ Sí ([/]) |

## 🎓 Conceptos Técnicos

### ¿Cómo funciona?

La cámara libre usa:

1. **Posición en 3D:** `(cam_posX, cam_posY, cam_posZ)`
2. **Dirección de vista:** Vector normalizado calculado desde ángulos
3. **Vector "arriba":** Define la orientación (0, 1, 0)
4. **gluLookAt():** Función OpenGL que posiciona la cámara

### Ventajas técnicas:

- **No depende del modelo:** La cámara es independiente del terreno
- **Intuitiva:** Controles tipo juegos (WASD + ratón)
- **Flexible:** Se adapta a cualquier escala de terreno
- **Precisa:** Control fino con ajuste de velocidad

## 💡 Tips Profesionales

1. **Para presentaciones:**
   - Planifica tu recorrido antes
   - Ajusta velocidad lenta para movimientos suaves
   - Usa modo sólido (2) para mejor apariencia

2. **Para análisis:**
   - Modo wireframe (1) para ver estructura
   - Velocidad media para exploración
   - Combina ratón + teclado para precisión

3. **Para screenshots:**
   - Posiciona con teclado
   - Ajusta vista final con ratón
   - Modo puntos (P) para efectos artísticos

4. **Para debugging:**
   - Presiona H para ver ayuda en consola
   - Observa la velocidad actual al ajustar
   - Resetea con R si algo va mal

## 🎉 ¡Disfruta explorando!

El nuevo sistema de cámara te da **libertad total** para explorar tus datos DEM. 
Ya no estás atado al origen - ¡muévete libremente por todo el terreno!

**¿Preguntas?** Presiona `H` en el programa para ayuda rápida.
