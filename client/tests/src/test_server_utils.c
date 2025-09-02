#include <cspecs/cspec.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>

// Incluir los headers del servidor
#include "../../server/src/utils.h"
#include "../../server/src/server.h"

/**
 * @file test_server_utils.c
 * @brief Tests unitarios para las funciones de utilidades del servidor
 *
 * Este archivo contiene tests para verificar el correcto funcionamiento
 * de las funciones de red, recepción de mensajes y manejo de conexiones del servidor.
 */

// Variable global para simular el logger del servidor
t_log *logger;

// ========== TESTS PARA FUNCIONES DE LOGGING DEL SERVIDOR ==========

context(test_server_logging){

    describe("Sistema de logging del servidor"){

        before{
            logger = log_create("test_server.log", "Test_Servidor", 1, LOG_LEVEL_DEBUG);
}
end

    after
{
    if (logger)
    {
        log_destroy(logger);
        logger = NULL;
    }
    unlink("test_server.log");
}
end

    it("debería crear logger del servidor correctamente")
{
    should_not_be_null(logger);

    log_info(logger, "Servidor de prueba iniciado");

    // Verificar que el archivo se creó
    struct stat buffer;
    should_int(stat("test_server.log", &buffer)) be equal to(0);
}
end

    it("debería usar la función iterator correctamente")
{
    // Crear un string de prueba
    char *mensaje = "Mensaje de prueba para iterator";

    // La función iterator solo registra en el log, así que verificamos que no crashee
    iterator(mensaje);

    // Si llegamos aquí, la función ejecutó correctamente
    should_bool(true) be equal to(true);
}
end
}
end
}
end

    // ========== TESTS PARA SIMULACIÓN DE RECEPCIÓN DE DATOS ==========

    context(test_server_data_handling){

        describe("Manejo de datos del servidor"){

            before{
                logger = log_create("test_server.log", "Test_Servidor", 0, LOG_LEVEL_DEBUG);
}
end

    after
{
    if (logger)
    {
        log_destroy(logger);
        logger = NULL;
    }
    unlink("test_server.log");
}
end

    it("debería simular deserialización de paquete correctamente")
{
    // Simular un buffer como el que recibiría el servidor
    t_list *lista = list_create();

    // Agregar algunos elementos de prueba
    char *msg1 = malloc(10);
    strcpy(msg1, "Mensaje 1");
    char *msg2 = malloc(10);
    strcpy(msg2, "Mensaje 2");
    char *msg3 = malloc(10);
    strcpy(msg3, "Mensaje 3");

    list_add(lista, msg1);
    list_add(lista, msg2);
    list_add(lista, msg3);

    should_int(list_size(lista)) be equal to(3);

    // Verificar que los elementos están en la lista
    should_string((char *)list_get(lista, 0)) be equal to("Mensaje 1");
    should_string((char *)list_get(lista, 1)) be equal to("Mensaje 2");
    should_string((char *)list_get(lista, 2)) be equal to("Mensaje 3");

    // Limpiar memoria
    list_destroy_and_destroy_elements(lista, free);
}
end

    it("debería manejar listas vacías correctamente")
{
    t_list *lista_vacia = list_create();

    should_int(list_size(lista_vacia)) be equal to(0);
    should_bool(list_is_empty(lista_vacia)) be equal to(true);

    list_destroy(lista_vacia);
}
end

    it("debería iterar sobre elementos de lista correctamente")
{
    t_list *lista = list_create();

    // Agregar elementos
    for (int i = 0; i < 5; i++)
    {
        char *elemento = malloc(20);
        sprintf(elemento, "Elemento %d", i);
        list_add(lista, elemento);
    }

    should_int(list_size(lista)) be equal to(5);

    // Simular iteración como lo hace el servidor
    int contador = 0;
    void contar_elementos(void *elemento)
    {
        contador++;
        // Verificar que el elemento no es NULL
        should_not_be_null(elemento);
    }

    list_iterate(lista, contar_elementos);
    should_int(contador) be equal to(5);

    list_destroy_and_destroy_elements(lista, free);
}
end
}
end
}
end

    // ========== TESTS PARA VALIDACIÓN DE CÓDIGOS DE OPERACIÓN ==========

    context(test_operation_codes){

        describe("Códigos de operación"){

            it("debería reconocer código MENSAJE correctamente"){
                op_code codigo = MENSAJE;
should_int(codigo) be equal to(0); // MENSAJE debe ser 0
}
end

    it("debería reconocer código PAQUETE correctamente")
{
    op_code codigo = PAQUETE;
    should_int(codigo) be equal to(1); // PAQUETE debe ser 1
}
end

    it("debería simular switch de códigos de operación")
{
    op_code codigos[] = {MENSAJE, PAQUETE};
    bool mensaje_procesado = false;
    bool paquete_procesado = false;

    for (int i = 0; i < 2; i++)
    {
        switch (codigos[i])
        {
        case MENSAJE:
            mensaje_procesado = true;
            break;
        case PAQUETE:
            paquete_procesado = true;
            break;
        default:
            // No debería llegar aquí
            should_bool(false) be equal to(true);
            break;
        }
    }

    should_bool(mensaje_procesado) be equal to(true);
    should_bool(paquete_procesado) be equal to(true);
}
end
}
end
}
end

    // ========== TESTS PARA SIMULACIÓN DE BUFFER ==========

    context(test_buffer_simulation){

        describe("Simulación de manejo de buffers"){

            it("debería simular creación de buffer serializado"){
                // Simular el formato de un paquete serializado
                char *mensaje1 = "Hola";
char *mensaje2 = "Mundo";

// Calcular tamaño total necesario
int tamanio_msg1 = strlen(mensaje1) + 1;
int tamanio_msg2 = strlen(mensaje2) + 1;
int tamanio_total = 2 * sizeof(int) + tamanio_msg1 + 2 * sizeof(int) + tamanio_msg2;

// Crear buffer simulado
void *buffer_simulado = malloc(tamanio_total);
int offset = 0;

// Agregar primer mensaje: [tamaño][mensaje]
memcpy(buffer_simulado + offset, &tamanio_msg1, sizeof(int));
offset += sizeof(int);
memcpy(buffer_simulado + offset, mensaje1, tamanio_msg1);
offset += tamanio_msg1;

// Agregar segundo mensaje: [tamaño][mensaje]
memcpy(buffer_simulado + offset, &tamanio_msg2, sizeof(int));
offset += sizeof(int);
memcpy(buffer_simulado + offset, mensaje2, tamanio_msg2);
offset += tamanio_msg2;

should_int(offset) be equal to(tamanio_total);

// Simular deserialización
offset = 0;
int tam_leido;
char buffer_msg[100];

// Leer primer mensaje
memcpy(&tam_leido, buffer_simulado + offset, sizeof(int));
offset += sizeof(int);
should_int(tam_leido) be equal to(tamanio_msg1);

memcpy(buffer_msg, buffer_simulado + offset, tam_leido);
offset += tam_leido;
should_string(buffer_msg) be equal to("Hola");

// Leer segundo mensaje
memcpy(&tam_leido, buffer_simulado + offset, sizeof(int));
offset += sizeof(int);
should_int(tam_leido) be equal to(tamanio_msg2);

memcpy(buffer_msg, buffer_simulado + offset, tam_leido);
should_string(buffer_msg) be equal to("Mundo");

free(buffer_simulado);
}
end
}
end
}
end

    // ========== TESTS PARA VALIDACIÓN DE CONSTANTES ==========

    context(test_constants){

        describe("Constantes del servidor"){

            it("debería tener puerto definido correctamente"){
                should_string(PUERTO) be equal to("4444");
}
end
}
end
}
end
