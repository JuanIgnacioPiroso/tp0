#include "client.h"

/**
 * @brief Función principal del cliente
 *
 * Esta función implementa un cliente que:
 * 1. Inicializa el sistema de logging
 * 2. Lee configuración desde archivo cliente.config
 * 3. Establece conexión con el servidor
 * 4. Envía un mensaje simple con la clave de configuración
 * 5. Permite al usuario enviar múltiples mensajes en un paquete
 * 6. Limpia recursos y termina
 *
 * @return int Código de salida del programa (0 = éxito)
 */
int main(void)
{
    /*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

    // Variables para la conexión y configuración
    int conexion; // File descriptor de la conexión con el servidor
    char *ip;     // Dirección IP del servidor (leída desde config)
    char *puerto; // Puerto del servidor (leído desde config)
    char *valor;  // Valor de la clave (leído desde config)

    t_log *logger;    // Logger para registrar eventos
    t_config *config; // Configuración leída desde archivo

    /* ---------------- LOGGING ---------------- */

    // Inicializar el sistema de logging
    logger = iniciar_logger();

    char *mensaje = "Hola! Soy un log";

    // Registrar un mensaje de prueba en el log
    log_info(logger, mensaje);

    /* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

    // Cargar configuración desde cliente.config
    config = iniciar_config();

    // Verificar que el archivo de configuración se cargó correctamente
    if (config == NULL)
    {
        log_error(logger, "Error: No se encontro el archivo de configuracion");
        config_destroy(config);
        log_destroy(logger);
        abort();
    }

    // Leer valores de configuración
    valor = config_get_string_value(config, "CLAVE");   // Valor a enviar al servidor
    puerto = config_get_string_value(config, "PUERTO"); // Puerto del servidor
    ip = config_get_string_value(config, "IP");         // IP del servidor

    // Mostrar valores leídos por consola (para verificación)
    printf("El valor es: %s\n", valor);
    printf("El puerto es: %s\n", puerto);
    printf("LA ip  es: %s\n", ip);

    // Registrar los valores de configuración en el log
    log_info(logger, config);
    log_info(logger, valor);
    log_info(logger, puerto);
    log_info(logger, ip);

    /* ---------------- LEER DE CONSOLA ---------------- */

    /*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

    // ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

    // Establecer conexión TCP con el servidor
    conexion = crear_conexion(ip, puerto);

    // Enviar mensaje simple con el valor de la clave de configuración
    enviar_mensaje(valor, conexion);

    // Permitir al usuario enviar múltiples mensajes en un paquete
    paquete(conexion, logger);

    // Limpiar recursos y terminar programa
    terminar_programa(conexion, logger, config);

    /*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
    // Proximamente
}

/**
 * @brief Inicializa el sistema de logging
 *
 * Crea un logger que escribirá eventos en el archivo "tp0.log" con nivel INFO.
 * El logger está configurado para mostrar mensajes por consola y guardarlos en archivo.
 *
 * @return t_log* Puntero al logger creado, NULL si hay error
 */
t_log *iniciar_logger(void)
{
    // Crear logger: archivo="tp0.log", programa="Cliente", mostrar_por_consola=1, nivel=INFO
    t_log *nuevo_logger = log_create("tp0.log", "Cliente", 1, LOG_LEVEL_INFO);

    return nuevo_logger;
}

/**
 * @brief Inicializa la configuración desde archivo
 *
 * Lee el archivo "cliente.config" que debe contener las claves:
 * - IP: Dirección del servidor
 * - PUERTO: Puerto del servidor
 * - CLAVE: Valor a enviar al servidor
 *
 * @return t_config* Puntero a la configuración cargada, NULL si hay error
 */
t_config *iniciar_config(void)
{
    // Cargar configuración desde el archivo cliente.config
    t_config *nuevo_config = config_create("cliente.config");

    return nuevo_config;
}

/**
 * @brief Lee comandos desde la consola interactivamente
 *
 * Función que entra en un bucle infinito leyendo líneas desde stdin.
 * Cada línea leída se registra en el log. Si se ingresa una línea vacía,
 * el programa termina abruptamente.
 *
 * NOTA: Esta función no se usa en el flujo principal actual del programa.
 *
 * @param logger Logger donde registrar las líneas leídas
 */
void leer_consola(t_log *logger)
{
    char *leido;

    // Bucle infinito para leer comandos
    while (1)
    {
        // Leer línea con prompt ">"
        leido = readline("> ");

        // Si la línea está vacía, terminar programa
        if (!strcmp(leido, ""))
        {
            free(leido);
            abort();
        }

        // Registrar la línea leída en el log
        log_info(logger, leido);
        free(leido);
    }
}

/**
 * @brief Permite al usuario enviar múltiples mensajes en un paquete
 *
 * Esta función:
 * 1. Crea un paquete vacío
 * 2. Lee líneas desde la consola interactivamente
 * 3. Agrega cada línea no vacía al paquete
 * 4. Cuando se ingresa una línea vacía, envía todo el paquete al servidor
 * 5. Libera la memoria del paquete
 *
 * @param conexion File descriptor de la conexión con el servidor
 * @param logger Logger para registrar eventos
 */
void paquete(int conexion, t_log *logger)
{
    char *leido;
    t_paquete *paquete;

    // Crear un nuevo paquete vacío
    paquete = crear_paquete();

    // Leer mensajes del usuario hasta que ingrese línea vacía
    while (1)
    {
        leido = readline("> ");

        // Si no ingresa nada (línea vacía), terminar y enviar el paquete
        if (!strcmp(leido, ""))
        {
            free(leido);
            break; // Salir del bucle para enviar el paquete
        }

        // Registrar el mensaje ingresado en el log
        log_info(logger, leido);

        // Agregar el mensaje al paquete (incluyendo el carácter nulo)
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);

        free(leido);
    }

    // Enviar el paquete completo con todos los mensajes al servidor
    enviar_paquete(paquete, conexion);
    log_info(logger, "Paquete enviado exitosamente");

    // Liberar memoria del paquete
    eliminar_paquete(paquete);
}

/**
 * @brief Limpia todos los recursos y termina el programa ordenadamente
 *
 * Esta función se encarga de liberar toda la memoria y cerrar conexiones:
 * - Destruye el logger
 * - Destruye la configuración
 * - Cierra la conexión de red
 *
 * @param conexion File descriptor de la conexión a cerrar
 * @param logger Logger a destruir
 * @param config Configuración a destruir
 */
void terminar_programa(int conexion, t_log *logger, t_config *config)
{
    // Liberar recursos en orden inverso a su creación
    log_destroy(logger);        // Cerrar y liberar logger
    config_destroy(config);     // Liberar configuración
    liberar_conexion(conexion); // Cerrar socket de conexión
}
