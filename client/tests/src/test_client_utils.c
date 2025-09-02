#include <cspecs/cspec.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Incluir los headers del cliente
#include "../../src/utils.h"
#include "../../src/client.h"

/**
 * @file test_client_utils.c
 * @brief Tests unitarios para las funciones de utilidades del cliente
 *
 * Este archivo contiene tests para verificar el correcto funcionamiento
 * de las funciones de red, serialización y manejo de paquetes del cliente.
 */

// ========== TESTS PARA FUNCIONES DE PAQUETES ==========

context(test_paquetes){

    describe("Creación de paquetes"){

        it("debería crear un paquete vacío correctamente"){
            t_paquete *paquete = crear_paquete();

should_not_be_null(paquete);
should_int(paquete->codigo_operacion) be equal to(PAQUETE);
should_not_be_null(paquete->buffer);
should_int(paquete->buffer->size) be equal to(0);
should_ptr(paquete->buffer->stream) be equal to(NULL);

eliminar_paquete(paquete);
}
end

    it("debería agregar datos a un paquete correctamente")
{
    t_paquete *paquete = crear_paquete();
    char *mensaje1 = "Hola";
    char *mensaje2 = "Mundo";

    agregar_a_paquete(paquete, mensaje1, strlen(mensaje1) + 1);
    should_int(paquete->buffer->size) be equal to(strlen(mensaje1) + 1 + sizeof(int));

    agregar_a_paquete(paquete, mensaje2, strlen(mensaje2) + 1);
    should_int(paquete->buffer->size) be equal to(
        strlen(mensaje1) + 1 + sizeof(int) +
        strlen(mensaje2) + 1 + sizeof(int));

    eliminar_paquete(paquete);
}
end

    it("debería serializar un paquete correctamente")
{
    t_paquete *paquete = crear_paquete();
    char *mensaje = "Test";

    agregar_a_paquete(paquete, mensaje, strlen(mensaje) + 1);

    int bytes_totales = paquete->buffer->size + 2 * sizeof(int);
    void *serializado = serializar_paquete(paquete, bytes_totales);

    should_not_be_null(serializado);

    // Verificar que el código de operación esté al inicio
    op_code codigo;
    memcpy(&codigo, serializado, sizeof(int));
    should_int(codigo) be equal to(PAQUETE);

    // Verificar que el tamaño del buffer esté después del código
    int tamanio_buffer;
    memcpy(&tamanio_buffer, serializado + sizeof(int), sizeof(int));
    should_int(tamanio_buffer) be equal to(paquete->buffer->size);

    free(serializado);
    eliminar_paquete(paquete);
}
end
}
end
}
end

    // ========== TESTS PARA FUNCIONES DE CONFIGURACIÓN ==========

    context(test_configuracion){

        describe("Manejo de archivos de configuración"){

            before{
                // Crear un archivo de configuración temporal para tests
                FILE *temp_config = fopen("test_cliente.config", "w");
if (temp_config)
{
    fprintf(temp_config, "IP=127.0.0.1\n");
    fprintf(temp_config, "PUERTO=4444\n");
    fprintf(temp_config, "CLAVE=test_value\n");
    fclose(temp_config);
}
}
end

    after
{
    // Limpiar archivo temporal
    unlink("test_cliente.config");
}
end

    it("debería cargar configuración desde archivo correctamente")
{
    // Crear configuración usando el archivo temporal
    t_config *config = config_create("test_cliente.config");

    should_not_be_null(config);

    char *ip = config_get_string_value(config, "IP");
    char *puerto = config_get_string_value(config, "PUERTO");
    char *clave = config_get_string_value(config, "CLAVE");

    should_string(ip) be equal to("127.0.0.1");
    should_string(puerto) be equal to("4444");
    should_string(clave) be equal to("test_value");

    config_destroy(config);
}
end

    it("debería retornar NULL para archivo inexistente")
{
    t_config *config = config_create("archivo_inexistente.config");
    should_be_null(config);
}
end
}
end
}
end

    // ========== TESTS PARA FUNCIONES DE LOGGING ==========

    context(test_logging){

        describe("Sistema de logging"){

            after{
                // Limpiar archivo de log temporal
                unlink("test_tp0.log");
}
end

    it("debería crear un logger correctamente")
{
    t_log *logger = log_create("test_tp0.log", "Test_Cliente", 1, LOG_LEVEL_INFO);

    should_not_be_null(logger);

    // Verificar que se puede escribir en el log
    log_info(logger, "Mensaje de prueba");

    // Verificar que el archivo se creó
    struct stat buffer;
    should_int(stat("test_tp0.log", &buffer)) be equal to(0);

    log_destroy(logger);
}
end

    it("debería registrar mensajes con diferentes niveles")
{
    t_log *logger = log_create("test_tp0.log", "Test_Cliente", 0, LOG_LEVEL_TRACE);

    should_not_be_null(logger);

    log_trace(logger, "Mensaje TRACE");
    log_debug(logger, "Mensaje DEBUG");
    log_info(logger, "Mensaje INFO");
    log_warning(logger, "Mensaje WARNING");
    log_error(logger, "Mensaje ERROR");

    log_destroy(logger);

    // Verificar que el archivo contiene datos
    struct stat buffer;
    stat("test_tp0.log", &buffer);
    should_int(buffer.st_size) be greater than(0);
}
end
}
end
}
end

    // ========== TESTS PARA FUNCIONES DE MENSAJES ==========

    context(test_mensajes){

        describe("Envío de mensajes simples"){

            it("debería crear mensaje simple correctamente"){
                // Este test simula la creación de un mensaje sin envío real
                char *mensaje = "Mensaje de prueba";

// Crear estructura de paquete como lo hace enviar_mensaje
t_paquete *paquete = malloc(sizeof(t_paquete));
paquete->codigo_operacion = MENSAJE;
paquete->buffer = malloc(sizeof(t_buffer));
paquete->buffer->size = strlen(mensaje) + 1;
paquete->buffer->stream = malloc(paquete->buffer->size);

memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

// Verificar estructura
should_int(paquete->codigo_operacion) be equal to(MENSAJE);
should_int(paquete->buffer->size) be equal to(strlen(mensaje) + 1);
should_string((char *)paquete->buffer->stream) be equal to(mensaje);

eliminar_paquete(paquete);
}
end
}
end
}
end

    // ========== TESTS PARA FUNCIONES AUXILIARES ==========

    context(test_auxiliares){

        describe("Funciones de limpieza y utilidades"){

            it("debería limpiar paquetes sin errores de memoria"){
                t_paquete *paquete = crear_paquete();
agregar_a_paquete(paquete, "test", 5);

// Esta función debe ejecutarse sin errores
eliminar_paquete(paquete);

// Si llegamos aquí, la limpieza fue exitosa
should_bool(true) be equal to(true);
}
end

    it("debería manejar paquetes con múltiples elementos")
{
    t_paquete *paquete = crear_paquete();

    for (int i = 0; i < 5; i++)
    {
        char mensaje[20];
        sprintf(mensaje, "Mensaje %d", i);
        agregar_a_paquete(paquete, mensaje, strlen(mensaje) + 1);
    }

    // Verificar que el tamaño creció apropiadamente
    should_int(paquete->buffer->size) be greater than(0);

    // Serializar para verificar integridad
    int bytes = paquete->buffer->size + 2 * sizeof(int);
    void *serializado = serializar_paquete(paquete, bytes);
    should_not_be_null(serializado);

    free(serializado);
    eliminar_paquete(paquete);
}
end
}
end
}
end
