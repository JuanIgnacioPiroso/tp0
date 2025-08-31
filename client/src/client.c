#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	char* mensaje = "Hola! Soy un log";

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, mensaje);

	


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	if (config == NULL)
	{
		log_error(logger,"Error: No se encontro el archivo de configuracion");
		config_destroy(config);
		log_destroy(logger);
		abort();
	}
	valor = config_get_string_value(config, "CLAVE");
	puerto = config_get_string_value(config, "PUERTO");
	ip = config_get_string_value(config, "IP");

	//PRUEBAS PROPIAS PARA VERIFICAR VALORES
	printf("El valor es: %s\n", valor);
	printf("El puerto es: %s\n", puerto);
	printf("LA ip  es: %s\n", ip);


	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config
	log_info(logger,config);
	log_info(logger,valor);
	log_info(logger,puerto);
	log_info(logger,ip);

	/* ---------------- LEER DE CONSOLA ---------------- */

	

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	leer_consola(logger);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log","Cliente",1,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	while (1)
	{
		leido = readline("> ");

		 if (!strcmp(leido,"")) {
			free(leido);
            abort();
        }
        log_info(logger,leido);
        free(leido);
	}

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	paquete = crear_paquete();

	agregar_a_paquete(paquete,leido,sizeof(leido));

	enviar_paquete(paquete,conexion);


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

}
