#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <string.h>
#include <assert.h>

/**
 * @file utils.h
 * @brief Utilidades para comunicación de red del servidor
 *
 * Este header define constantes, tipos de datos y funciones
 * para manejar la comunicación TCP en el lado del servidor.
 */

// ========== CONSTANTES ==========

/**
 * @brief Puerto donde escucha el servidor
 */
#define PUERTO "4444"

// ========== ENUMERACIONES ==========

/**
 * @brief Códigos de operación para el protocolo de comunicación
 *
 * Define los tipos de mensaje que puede recibir el servidor:
 * - MENSAJE: Un mensaje simple del cliente
 * - PAQUETE: Múltiples mensajes agrupados del cliente
 */
typedef enum
{
    MENSAJE, // Operación para recibir un mensaje simple
    PAQUETE  // Operación para recibir múltiples mensajes
} op_code;

// ========== VARIABLES GLOBALES ==========

/**
 * @brief Logger global del servidor
 *
 * Variable externa que debe ser inicializada en main()
 * y se usa en todas las funciones de utilidades.
 */
extern t_log *logger;

// ========== DECLARACIONES DE FUNCIONES ==========

/**
 * @brief Recibe un buffer de datos del cliente
 * @param size Puntero donde se guardará el tamaño recibido
 * @param socket_cliente Socket del cliente
 * @return void* Buffer recibido (debe liberarse con free())
 */
void *recibir_buffer(int *, int);

/**
 * @brief Inicializa y configura el servidor TCP
 * @return int File descriptor del socket de escucha
 */
int iniciar_servidor(void);

/**
 * @brief Espera y acepta la conexión de un cliente
 * @param socket_servidor Socket de escucha del servidor
 * @return int File descriptor del socket del cliente conectado
 */
int esperar_cliente(int);

/**
 * @brief Recibe un paquete con múltiples mensajes
 * @param socket_cliente Socket del cliente
 * @return t_list* Lista con todos los mensajes recibidos
 */
t_list *recibir_paquete(int);

/**
 * @brief Recibe y procesa un mensaje simple
 * @param socket_cliente Socket del cliente
 */
void recibir_mensaje(int);

/**
 * @brief Recibe el código de operación del cliente
 * @param socket_cliente Socket del cliente
 * @return int Código de operación o -1 si hay error
 */
int recibir_operacion(int);

#endif /* UTILS_H_ */
