# 🎨 Proyecto: Pintura Automática con Greedy + Hill Climbing
**Autor: Carlos Ramírez Valdés**
## 1. Descripción del Proyecto


Este proyecto implementa una metaheurística híbrida (Greedy + Hill Climbing) para resolver un problema de optimización artística: aproximar una imagen objetivo (ej. "Mona Lisa") utilizando un número fijo (modificable) de pinceladas (por ejemplo en este caso será 100).


El objetivo es encontrar la lista de 100 "Strokes" (pinceladas) que minimice el **Error Cuadrático Medio (MSE)** entre la imagen generada y la imagen original.


La estrategia implementada es un algoritmo híbrido de dos fases:

  

1.  **Fase 1: Constructiva (Greedy)**

    Se utiliza una función miope para construir una solución inicial ($s_0$). En cada paso (1 por cada trazo), se prueban 1000 trazos aleatorios y se selecciona "codiciosamente" aquel que produce la mayor reducción inmediata del MSE.

  

2.  **Fase 2: Reparadora (Hill Climbing + Alguna Mejora)**

    Se toma la solución $s_0$ del Greedy y se refina. Se aplica un movimiento de **"Perturbación Aleatoria de Atributo"** (modificando un atributo de un trazo al azar). Se acepta el primer vecino que mejore el MSE (Alguna Mejora), repitiendo el proceso hasta alcanzar un óptimo local (5000 intentos fallidos).



El resultado incluye:
- Imagen final generada.
- Reporte textual del proceso.
- Frames del rendering paso a paso.
- (Opcional) Generación de GIFs y videos usando un script en Python.

## 🧱 Dependencias

### ✔ C++
- g++ (MinGW recomendado en Windows)
- La utilidad `make` (en Windows, esto usualmente se obtiene instalando MinGW-w64 y renombrando `mingw32-make.exe` a `make.exe`).
- Soporte para C++17
- Librerías:
  - `stb_image.h`
  - `stb_image_write.h`
  - `stroke.cpp` / `stroke.h`

### ✔ Python (solo si generas GIFs/videos)
```
pip install imageio
pip install imageio[ffmpeg]
```

Opcional:
```
pip install pillow
```

## ⚙️ Compilación

```
make
```

Genera:

```
testCall.exe
```

## ▶️ Ejecución

```
./testCall.exe "Nombre_imagen".png
```

Por ejemplo:

```
./testCall.exe mona.png
```

## 📁 Estructura Automática de Salida

```
resultados_mona/
    output/
        output_mona.png
        report_mona.txt
    frames_greedy/
    frames_hc/
```

## 🎥 (Opcional) Multimedia 

```
python generar_multimedia.py
```

- Esto hace q se generen gifs de las imágenes de los frames de la fase del greedy y hill climbing por separado

## 🧹 Limpieza

```
make clean
```

## 🧠 Pipeline resumido

1. Carga imagen objetivo  
2. Fase Greedy  
3. Fase Hill Climbing  
4. Exportación de resultados  
5. Multimedia opcional  

## 🧠 Pipeline resumido

1. Carga imagen objetivo  
2. Fase Greedy  
3. Fase Hill Climbing  
4. Exportación de resultados  
5. Multimedia opcional  

---

## 🎨 Resultados Visuales (Mona Lisa)

<p align="center">
  <img src="https://github.com/user-attachments/assets/f9e43468-4dd9-4cb0-be44-3b17301e42cd" width="350" style="margin-right: 20px;">
  <img src="https://github.com/user-attachments/assets/edb0409c-3f3c-4d93-88b4-a56bb139c34f" width="350">
</p>

<p align="center">
  <b>Greedy (izquierda)</b> — <b>Hill Climbing (derecha)</b>
</p>

