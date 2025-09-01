#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utils.h"

/**
 * @file server.h
 * @brief Declaraciones de funciones específicas del servidor TCP
 *
 * Este header contiene las declaraciones de funciones específicas
 * del servidor que procesa mensajes de clientes.
 */

// ========== DECLARACIONES DE FUNCIONES ==========

/**
 * @brief Función auxiliar para iterar sobre mensajes recibidos
 * @param value String con el mensaje a procesar
 */
void iterator(char *value);

#endif /* SERVER_H_ */
