#include <cspecs/cspec.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @file test_runner.c
 * @brief Archivo principal para ejecutar todos los tests del proyecto
 *
 * Este archivo importa y ejecuta todos los contextos de test definidos
 * para el cliente y servidor TCP. Utiliza CSpec para generar reportes
 * de los resultados de las pruebas.
 */

// ========== DECLARACIÓN DE CONTEXTOS DE TEST ==========

// Tests del cliente
extern context(test_paquetes);
extern context(test_configuracion);
extern context(test_logging);
extern context(test_mensajes);
extern context(test_auxiliares);

// Tests del servidor
extern context(test_server_logging);
extern context(test_server_data_handling);
extern context(test_operation_codes);
extern context(test_buffer_simulation);
extern context(test_constants);

/**
 * @brief Función principal del runner de tests
 *
 * Ejecuta todos los contextos de test y muestra un resumen de los resultados.
 * Retorna 0 si todos los tests pasan, 1 si alguno falla.
 *
 * @return int Código de salida (0 = éxito, 1 = fallos)
 */
int main()
{
    printf("\n");
    printf("================================================================\n");
    printf("           EJECUTANDO TESTS UNITARIOS - TP0 SO                 \n");
    printf("================================================================\n");
    printf("\n");

    // ========== EJECUTAR TESTS DEL CLIENTE ==========

    printf("🔧 TESTS DEL CLIENTE\n");
    printf("--------------------\n");

    printf("📦 Ejecutando tests de paquetes...\n");
    cspec_run_context(test_paquetes, "", "");

    printf("\n⚙️  Ejecutando tests de configuración...\n");
    cspec_run_context(test_configuracion, "", "");

    printf("\n📝 Ejecutando tests de logging...\n");
    cspec_run_context(test_logging, "", "");

    printf("\n💬 Ejecutando tests de mensajes...\n");
    cspec_run_context(test_mensajes, "", "");

    printf("\n🧹 Ejecutando tests auxiliares...\n");
    cspec_run_context(test_auxiliares, "", "");

    // ========== EJECUTAR TESTS DEL SERVIDOR ==========

    printf("\n");
    printf("🖥️  TESTS DEL SERVIDOR\n");
    printf("----------------------\n");

    printf("📝 Ejecutando tests de logging del servidor...\n");
    cspec_run_context(test_server_logging, "", "");

    printf("\n📊 Ejecutando tests de manejo de datos...\n");
    cspec_run_context(test_server_data_handling, "", "");

    printf("\n🔢 Ejecutando tests de códigos de operación...\n");
    cspec_run_context(test_operation_codes, "", "");

    printf("\n🔄 Ejecutando tests de simulación de buffer...\n");
    cspec_run_context(test_buffer_simulation, "", "");

    printf("\n🔧 Ejecutando tests de constantes...\n");
    cspec_run_context(test_constants, "", "");

    // ========== MOSTRAR RESUMEN FINAL ==========

    printf("\n");
    printf("================================================================\n");
    printf("                        RESUMEN FINAL                          \n");
    printf("================================================================\n");

    // CSpec automáticamente maneja el conteo de tests pasados/fallidos
    // y establece el código de salida apropiado

    printf("\n✅ Tests completados. Revisa los resultados arriba.\n");
    printf("💡 Los tests marcados con ✓ pasaron exitosamente\n");
    printf("❌ Los tests marcados con ✗ fallaron\n");
    printf("\n");

    return 0; // CSpec maneja internamente los códigos de error
}

/**
 * @brief Función auxiliar para mostrar información de ayuda
 *
 * Muestra información sobre cómo usar el runner de tests y qué
 * funcionalidades están siendo probadas.
 */
void mostrar_ayuda()
{
    printf("RUNNER DE TESTS - TP0 SISTEMAS OPERATIVOS\n");
    printf("=========================================\n\n");

    printf("Este programa ejecuta tests unitarios para:\n\n");

    printf("CLIENTE:\n");
    printf("  • Creación y manejo de paquetes\n");
    printf("  • Serialización de datos\n");
    printf("  • Carga de archivos de configuración\n");
    printf("  • Sistema de logging\n");
    printf("  • Envío de mensajes\n");
    printf("  • Funciones auxiliares\n\n");

    printf("SERVIDOR:\n");
    printf("  • Logging del servidor\n");
    printf("  • Manejo de datos recibidos\n");
    printf("  • Procesamiento de códigos de operación\n");
    printf("  • Deserialización de buffers\n");
    printf("  • Validación de constantes\n\n");

    printf("USO:\n");
    printf("  ./test_runner    - Ejecutar todos los tests\n");
    printf("  ./test_runner -h - Mostrar esta ayuda\n\n");

    printf("DEPENDENCIAS:\n");
    printf("  • CSpec (framework de testing)\n");
    printf("  • Commons Library (so-commons-library)\n");
    printf("  • Bibliotecas estándar de C\n\n");
}
