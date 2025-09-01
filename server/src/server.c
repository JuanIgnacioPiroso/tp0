#include "server.h"

/**
 * @brief Función principal del servidor
 *
 * Esta función implementa un servidor que:
 * 1. Inicializa el sistema de logging
 * 2. Inicia el servidor en el puerto configurado
 * 3. Espera conexiones de clientes
 * 4. Procesa mensajes y paquetes en un bucle infinito
 * 5. Maneja desconexiones de clientes
 *
 * El servidor puede recibir dos tipos de operaciones:
 * - MENSAJE: Un mensaje simple
 * - PAQUETE: Múltiples mensajes agrupados
 *
 * @return int Código de salida (EXIT_SUCCESS o EXIT_FAILURE)
 */
int main(void)
{
    // Inicializar logger del servidor con nivel DEBUG
    logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

    // Iniciar servidor y obtener socket de escucha
    int server_fd = iniciar_servidor();
    log_info(logger, "Servidor listo para recibir al cliente");

    // Esperar y aceptar conexión de un cliente
    int cliente_fd = esperar_cliente(server_fd);
    t_list *lista;

    // Bucle principal: procesar mensajes del cliente
    while (1)
    {
        // Recibir código de operación del cliente
        int cod_op = recibir_operacion(cliente_fd);

        // Procesar según el tipo de operación
        switch (cod_op)
        {
        case MENSAJE:
            // Procesar mensaje simple
            recibir_mensaje(cliente_fd);
            break;
        case PAQUETE:
            // Procesar paquete con múltiples mensajes
            lista = recibir_paquete(cliente_fd);
            log_info(logger, "Me llegaron los siguientes valores:\n");
            // Iterar y mostrar todos los mensajes recibidos
            list_iterate(lista, (void *)iterator);
            break;
        case -1:
            // Cliente se desconectó
            log_error(logger, "el cliente se desconecto. Terminando servidor");
            return EXIT_FAILURE;
        default:
            // Operación desconocida
            log_warning(logger, "Operacion desconocida. No quieras meter la pata");
            break;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Función auxiliar para iterar sobre los mensajes recibidos
 *
 * Esta función se usa como callback para list_iterate() y registra
 * cada mensaje del paquete en el log del servidor.
 *
 * @param value String con el mensaje a registrar
 */
void iterator(char *value)
{
    // Registrar cada mensaje en el log
    log_info(logger, "%s", value);
}
