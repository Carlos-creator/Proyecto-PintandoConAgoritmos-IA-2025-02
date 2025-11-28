# Makefile para Proyecto IA: Pintando con Algoritmos

# Forzar el uso de la consola de Windows para que funcionen los comandos 'del' y 'rd'
SHELL = cmd.exe

# Compilador
CXX = g++

# Flags de compilación:
# -std=c++17: Necesario para std::clamp (aunque lo implementamos manual)
# -Wall -Wextra: Muestra todas las advertencias
# -g3: Incluye información de debugging
# -O3: Optimiza el código para velocidad (más rápido)
CXXFLAGS = -std=c++17 -Wall -Wextra -g3 -O3

# Archivos fuente
SOURCES = testCall.cpp stroke.cpp

# Nombre del ejecutable final
TARGET = testCall.exe

# Regla principal: se ejecuta con 'make' o 'make all'
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)
	@echo "Compilacion exitosa. Ejecutable creado: $(TARGET)"

# Regla de limpieza: se ejecuta con 'make clean'
# MODIFICADO PARA USAR 'del' DE WINDOWS
# Elimina el .exe, los reportes sueltos y TODAS las carpetas 'resultados_*'
clean:
	@echo "Limpiando archivos generados..."
	-@del $(TARGET) 2> nul
	-@del output_*.png 2> nul
	-@del report_*.txt 2> nul
	@echo "Eliminando carpetas de resultados..."
	-@for /d %%d in (resultados_*) do rd /s /q "%%d"
	@echo "Limpieza completada."

.PHONY: all clean