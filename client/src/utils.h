#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h>

/**
 * @file utils.h
 * @brief Estructuras y funciones para comunicación cliente-servidor
 *
 * Este header define las estructuras de datos y funciones necesarias
 * para la comunicación TCP entre cliente y servidor usando un protocolo
 * personalizado de paquetes.
 */

// ========== ENUMERACIONES ==========

/**
 * @brief Códigos de operación para el protocolo de comunicación
 *
 * Define los tipos de mensaje que se pueden enviar:
 * - MENSAJE: Un mensaje simple
 * - PAQUETE: Múltiples mensajes agrupados
 */
typedef enum
{
    MENSAJE, // Operación para enviar un mensaje simple
    PAQUETE  // Operación para enviar múltiples mensajes
} op_code;

// ========== ESTRUCTURAS ==========

/**
 * @brief Buffer de datos para almacenar información serializada
 *
 * Contiene un puntero a los datos y su tamaño total.
 * Usado dentro de t_paquete para almacenar los datos a transmitir.
 */
typedef struct
{
    int size;     // Tamaño en bytes de los datos
    void *stream; // Puntero a los datos (buffer de bytes)
} t_buffer;

/**
 * @brief Estructura de paquete para comunicación de red
 *
 * Representa un mensaje completo que se puede enviar por red.
 * Incluye el tipo de operación y un buffer con los datos.
 */
typedef struct
{
    op_code codigo_operacion; // Tipo de operación (MENSAJE o PAQUETE)
    t_buffer *buffer;         // Buffer con los datos a enviar
} t_paquete;

// ========== DECLARACIONES DE FUNCIONES ==========

/**
 * @brief Establece conexión TCP con el servidor
 * @param ip Dirección IP del servidor
 * @param puerto Puerto del servidor
 * @return int File descriptor del socket conectado
 */
int crear_conexion(char *ip, char *puerto);

/**
 * @brief Envía un mensaje simple al servidor
 * @param mensaje String a enviar
 * @param socket_cliente Socket conectado al servidor
 */
void enviar_mensaje(char *mensaje, int socket_cliente);

/**
 * @brief Crea un nuevo paquete vacío
 * @return t_paquete* Paquete inicializado
 */
t_paquete *crear_paquete(void);

/**
 * @brief Agrega datos a un paquete existente
 * @param paquete Paquete donde agregar los datos
 * @param valor Datos a agregar
 * @param tamanio Tamaño de los datos en bytes
 */
void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio);

/**
 * @brief Envía un paquete completo al servidor
 * @param paquete Paquete a enviar
 * @param socket_cliente Socket conectado al servidor
 */
void enviar_paquete(t_paquete *paquete, int socket_cliente);

/**
 * @brief Cierra una conexión de socket
 * @param socket_cliente Socket a cerrar
 */
void liberar_conexion(int socket_cliente);

/**
 * @brief Libera la memoria de un paquete
 * @param paquete Paquete a liberar
 */
void eliminar_paquete(t_paquete *paquete);

#endif /* UTILS_H_ */
