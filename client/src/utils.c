#include "utils.h"

/**
 * @brief Serializa un paquete en un buffer de bytes para envío por red
 *
 * Convierte la estructura t_paquete en un buffer lineal de bytes que puede
 * ser enviado a través de un socket. El formato del buffer serializado es:
 * [código_operación (4 bytes)] [tamaño_buffer (4 bytes)] [datos del buffer (N bytes)]
 *
 * @param paquete Puntero al paquete a serializar
 * @param bytes Tamaño total en bytes que ocupará el buffer serializado
 * @return void* Puntero al buffer serializado (debe ser liberado con free())
 */
void *serializar_paquete(t_paquete *paquete, int bytes)
{
    void *magic = malloc(bytes); // Buffer para datos serializados
    int desplazamiento = 0;      // Offset actual en el buffer

    // Copiar código de operación (MENSAJE o PAQUETE)
    memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
    desplazamiento += sizeof(int);

    // Copiar tamaño del buffer de datos
    memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
    desplazamiento += sizeof(int);

    // Copiar los datos del buffer
    memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
    desplazamiento += paquete->buffer->size;

    return magic;
}

/**
 * @brief Establece una conexión TCP con el servidor
 *
 * Esta función:
 * 1. Configura las estructuras addrinfo para IPv4 y TCP
 * 2. Resuelve la dirección del servidor usando getaddrinfo()
 * 3. Crea un socket TCP
 * 4. Establece la conexión con el servidor
 * 5. Libera las estructuras auxiliares
 *
 * @param ip Dirección IP del servidor (ej: "127.0.0.1")
 * @param puerto Puerto del servidor (ej: "4444")
 * @return int File descriptor del socket conectado, -1 si hay error
 */
int crear_conexion(char *ip, char *puerto)
{
    struct addrinfo hints;        // Configuración para getaddrinfo
    struct addrinfo *server_info; // Información del servidor

    // Inicializar estructura hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Crear el socket del cliente
    int socket_cliente = 0;

    // Resolver dirección del servidor
    socket_cliente = getaddrinfo(ip, puerto, &hints, &server_info);

    // Crear socket TCP
    int fd_conexion = socket(server_info->ai_family,
                             server_info->ai_socktype,
                             server_info->ai_protocol);

    // Conectar al servidor
    socket_cliente = connect(fd_conexion, server_info->ai_addr, server_info->ai_addrlen);

    // Liberar información del servidor
    freeaddrinfo(server_info);

    return fd_conexion;
}

/**
 * @brief Envía un mensaje simple al servidor
 *
 * Crea un paquete con código de operación MENSAJE, empaqueta el mensaje
 * y lo envía al servidor. Después libera toda la memoria utilizada.
 *
 * @param mensaje String a enviar (debe estar terminado en \0)
 * @param socket_cliente File descriptor del socket conectado al servidor
 */
void enviar_mensaje(char *mensaje, int socket_cliente)
{
    // Crear estructura de paquete
    t_paquete *paquete = malloc(sizeof(t_paquete));

    // Configurar como mensaje simple
    paquete->codigo_operacion = MENSAJE;
    paquete->buffer = malloc(sizeof(t_buffer));
    paquete->buffer->size = strlen(mensaje) + 1; // +1 para el \0
    paquete->buffer->stream = malloc(paquete->buffer->size);

    // Copiar el mensaje al buffer
    memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

    // Calcular tamaño total: datos + código_op + tamaño
    int bytes = paquete->buffer->size + 2 * sizeof(int);

    // Serializar y enviar
    void *a_enviar = serializar_paquete(paquete, bytes);
    send(socket_cliente, a_enviar, bytes, 0);

    // Liberar memoria
    free(a_enviar);
    eliminar_paquete(paquete);
}

/**
 * @brief Inicializa el buffer de un paquete
 *
 * Crea un buffer vacío dentro del paquete, listo para recibir datos.
 * El buffer se inicializa con tamaño 0 y stream NULL.
 *
 * @param paquete Puntero al paquete al que se le creará el buffer
 */
void crear_buffer(t_paquete *paquete)
{
    paquete->buffer = malloc(sizeof(t_buffer));
    paquete->buffer->size = 0;      // Tamaño inicial: 0 bytes
    paquete->buffer->stream = NULL; // Sin datos inicialmente
}

/**
 * @brief Crea un nuevo paquete vacío
 *
 * Aloca memoria para un paquete, lo configura con código de operación PAQUETE
 * e inicializa su buffer vacío. Este paquete puede recibir múltiples mensajes.
 *
 * @return t_paquete* Puntero al paquete creado (debe ser liberado con eliminar_paquete())
 */
t_paquete *crear_paquete(void)
{
    // Alocar memoria para el paquete
    t_paquete *paquete = malloc(sizeof(t_paquete));

    // Configurar como paquete múltiple
    paquete->codigo_operacion = PAQUETE;

    // Inicializar buffer vacío
    crear_buffer(paquete);

    return paquete;
}

/**
 * @brief Agrega datos a un paquete existente
 *
 * Expande el buffer del paquete para incluir nuevos datos. El formato interno es:
 * [tamaño_dato (4 bytes)] [dato (N bytes)] [tamaño_siguiente_dato] [siguiente_dato] ...
 *
 * @param paquete Puntero al paquete donde agregar los datos
 * @param valor Puntero a los datos a agregar
 * @param tamanio Cantidad de bytes a agregar
 */
void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio)
{
    // Expandir el buffer para incluir: tamaño (4 bytes) + datos (tamanio bytes)
    paquete->buffer->stream = realloc(paquete->buffer->stream,
                                      paquete->buffer->size + tamanio + sizeof(int));

    // Escribir primero el tamaño del dato
    memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));

    // Escribir los datos después del tamaño
    memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

    // Actualizar el tamaño total del buffer
    paquete->buffer->size += tamanio + sizeof(int);
}

/**
 * @brief Envía un paquete completo al servidor
 *
 * Serializa el paquete y lo envía a través del socket. No libera el paquete,
 * eso debe hacerse por separado con eliminar_paquete().
 *
 * @param paquete Puntero al paquete a enviar
 * @param socket_cliente File descriptor del socket conectado al servidor
 */
void enviar_paquete(t_paquete *paquete, int socket_cliente)
{
    // Calcular tamaño total del paquete serializado
    int bytes = paquete->buffer->size + 2 * sizeof(int);

    // Serializar el paquete
    void *a_enviar = serializar_paquete(paquete, bytes);

    // Enviar por socket
    send(socket_cliente, a_enviar, bytes, 0);

    // Liberar buffer temporal de serialización
    free(a_enviar);
}

/**
 * @brief Libera toda la memoria de un paquete
 *
 * Libera en orden:
 * 1. El stream de datos del buffer
 * 2. La estructura del buffer
 * 3. La estructura del paquete
 *
 * @param paquete Puntero al paquete a liberar
 */
void eliminar_paquete(t_paquete *paquete)
{
    free(paquete->buffer->stream); // Liberar datos
    free(paquete->buffer);         // Liberar buffer
    free(paquete);                 // Liberar paquete
}

/**
 * @brief Cierra una conexión de socket
 *
 * Cierra el file descriptor del socket y libera los recursos asociados.
 *
 * @param socket_cliente File descriptor del socket a cerrar
 */
void liberar_conexion(int socket_cliente)
{
    close(socket_cliente);
}
