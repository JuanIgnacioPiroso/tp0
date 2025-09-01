#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <readline/readline.h>

#include "utils.h"

/**
 * @file client.h
 * @brief Declaraciones de funciones del cliente TCP
 *
 * Este header contiene las declaraciones de todas las funciones
 * específicas del cliente que se comunica con el servidor.
 */

// ========== DECLARACIONES DE FUNCIONES ==========

/**
 * @brief Inicializa el sistema de logging del cliente
 * @return t_log* Logger configurado para el cliente
 */
t_log *iniciar_logger(void);

/**
 * @brief Carga la configuración desde cliente.config
 * @return t_config* Configuración cargada o NULL si hay error
 */
t_config *iniciar_config(void);

/**
 * @brief Lee comandos interactivamente desde consola
 * @param logger Logger donde registrar los comandos leídos
 */
void leer_consola(t_log *);

/**
 * @brief Permite enviar múltiples mensajes en un paquete
 * @param conexion File descriptor del socket conectado al servidor
 * @param logger Logger para registrar eventos
 */
void paquete(int, t_log *);

/**
 * @brief Limpia recursos y termina el programa ordenadamente
 * @param conexion File descriptor de la conexión a cerrar
 * @param logger Logger a destruir
 * @param config Configuración a destruir
 */
void terminar_programa(int, t_log *, t_config *);

#endif /* CLIENT_H_ */
