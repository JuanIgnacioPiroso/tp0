#!/bin/bash

# ================================================================
# SCRIPT DE INSTALACIÓN - TESTS UNITARIOS TP0 SO
# ================================================================
# 
# Este script automatiza la instalación de dependencias necesarias
# para ejecutar los tests unitarios del TP0.

set -e  # Salir si cualquier comando falla

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Funciones de utilidad
print_info() {
    echo -e "${BLUE}ℹ️  $1${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_header() {
    echo ""
    echo -e "${BLUE}================================================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}================================================================${NC}"
    echo ""
}

# Verificar si estamos en Ubuntu/Debian
check_os() {
    if [[ -f /etc/os-release ]]; then
        . /etc/os-release
        OS=$NAME
        VER=$VERSION_ID
    else
        print_error "No se puede determinar el sistema operativo"
        exit 1
    fi
    
    print_info "Sistema detectado: $OS $VER"
}

# Actualizar repositorios
update_repos() {
    print_info "Actualizando repositorios del sistema..."
    sudo apt-get update -qq
    print_success "Repositorios actualizados"
}

# Instalar dependencias básicas
install_basic_deps() {
    print_info "Instalando dependencias básicas..."
    sudo apt-get install -y build-essential git cmake pkg-config libreadline-dev
    print_success "Dependencias básicas instaladas"
}

# Instalar so-commons-library
install_commons() {
    print_info "Instalando so-commons-library..."
    
    # Crear directorio temporal
    TEMP_DIR=$(mktemp -d)
    cd "$TEMP_DIR"
    
    # Clonar y compilar
    git clone https://github.com/sisoputnfrba/so-commons-library.git
    cd so-commons-library
    make
    sudo make install
    
    # Actualizar cache de librerías
    sudo ldconfig
    
    # Limpiar
    cd /
    rm -rf "$TEMP_DIR"
    
    print_success "so-commons-library instalada"
}

# Instalar CSpec
install_cspec() {
    print_info "Instalando CSpec framework..."
    
    # Crear directorio temporal
    TEMP_DIR=$(mktemp -d)
    cd "$TEMP_DIR"
    
    # Clonar y compilar
    git clone https://github.com/mumuki/cspec.git
    cd cspec
    make
    sudo make install
    
    # Actualizar cache de librerías
    sudo ldconfig
    
    # Limpiar
    cd /
    rm -rf "$TEMP_DIR"
    
    print_success "CSpec framework instalado"
}

# Verificar instalación
verify_installation() {
    print_info "Verificando instalación..."
    
    # Verificar CSpec
    if pkg-config --exists cspecs 2>/dev/null; then
        print_success "CSpec: Disponible"
    else
        print_warning "CSpec: No se puede verificar con pkg-config, pero podría estar instalado"
    fi
    
    # Verificar Commons
    if pkg-config --exists commons 2>/dev/null; then
        print_success "Commons: Disponible"
    else
        print_warning "Commons: No se puede verificar con pkg-config, pero podría estar instalado"
    fi
    
    # Verificar GCC
    if which gcc > /dev/null 2>&1; then
        GCC_VERSION=$(gcc --version | head -n1)
        print_success "GCC: $GCC_VERSION"
    else
        print_error "GCC: No encontrado"
        return 1
    fi
}

# Compilar tests de prueba
test_compilation() {
    print_info "Probando compilación de tests..."
    
    # Volver al directorio de tests
    cd "$(dirname "$0")"
    
    # Intentar compilar
    if make build > /dev/null 2>&1; then
        print_success "Compilación de tests: Exitosa"
        make clean > /dev/null 2>&1
    else
        print_warning "Compilación de tests: Falló (revisar dependencias manualmente)"
        return 1
    fi
}

# Función principal
main() {
    print_header "INSTALADOR DE DEPENDENCIAS PARA TESTS TP0 SO"
    
    # Verificar que estamos ejecutando como usuario normal (no root)
    if [[ $EUID -eq 0 ]]; then
        print_error "No ejecutes este script como root. Usaremos sudo cuando sea necesario."
        exit 1
    fi
    
    # Verificar OS
    check_os
    
    # Preguntar al usuario si quiere continuar
    echo ""
    print_info "Este script instalará:"
    echo "  • Herramientas de compilación (build-essential)"
    echo "  • so-commons-library (desde GitHub)"
    echo "  • CSpec framework (desde GitHub)"
    echo "  • Dependencias adicionales"
    echo ""
    read -p "¿Continuar con la instalación? (y/N): " -n 1 -r
    echo ""
    
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        print_info "Instalación cancelada por el usuario"
        exit 0
    fi
    
    # Ejecutar instalación
    echo ""
    update_repos
    install_basic_deps
    install_commons
    install_cspec
    
    print_header "VERIFICACIÓN DE INSTALACIÓN"
    verify_installation
    
    # Probar compilación si estamos en el directorio correcto
    if [[ -f "Makefile" ]]; then
        test_compilation
    else
        print_warning "No se encontró Makefile. Ejecuta 'cd tests && make build' para probar."
    fi
    
    print_header "INSTALACIÓN COMPLETADA"
    print_success "¡Todas las dependencias han sido instaladas!"
    echo ""
    print_info "Próximos pasos:"
    echo "  1. cd tests/"
    echo "  2. make test"
    echo ""
    print_info "Para más información, consulta tests/README.md"
}

# Función de ayuda
show_help() {
    echo "INSTALADOR DE DEPENDENCIAS - TESTS TP0 SO"
    echo "========================================="
    echo ""
    echo "Este script instala automáticamente todas las dependencias"
    echo "necesarias para compilar y ejecutar los tests unitarios."
    echo ""
    echo "USO:"
    echo "  ./setup.sh        - Ejecutar instalación completa"
    echo "  ./setup.sh -h     - Mostrar esta ayuda"
    echo "  ./setup.sh --help - Mostrar esta ayuda"
    echo ""
    echo "DEPENDENCIAS QUE SE INSTALARÁN:"
    echo "  • build-essential (gcc, make, etc.)"
    echo "  • so-commons-library"
    echo "  • CSpec framework"
    echo "  • libreadline-dev"
    echo "  • pkg-config"
    echo ""
    echo "REQUISITOS:"
    echo "  • Sistema Ubuntu/Debian"
    echo "  • Acceso a sudo"
    echo "  • Conexión a Internet"
    echo ""
}

# Manejar argumentos de línea de comandos
case "${1:-}" in
    -h|--help)
        show_help
        exit 0
        ;;
    "")
        main
        ;;
    *)
        print_error "Argumento desconocido: $1"
        print_info "Usa './setup.sh -h' para ver la ayuda"
        exit 1
        ;;
esac
