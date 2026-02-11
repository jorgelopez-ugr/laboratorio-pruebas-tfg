# Visor de DEM (Digital Elevation Model) - OpenGL

## Instalación de dependencias

### Instalar GDAL (Geospatial Data Abstraction Library)

En Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install libgdal-dev gdal-bin
```

En Fedora/RHEL:
```bash
sudo dnf install gdal-devel
```

### Verificar instalación
```bash
gdal-config --version
gdal-config --cflags
gdal-config --libs
```

## Compilación

```bash
make clean
make
```

## Uso

### Con archivo DEM (GeoTIFF):
```bash
./lab <archivo.tif>
```

Ejemplo:
```bash
./lab mi_terreno.tif
./lab /ruta/completa/al/dem_data.tif
```

### Sin archivo (terreno sintético de prueba):
```bash
./lab
```

## Controles

- **Botón izquierdo del ratón**: Rotar el modelo
- **Botón derecho del ratón**: Zoom (mover arriba/abajo)

## Formatos soportados

La función `leerDEMdesdeTIFF()` soporta:

- **GeoTIFF** (.tif, .tiff) - formato más común para datos DEM
- Tipos de datos: Float32, Int16, UInt16, Int32, etc. (conversión automática)
- Valores NoData: Se detectan y manejan automáticamente
- Georreferenciación: Se lee la resolución del píxel si está disponible

## Información técnica

El programa:
1. Lee el archivo TIFF usando GDAL
2. Extrae las dimensiones (ancho × alto)
3. Lee la georreferenciación para calcular la escala real
4. Carga los datos de elevación
5. Genera una malla 3D de triángulos
6. Calcula normales para iluminación suave
7. Renderiza con OpenGL

## Solución de problemas

### Error: "cannot open source file gdal/gdal_priv.h"
Instala GDAL:
```bash
sudo apt-get install libgdal-dev
```

### Error al ejecutar: "No se pudo abrir el archivo"
Verifica:
- Que el archivo existe
- Que la ruta es correcta (absoluta o relativa)
- Que el archivo es un GeoTIFF válido

### El terreno se ve muy plano o muy empinado
La escala se lee automáticamente del GeoTIFF. Si quieres ajustarla manualmente,
modifica el parámetro `escala` en la función `leerDEMdesdeTIFF()`.

## Ejemplos de datasets DEM

Puedes descargar datos DEM gratuitos de:
- **USGS Earth Explorer**: https://earthexplorer.usgs.gov/
- **OpenTopography**: https://opentopography.org/
- **Copernicus DEM**: https://spacedata.copernicus.eu/
- **SRTM**: https://www2.jpl.nasa.gov/srtm/

Busca archivos en formato GeoTIFF para mejor compatibilidad.
