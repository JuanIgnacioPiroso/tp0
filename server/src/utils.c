#include "utils.h"

// Logger global del servidor
t_log *logger;

/**
 * @brief Inicializa y configura el servidor TCP
 *
 * Esta función:
 * 1. Configura las estructuras addrinfo para IPv4 y TCP
 * 2. Crea un socket de escucha
 * 3. Configura el socket para reutilizar el puerto (SO_REUSEPORT)
 * 4. Asocia el socket al puerto definido en PUERTO
 * 5. Pone el socket en modo escucha
 *
 * @return int File descriptor del socket de escucha, -1 si hay error
 */
int iniciar_servidor(void)
{
    int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    // Inicializar estructura hints para configuración
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Usar IP local

    // Resolver información del servidor (puerto PUERTO, cualquier IP local)
    socket_servidor = getaddrinfo(NULL, PUERTO, &hints, &servinfo);

    // Crear el socket de escucha del servidor
    int fd_escucha = socket(servinfo->ai_family,
                            servinfo->ai_socktype,
                            servinfo->ai_protocol);

    // Configurar socket para reutilizar puerto (evita "Address already in use")
    socket_servidor = setsockopt(fd_escucha, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));

    // Asociar el socket al puerto especificado
    socket_servidor = bind(fd_escucha, servinfo->ai_addr, servinfo->ai_addrlen);

    // Poner el socket en modo escucha (acepta hasta SOMAXCONN conexiones pendientes)
    socket_servidor = listen(fd_escucha, SOMAXCONN);

    // Liberar información del servidor
    freeaddrinfo(servinfo);
    log_trace(logger, "Listo para escuchar a mi cliente");

    return fd_escucha;
}

/**
 * @brief Espera y acepta la conexión de un cliente
 *
 * Bloquea la ejecución hasta que un cliente se conecte al servidor.
 * Cuando llega una conexión, la acepta y devuelve el socket del cliente.
 *
 * @param socket_servidor File descriptor del socket de escucha del servidor
 * @return int File descriptor del socket del cliente conectado
 */
int esperar_cliente(int socket_servidor)
{
    // Aceptar conexión entrante (bloquea hasta que llegue un cliente)
    int socket_cliente = accept(socket_servidor, NULL, NULL);
    log_info(logger, "Se conecto un cliente!");

    return socket_cliente;
}

/**
 * @brief Recibe el código de operación del cliente
 *
 * Lee los primeros 4 bytes del mensaje para determinar qué tipo de
 * operación quiere realizar el cliente (MENSAJE o PAQUETE).
 *
 * @param socket_cliente File descriptor del socket del cliente
 * @return int Código de operación (MENSAJE, PAQUETE) o -1 si hay error/desconexión
 */
int recibir_operacion(int socket_cliente)
{
    int cod_op;

    // Recibir exactamente sizeof(int) bytes con MSG_WAITALL
    if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
        return cod_op; // Operación recibida correctamente
    else
    {
        // Error en recepción o cliente desconectado
        close(socket_cliente);
        return -1;
    }
}

/**
 * @brief Recibe un buffer de datos del cliente
 *
 * Esta función:
 * 1. Recibe primero el tamaño del buffer (4 bytes)
 * 2. Aloca memoria para ese tamaño
 * 3. Recibe los datos del buffer
 * 4. Devuelve el buffer y actualiza el tamaño por referencia
 *
 * @param size Puntero donde se guardará el tamaño del buffer recibido
 * @param socket_cliente File descriptor del socket del cliente
 * @return void* Puntero al buffer recibido (debe ser liberado con free())
 */
void *recibir_buffer(int *size, int socket_cliente)
{
    void *buffer;

    // Recibir primero el tamaño del buffer
    recv(socket_cliente, size, sizeof(int), MSG_WAITALL);

    // Alocar memoria para el buffer
    buffer = malloc(*size);

    // Recibir los datos del buffer
    recv(socket_cliente, buffer, *size, MSG_WAITALL);

    return buffer;
}

/**
 * @brief Recibe y procesa un mensaje simple del cliente
 *
 * Recibe un mensaje simple (código de operación MENSAJE),
 * lo registra en el log y libera la memoria.
 *
 * @param socket_cliente File descriptor del socket del cliente
 */
void recibir_mensaje(int socket_cliente)
{
    int size;

    // Recibir el buffer con el mensaje
    char *buffer = recibir_buffer(&size, socket_cliente);

    // Registrar el mensaje recibido
    log_info(logger, "Me llego el mensaje: %s", buffer);

    // Liberar memoria del buffer
    free(buffer);
}

/**
 * @brief Recibe y procesa un paquete con múltiples mensajes
 *
 * Esta función:
 * 1. Recibe el buffer completo del paquete
 * 2. Deserializa los mensajes individuales del buffer
 * 3. Crea una lista con todos los mensajes
 * 4. Libera el buffer temporal
 *
 * El formato del buffer es: [tamaño_msg1][msg1][tamaño_msg2][msg2]...
 *
 * @param socket_cliente File descriptor del socket del cliente
 * @return t_list* Lista con todos los mensajes recibidos (debe ser liberada)
 */
t_list *recibir_paquete(int socket_cliente)
{
    int size;                        // Tamaño total del buffer
    int desplazamiento = 0;          // Offset actual en el buffer
    void *buffer;                    // Buffer con todos los datos
    t_list *valores = list_create(); // Lista para almacenar mensajes
    int tamanio;                     // Tamaño del mensaje actual

    // Recibir buffer completo con todos los mensajes
    buffer = recibir_buffer(&size, socket_cliente);

    // Deserializar mensajes uno por uno
    while (desplazamiento < size)
    {
        // Leer tamaño del siguiente mensaje
        memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
        desplazamiento += sizeof(int);

        // Alocar memoria y leer el mensaje
        char *valor = malloc(tamanio);
        memcpy(valor, buffer + desplazamiento, tamanio);
        desplazamiento += tamanio;

        // Agregar mensaje a la lista
        list_add(valores, valor);
    }

    // Liberar buffer temporal
    free(buffer);
    return valores;
}
