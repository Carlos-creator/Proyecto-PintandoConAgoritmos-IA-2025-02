# 📘 Utilidades Multimedia para Proyecto de Pinceladas (Greedy + HC)

Este notebook (`utilidades_multimedia.ipynb`) permite:

## ✅ Funciones disponibles
### 1. Crear VIDEO desde frames
Convierte `frames_hc/` o `frames_greedy/` en un `.mp4`.

### 2. Crear GIF
Ideal para informes, presentaciones o visualizar rápidamente la ejecución.

### 3. Ver animación frame a frame
Reproduce cada imagen secuencialmente dentro del notebook.

---

## 📦 Requisitos

Ejecutar:

```
pip install imageio
pip install "imageio[ffmpeg]"
```

(Esto permite exportar MP4 y GIF sin errores.)

---

## ▶️ Cómo usar el notebook

Al ejecutar el notebook, aparecerá un selector:

```
¿Qué deseas hacer? (video / gif / anim / salir):
```

Luego te pedirá:

- carpeta de frames
- nombre de archivo de salida
- FPS o duración por frame (según corresponda)

---

## 🗂️ Estructura recomendada

```
Proyecto/
│ utilidades_multimedia.ipynb
│ frames_greedy/
│     frame_00001.png
│     ...
│ frames_hc/
      frame_00001.png
      ...
```

---

## 🎨 Consejos
- Para GIF rápidos, usa `duration = 0.03`.
- Para videos fluidos, usa `fps = 30`.
- Si tienes miles de frames, generar MP4 es más rápido que GIF.

---
