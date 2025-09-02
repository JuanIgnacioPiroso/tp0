# ================================================================
# MAKEFILE PARA TESTS UNITARIOS - TP0 SISTEMAS OPERATIVOS
# ================================================================
# 
# Este makefile compila y ejecuta los tests unitarios usando CSpec
# para las funciones del cliente y servidor TCP.

# ========== CONFIGURACIÓN DEL COMPILADOR ==========

CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -D_GNU_SOURCE
INCLUDES = -I../src -I../server/src -I/usr/local/include
LIBDIRS = -L/usr/local/lib
LIBS = -lcspecs -lcommons -lpthread -lreadline -lm

# ========== DIRECTORIOS ==========

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
CLIENT_SRC_DIR = ../src
SERVER_SRC_DIR = ../server/src

# ========== ARCHIVOS FUENTE ==========

# Tests
TEST_SOURCES = $(wildcard $(SRC_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Código fuente del cliente (excluyendo main)
CLIENT_SOURCES = $(filter-out $(CLIENT_SRC_DIR)/client.c, $(wildcard $(CLIENT_SRC_DIR)/*.c))
CLIENT_OBJECTS = $(CLIENT_SOURCES:$(CLIENT_SRC_DIR)/%.c=$(OBJ_DIR)/client_%.o)

# Código fuente del servidor (excluyendo main)
SERVER_SOURCES = $(filter-out $(SERVER_SRC_DIR)/server.c, $(wildcard $(SERVER_SRC_DIR)/*.c))
SERVER_OBJECTS = $(SERVER_SOURCES:$(SERVER_SRC_DIR)/%.c=$(OBJ_DIR)/server_%.o)

# ========== TARGETS PRINCIPALES ==========

.PHONY: all clean test help install-deps

# Target por defecto
all: test

# Compilar y ejecutar tests
test: $(BIN_DIR)/test_runner
	@echo ""
	@echo "🚀 Ejecutando tests unitarios..."
	@echo "=================================="
	@./$(BIN_DIR)/test_runner
	@echo ""

# Compilar el ejecutable de tests
$(BIN_DIR)/test_runner: $(TEST_OBJECTS) $(CLIENT_OBJECTS) $(SERVER_OBJECTS) | $(BIN_DIR)
	@echo "🔗 Enlazando ejecutable de tests..."
	$(CC) $(CFLAGS) -o $@ $^ $(LIBDIRS) $(LIBS)
	@echo "✅ Ejecutable de tests creado: $@"

# ========== COMPILACIÓN DE OBJETOS ==========

# Compilar tests
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "🔨 Compilando test: $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compilar código fuente del cliente
$(OBJ_DIR)/client_%.o: $(CLIENT_SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "🔨 Compilando cliente: $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compilar código fuente del servidor
$(OBJ_DIR)/server_%.o: $(SERVER_SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "🔨 Compilando servidor: $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ========== CREACIÓN DE DIRECTORIOS ==========

$(OBJ_DIR):
	@echo "📁 Creando directorio de objetos..."
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@echo "📁 Creando directorio de binarios..."
	mkdir -p $(BIN_DIR)

# ========== LIMPIEZA ==========

clean:
	@echo "🧹 Limpiando archivos generados..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -f *.log test_*.config test_*.log
	@echo "✅ Limpieza completada"

# Limpieza profunda (incluye logs de ejecución)
clean-all: clean
	@echo "🧹 Limpieza profunda..."
	rm -f tp0.log log.log
	@echo "✅ Limpieza profunda completada"

# ========== INSTALACIÓN DE DEPENDENCIAS ==========

install-deps:
	@echo "📦 Instalando dependencias..."
	@echo ""
	@echo "Para Ubuntu/Debian:"
	@echo "  sudo apt-get update"
	@echo "  sudo apt-get install libcspecs-dev libcommons-dev"
	@echo ""
	@echo "Para compilar CSpec desde fuente:"
	@echo "  git clone https://github.com/mumuki/cspec.git"
	@echo "  cd cspec"
	@echo "  make"
	@echo "  sudo make install"
	@echo ""
	@echo "Para so-commons-library:"
	@echo "  git clone https://github.com/sisoputnfrba/so-commons-library.git"
	@echo "  cd so-commons-library"
	@echo "  make"
	@echo "  sudo make install"
	@echo ""

# ========== TARGETS DE AYUDA Y DEBUG ==========

help:
	@echo "MAKEFILE DE TESTS - TP0 SISTEMAS OPERATIVOS"
	@echo "==========================================="
	@echo ""
	@echo "Targets disponibles:"
	@echo ""
	@echo "  all           - Compilar y ejecutar todos los tests (default)"
	@echo "  test          - Compilar y ejecutar tests"
	@echo "  clean         - Limpiar archivos generados"
	@echo "  clean-all     - Limpieza profunda (incluye logs)"
	@echo "  install-deps  - Mostrar instrucciones para instalar dependencias"
	@echo "  debug         - Mostrar información de debug"
	@echo "  help          - Mostrar esta ayuda"
	@echo ""
	@echo "Estructura de archivos:"
	@echo "  tests/src/    - Código fuente de los tests"
	@echo "  tests/obj/    - Archivos objeto (generado)"
	@echo "  tests/bin/    - Ejecutables (generado)"
	@echo ""

debug:
	@echo "INFORMACIÓN DE DEBUG"
	@echo "==================="
	@echo ""
	@echo "Compilador: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Includes: $(INCLUDES)"
	@echo "Librerías: $(LIBS)"
	@echo ""
	@echo "Archivos de test encontrados:"
	@$(foreach file,$(TEST_SOURCES),echo "  $(file)";)
	@echo ""
	@echo "Archivos del cliente encontrados:"
	@$(foreach file,$(CLIENT_SOURCES),echo "  $(file)";)
	@echo ""
	@echo "Archivos del servidor encontrados:"
	@$(foreach file,$(SERVER_SOURCES),echo "  $(file)";)
	@echo ""

# ========== TARGETS ESPECÍFICOS ==========

# Ejecutar solo tests del cliente
test-client: $(BIN_DIR)/test_runner
	@echo "🔧 Ejecutando solo tests del cliente..."
	@./$(BIN_DIR)/test_runner | grep -A 50 "TESTS DEL CLIENTE" | grep -B 50 "TESTS DEL SERVIDOR" | head -n -2

# Ejecutar solo tests del servidor
test-server: $(BIN_DIR)/test_runner
	@echo "🖥️  Ejecutando solo tests del servidor..."
	@./$(BIN_DIR)/test_runner | grep -A 50 "TESTS DEL SERVIDOR"

# Compilar sin ejecutar
build: $(BIN_DIR)/test_runner
	@echo "✅ Tests compilados exitosamente"

# Verificar que las dependencias están instaladas
check-deps:
	@echo "🔍 Verificando dependencias..."
	@echo -n "CSpec: "
	@pkg-config --exists cspecs && echo "✅ Encontrado" || echo "❌ No encontrado"
	@echo -n "Commons: "
	@pkg-config --exists commons && echo "✅ Encontrado" || echo "❌ No encontrado"
	@echo -n "GCC: "
	@which gcc > /dev/null && echo "✅ Encontrado" || echo "❌ No encontrado"
