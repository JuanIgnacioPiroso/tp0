# Tests Unitarios - TP0 Sistemas Operativos

Este directorio contiene los tests unitarios para el proyecto cliente-servidor TCP del TP0 de Sistemas Operativos, implementados usando el framework **CSpec**.

## 📁 Estructura del Proyecto

```
tests/
├── src/
│   ├── test_client_utils.c    # Tests para funciones del cliente
│   ├── test_server_utils.c    # Tests para funciones del servidor
│   └── test_runner.c          # Ejecutor principal de tests
├── obj/                       # Archivos objeto (generado automáticamente)
├── bin/                       # Ejecutables (generado automáticamente)
├── Makefile                   # Configuración de compilación
└── README.md                  # Este archivo
```

## 🧪 Tests Implementados

### Tests del Cliente (`test_client_utils.c`)

- **Creación de paquetes**: Verificar creación y manipulación de estructuras `t_paquete`
- **Manejo de configuración**: Tests para carga de archivos `.config`
- **Sistema de logging**: Verificar creación y uso de loggers
- **Envío de mensajes**: Tests para serialización y envío de datos
- **Funciones auxiliares**: Tests para limpieza de memoria y utilidades

### Tests del Servidor (`test_server_utils.c`)

- **Logging del servidor**: Verificar sistema de logs del servidor
- **Manejo de datos**: Tests para procesamiento de mensajes recibidos
- **Códigos de operación**: Validar enumeraciones `MENSAJE` y `PAQUETE`
- **Simulación de buffers**: Tests para deserialización de datos
- **Constantes**: Verificar valores predefinidos como puertos

## 🚀 Instalación y Configuración

### Dependencias Requeridas

1. **CSpec Framework**

   ```bash
   # Opción 1: Instalar desde repositorio (Ubuntu/Debian)
   sudo apt-get install libcspecs-dev

   # Opción 2: Compilar desde fuente
   git clone https://github.com/mumuki/cspec.git
   cd cspec
   make
   sudo make install
   ```

2. **so-commons-library**

   ```bash
   git clone https://github.com/sisoputnfrba/so-commons-library.git
   cd so-commons-library
   make
   sudo make install
   ```

3. **Bibliotecas del sistema**
   ```bash
   sudo apt-get install build-essential libreadline-dev
   ```

### Verificar Dependencias

```bash
cd tests/
make check-deps
```

## 🔨 Compilación y Ejecución

### Comandos Básicos

```bash
# Compilar y ejecutar todos los tests
make test

# Solo compilar (sin ejecutar)
make build

# Limpiar archivos generados
make clean

# Limpieza profunda (incluye logs)
make clean-all
```

### Comandos Específicos

```bash
# Ejecutar solo tests del cliente
make test-client

# Ejecutar solo tests del servidor
make test-server

# Ver información de debug
make debug

# Ver ayuda
make help
```

## 📊 Interpretación de Resultados

### Símbolos de Resultado

- ✅ `✓` - Test pasó exitosamente
- ❌ `✗` - Test falló
- 📦 `📦` - Contexto de tests de paquetes
- ⚙️ `⚙️` - Contexto de tests de configuración
- 📝 `📝` - Contexto de tests de logging

### Ejemplo de Salida Exitosa

```
================================================================
           EJECUTANDO TESTS UNITARIOS - TP0 SO
================================================================

🔧 TESTS DEL CLIENTE
--------------------
📦 Ejecutando tests de paquetes...
  ✓ debería crear un paquete vacío correctamente
  ✓ debería agregar datos a un paquete correctamente
  ✓ debería serializar un paquete correctamente

⚙️  Ejecutando tests de configuración...
  ✓ debería cargar configuración desde archivo correctamente
  ✓ debería retornar NULL para archivo inexistente
```

## 🐛 Resolución de Problemas

### Error: "cspecs not found"

```bash
# Instalar CSpec
sudo apt-get install libcspecs-dev
# O compilar desde fuente (ver sección de dependencias)
```

### Error: "commons not found"

```bash
# Instalar so-commons-library
git clone https://github.com/sisoputnfrba/so-commons-library.git
cd so-commons-library
make && sudo make install
```

### Error: "No such file or directory"

Asegúrate de estar en el directorio `tests/` y que los directorios padre (`client/` y `server/`) contengan el código fuente.

### Tests que fallan

1. Verifica que el código fuente principal esté compilando correctamente
2. Revisa que los archivos de configuración de prueba se estén creando
3. Verifica permisos de escritura en el directorio de tests

## 📝 Agregar Nuevos Tests

### 1. Crear nuevo contexto de test

```c
context(test_nueva_funcionalidad) {
    describe("Descripción de la funcionalidad") {
        it("debería hacer algo específico") {
            // Código del test
            should_int(resultado) be equal to(valor_esperado);
        } end
    } end
} end
```

### 2. Declarar en test_runner.c

```c
// Agregar declaración
extern context(test_nueva_funcionalidad);

// Agregar ejecución en main()
cspec_run_context(test_nueva_funcionalidad, "", "");
```

### 3. Recompilar

```bash
make clean && make test
```

## 🔍 Funciones de CSpec Disponibles

### Assertions Básicas

```c
should_bool(valor) be equal to(true);
should_int(valor) be equal to(42);
should_string(cadena) be equal to("esperado");
should_ptr(puntero) be equal to(NULL);
should_not_be_null(puntero);
should_be_null(puntero);
```

### Comparaciones Numéricas

```c
should_int(valor) be greater than(10);
should_int(valor) be less than(100);
should_double(valor) be equal to(3.14) with delta(0.01);
```

## 📚 Referencias

- [CSpec Documentation](https://github.com/mumuki/cspec)
- [so-commons-library](https://github.com/sisoputnfrba/so-commons-library)
- [Cátedra de Sistemas Operativos - UTN FRBA](https://www.utn.edu.ar/)

## 🤝 Contribuir

Para agregar o mejorar tests:

1. Seguir la estructura de contextos existente
2. Usar nombres descriptivos para los tests
3. Incluir comentarios explicativos
4. Verificar que no haya memory leaks
5. Actualizar este README si es necesario

---

**Nota**: Estos tests están diseñados para funcionar sin necesidad de conexiones de red reales, utilizando simulaciones y mocks cuando es necesario.
