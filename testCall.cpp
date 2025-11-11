#include "stroke.h" //
#include <iostream>
#include <random>
#include <vector>     // Necesario para std::vector
#include <string>     // Necesario para std::string
#include <cmath>      // Necesario para pow() y sqrt()
#include <limits>     // Necesario para std::numeric_limits
#include <chrono>     // Necesario para medir tiempo (opcional)
#include <fstream>    // Necesario para operaciones con archivos
// --- Helpers RNG (Generadores de números aleatorios) ---

static std::mt19937 rng(std::random_device{}());

int randInt(int min, int max) { //
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

float randFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

// SOLUCIÓN ALTERNATIVA: Añadir clamp manual
template<typename T>
const T& clamp(const T& v, const T& lo, const T& hi) {
    if (v < lo) return lo;
    if (hi < v) return hi;
    return v;
}


// ===================================================================
// TAREA 1: IMPLEMENTAR FUNCIÓN DE EVALUACIÓN (MSE)
// ===================================================================
double calculateMSE(const Canvas& generated, const Canvas& target) {
    // Asegúrate de que los lienzos tengan el mismo tamaño
    if (generated.width != target.width || generated.height != target.height) {
        return std::numeric_limits<double>::max(); // Error, devuelve el peor costo
    }

    double total_squared_error = 0.0;
    int num_pixels = generated.width * generated.height;

    // Itera por cada píxel (en grupos de 3 bytes: R, G, B)
    for (int i = 0; i < num_pixels * 3; ++i) {
        // Calcula la diferencia (error) entre el píxel generado y el objetivo
        double diff = (double)generated.rgb[i] - (double)target.rgb[i];
        total_squared_error += diff * diff;
    }

    // Retorna el Error Cuadrático Medio (MSE)
    return total_squared_error / (num_pixels * 3);
}

// ===================================================================
// TAREA 2: IMPLEMENTAR HELPERS PARA LA METAHEURÍSTICA
// ===================================================================
Stroke generateRandomStroke() {
    Stroke s;
    s.x_rel = randFloat(0.0f, 1.0f);
    s.y_rel = randFloat(0.0f, 1.0f);
    s.size_rel = randFloat(0.1f, 0.5f); // Evitar trazos demasiado grandes o pequeños
    s.rotation_deg = randFloat(0.0f, 360.0f);
    s.type = randInt(0, 3); // Asumiendo 4 brushes
    s.r = randInt(0, 255);
    s.g = randInt(0, 255);
    s.b = randInt(0, 255);
    return s;
}

// ===================================================================
// TAREA 3: IMPLEMENTAR FASE 1 (GREEDY)
// ===================================================================

/**
 * Genera la solución inicial s0 usando una heurística Greedy.
 * La "Función Miope" aquí es: en cada paso, probar K trazos aleatorios y
 * añadir el que cause la mayor reducción INMEDIATA del MSE.
 */
std::vector<Stroke> runGreedyPhase(const Canvas& C_target, int N_strokes, int K_samples, double& out_greedy_mse) {
    
    std::vector<Stroke> s0; // Solución inicial (vacía)
    
    // Lienzo temporal que se actualiza con cada trazo "fijo"
    Canvas C_current(C_target.width, C_target.height);
    C_current.clear(); // Empieza en blanco

    // El MSE actual que intentamos mejorar
    double best_mse = calculateMSE(C_current, C_target);

    std::cout << "Iniciando Fase Greedy..." << std::endl;
    std::cout << "MSE Inicial (lienzo en blanco): " << best_mse << std::endl;

    // Añadir N_strokes uno por uno
    for (int i = 0; i < N_strokes; ++i) {
        
        Stroke best_stroke_this_step;
        double best_mse_this_step = best_mse;

        // --- Búsqueda Miope: Probar K muestras ---
        for (int k = 0; k < K_samples; ++k) {
            
            Stroke random_stroke = generateRandomStroke();
            
            // 1. Crea un lienzo temporal para probar el trazo
            Canvas C_temp = C_current; // Copia el estado actual
            
            // 2. Dibuja el nuevo trazo de prueba
            random_stroke.draw(C_temp); //
            
            // 3. Evalúa el resultado
            double mse_test = calculateMSE(C_temp, C_target);

            // 4. Compara (Minimizando MSE)
            if (mse_test < best_mse_this_step) {
                best_mse_this_step = mse_test;
                best_stroke_this_step = random_stroke;
            }
        }

        // --- Fin Búsqueda Miope ---
        // Fija el mejor trazo encontrado en este paso
        s0.push_back(best_stroke_this_step);
        best_stroke_this_step.draw(C_current); // Actualiza permanentemente el lienzo actual
        best_mse = best_mse_this_step; // Actualiza el MSE a batir

        std::cout << "Greedy [Stroke " << i+1 << "/" << N_strokes << "] MSE actual: " << best_mse << std::endl;
    }

    std::cout << "Fase Greedy terminada." << std::endl;
    out_greedy_mse = best_mse; // <-- AÑADE ESTA LÍNEA
    return s0; // Retorna la solución inicial
}


// ===================================================================
// TAREA 4: IMPLEMENTAR EL MOVIMIENTO (Generar Vecino)
// ===================================================================

/**
 * Genera una solución vecina (s_vecino) a partir de s_actual.
 * El "Movimiento" consiste en elegir 1 trazo al azar y modificar 1 de sus
 * 7 atributos al azar.
 */
std::vector<Stroke> generateNeighbor(const std::vector<Stroke>& s_actual) {
    std::vector<Stroke> s_vecino = s_actual; // Copia la solución actual

    if (s_vecino.empty()) {
        return s_vecino; // No se puede hacer nada si no hay trazos
    }

    // 1. Seleccionar un trazo aleatorio
    int stroke_idx = randInt(0, s_vecino.size() - 1);

    // 2. Seleccionar un atributo aleatorio (0-6)
    int attribute_idx = randInt(0, 6);

    // 3. Modificar el atributo
    // Usamos 'switch' para aplicar una pequeña perturbación a cada atributo
    switch (attribute_idx) {
        case 0: // x_rel
            s_vecino[stroke_idx].x_rel = clamp(s_vecino[stroke_idx].x_rel + randFloat(-0.1f, 0.1f), 0.0f, 1.0f);
            break;
        case 1: // y_rel
            s_vecino[stroke_idx].y_rel = clamp(s_vecino[stroke_idx].y_rel + randFloat(-0.1f, 0.1f), 0.0f, 1.0f);
            break;
        case 2: // size_rel
            s_vecino[stroke_idx].size_rel = clamp(s_vecino[stroke_idx].size_rel + randFloat(-0.05f, 0.05f), 0.1f, 0.5f);
            break;
        case 3: // rotation_deg
            s_vecino[stroke_idx].rotation_deg = fmod(s_vecino[stroke_idx].rotation_deg + randFloat(-30.0f, 30.0f), 360.0f);
            break;
        case 4: // type
            s_vecino[stroke_idx].type = randInt(0, 3); // Re-aleatorizar tipo de brush
            break;
        case 5: // r (color)
            s_vecino[stroke_idx].r = clamp(s_vecino[stroke_idx].r + randInt(-32, 32), 0, 255);
            break;
        case 6: // g (color)
            s_vecino[stroke_idx].g = clamp(s_vecino[stroke_idx].g + randInt(-32, 32), 0, 255);
            break;
        // (Podríamos añadir 'b' o agrupar r,g,b, pero 7 casos son suficientes)
    }

    return s_vecino;
}

// ===================================================================
// TAREA 5: IMPLEMENTAR FASE 2 (Hill Climbing AM)
// ===================================================================

/**
 * Refina la solución s0 usando Hill Climbing + Alguna Mejora (HC+AM).
 * Criterio de parada: se detiene después de K_ATTEMPTS intentos fallidos de
 * encontrar un vecino mejor (indicando un óptimo local).
 */
std::vector<Stroke> runHillClimbing(
    std::vector<Stroke> s0, 
    const Canvas& C_target, 
    int K_ATTEMPTS,
    int& out_total_iters, 
    double& out_final_mse) 
{
    std::vector<Stroke> s_actual = s0;
    
    // Lienzo para renderizar la solución actual
    Canvas C_current(C_target.width, C_target.height);
    render(s_actual, C_current); //
    
    // Evaluar la solución inicial
    double mse_actual = calculateMSE(C_current, C_target);
    std::cout << "Iniciando Fase HC+AM..." << std::endl;
    std::cout << "HC [Iter 0] MSE inicial: " << mse_actual << std::endl;

    // Lienzo temporal para probar vecinos
    Canvas C_neighbor(C_target.width, C_target.height);

    int attempts_failed = 0; // Contador de intentos fallidos
    int iter = 1;

    while (attempts_failed < K_ATTEMPTS) {
        
        // 1. Generar un vecino aleatorio
        std::vector<Stroke> s_vecino = generateNeighbor(s_actual);
        
        // 2. Renderizar y Evaluar el vecino
        render(s_vecino, C_neighbor);
        double mse_vecino = calculateMSE(C_neighbor, C_target);

        // 3. Criterio de Selección (Alguna Mejora)
        // Estamos minimizando MSE, así que buscamos un costo menor.
        if (mse_vecino < mse_actual) {
            // ¡Mejora! Aceptamos el vecino
            s_actual = s_vecino;
            mse_actual = mse_vecino;
            
            // Reiniciamos el contador de fallos
            attempts_failed = 0;
            
            std::cout << "HC [Iter " << iter << "] ¡Mejora! MSE: " << mse_actual << std::endl;
        } else {
            // No mejora. Descartamos el vecino e incrementamos el contador.
            attempts_failed++;
        }
        iter++;
    }

    std::cout << "Fase HC+AM terminada (Óptimo Local...)." << std::endl;
    out_total_iters = iter;         // 
    out_final_mse = mse_actual;     // 
    return s_actual; // Devuelve la mejor solución (el óptimo local)
}


// ===================================================================
// PROGRAMA PRINCIPAL
// ===================================================================
int main() {
    // <--- INICIAR CRONÓMETRO ---
    auto start_time = std::chrono::high_resolution_clock::now();

    // 1) Cargar brushes (igual que antes)
    {
        ImageGray b0, b1, b2, b3;
        if (!loadImageGray("brushes/1.jpg", b0)) return 1;
        if (!loadImageGray("brushes/2.jpg", b1)) return 1;
        if (!loadImageGray("brushes/3.jpg", b2)) return 1;
        if (!loadImageGray("brushes/4.jpg", b3)) return 1;
        gBrushes.push_back(std::move(b0));
        gBrushes.push_back(std::move(b1));
        gBrushes.push_back(std::move(b2));
        gBrushes.push_back(std::move(b3));
    }

    // 2) Cargar la imagen OBJETIVO
    std::string target_name = "starrynight.png"; // ¡Puedes cambiar esto!
    Canvas C_target(1, 1);
    if (!loadImageRGB_asCanvas("instancias/" + target_name, C_target)) { //
        std::cerr << "Error cargando instancia " << target_name << "\n";
        return 1;
    }
    std::cout << "Instancia cargada: " << target_name << " (" << C_target.width << "x" << C_target.height << ")\n";

// 3) Crear lienzo principal (igual que antes)
    Canvas C_generated(C_target.width, C_target.height);
    
    // --- Definir Parámetros de la Metaheurística ---
    int N_STROKES = 100;         // Número total de pinceladas a usar
    int K_SAMPLES_GREEDY = 1000; // Muestras por paso del Greedy (Función Miope)
    int K_ATTEMPTS_HC = 5000;    // Intentos fallidos antes de detener HC

    // ======================== EXTRAS ====================================
    // --- Variables para el reporte ---
    double greedy_mse = 0.0;
    int total_hc_iters = 0;
    double final_mse = 0.0;

    // ===================================================================
    // EJECUCIÓN DEL ALGORITMO
    // ===================================================================

    // --- FASE 1: CONSTRUIR SOLUCIÓN INICIAL (Greedy) ---
    //std::vector<Stroke> s0 = runGreedyPhase(C_target, N_STROKES, K_SAMPLES_GREEDY);
    std::vector<Stroke> s0 = runGreedyPhase(C_target, N_STROKES, K_SAMPLES_GREEDY, greedy_mse);
    
    // --- FASE 2: REFINAR SOLUCIÓN (Hill Climbing AM) ---
    //std::vector<Stroke> s_final = runHillClimbing(s0, C_target, K_ATTEMPTS_HC);
    std::vector<Stroke> s_final = runHillClimbing(s0, C_target, K_ATTEMPTS_HC, total_hc_iters, final_mse);

    // Por ahora, nuestra solución final es la que salió del Greedy
    //std::vector<Stroke> s_final = s0;

    // 4) Renderiza (pinta) la solución final en el lienzo C_generated
    render(s_final, C_generated); //

    // 5) Guardar
    if (!savePNG(C_generated, "output.png")) { //
        std::cerr << "Error guardando output.png\n";
        return 1;
    }
    std::cout << "OK: guardado output.png\n";

    // <--- DETENER CRONÓMETRO Y GUARDAR REPORTE ---
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::ofstream report_file("output_report.txt");
    report_file << "--- Reporte de Ejecucion ---\n";
    report_file << "Instancia: " << target_name << "\n";
    report_file << "Tiempo Total: " << elapsed.count() << " segundos (" << elapsed.count() / 60.0 << " min)\n";
    report_file << "\n--- Parametros Usados ---\n";
    report_file << "N_STROKES: " << N_STROKES << "\n";
    report_file << "K_SAMPLES_GREEDY: " << K_SAMPLES_GREEDY << "\n";
    report_file << "K_ATTEMPTS_HC: " << K_ATTEMPTS_HC << "\n";
    report_file << "\n--- Resultados de Optimizacion ---\n";
    report_file << "MSE post-Greedy (Inicial): " << greedy_mse << "\n";
    report_file << "MSE post-HC (Final): " << final_mse << "\n";
    report_file << "Iteraciones HC Totales: " << total_hc_iters << "\n";
    report_file.close();

    // (Imprime el tiempo en consola también)
    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "TIEMPO TOTAL DE EJECUCION: " << elapsed.count() << " segundos." << std::endl;
    std::cout << "Reporte 'output_report.txt' guardado." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    return 0;
}
