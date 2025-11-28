import os
import imageio.v2 as imageio
import sys

def generate_visuals(result_folder):
    """
    Busca frames dentro de la carpeta de resultados y genera GIFs/MP4s ahí mismo.
    """
    print(f"\n🎨 Procesando carpeta: {result_folder}...")
    
    # Rutas esperadas
    path_greedy = os.path.join(result_folder, "frames_greedy")
    path_hc = os.path.join(result_folder, "frames_hc")
    
    # 1. Generar Greedy
    if os.path.exists(path_greedy):
        frames = sorted([f for f in os.listdir(path_greedy) if f.endswith('.png')])
        if frames:
            print(f"   -> Generando GIF Greedy ({len(frames)} frames)...")
            imgs = [imageio.imread(os.path.join(path_greedy, f)) for f in frames]
            
            # Guardar GIF en la raíz de la carpeta de resultados
            imageio.mimsave(os.path.join(result_folder, "greedy.gif"), imgs, duration=0.03)
            # Guardar MP4 (Opcional, descomentar si tienes ffmpeg instalado)
            # imageio.mimsave(os.path.join(result_folder, "video_greedy.mp4"), imgs, fps=30)
    
    # 2. Generar HC
    if os.path.exists(path_hc):
        frames = sorted([f for f in os.listdir(path_hc) if f.endswith('.png')])
        if frames:
            print(f"   -> Generando GIF HC ({len(frames)} frames)...")
            imgs = [imageio.imread(os.path.join(path_hc, f)) for f in frames]
            
            imageio.mimsave(os.path.join(result_folder, "hc.gif"), imgs, duration=0.03)
            # imageio.mimsave(os.path.join(result_folder, "video_hc.mp4"), imgs, fps=30)

    print("✅ Listo.")

if __name__ == "__main__":
    # Si se pasa un argumento (ej: python generar_multimedia.py resultados_mona)
    if len(sys.argv) > 1:
        folder = sys.argv[1]
        if os.path.exists(folder):
            generate_visuals(folder)
        else:
            print(f"Error: La carpeta {folder} no existe.")
    else:
        # Modo automático: Busca TODAS las carpetas que empiecen con "resultados_"
        print("🔍 Buscando carpetas 'resultados_' automáticamente...")
        dirs = [d for d in os.listdir(".") if os.path.isdir(d) and d.startswith("resultados_")]
        
        if not dirs:
            print("No se encontraron carpetas de resultados.")
        
        for d in dirs:
            generate_visuals(d)