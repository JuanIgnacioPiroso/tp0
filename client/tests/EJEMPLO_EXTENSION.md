# Ejemplo: Cómo Extender los Tests

Este documento muestra cómo agregar nuevos tests al proyecto de manera sencilla.

## 📝 Ejemplo: Agregar Tests para una Nueva Función

Supongamos que tienes una nueva función en `client.c`:

```c
/**
 * @brief Valida que un mensaje no esté vacío
 * @param mensaje El mensaje a validar
 * @return true si es válido, false si está vacío o es NULL
 */
bool validar_mensaje(char* mensaje) {
    if (mensaje == NULL) return false;
    if (strlen(mensaje) == 0) return false;
    return true;
}
```

## 🧪 Paso 1: Agregar Tests en `test_client_utils.c`

```c
// Agregar al final del archivo, antes del último } end

context(test_validacion_mensajes) {

    describe("Validación de mensajes") {

        it("debería validar mensaje correcto") {
            char* mensaje_valido = "Hola mundo";
            bool resultado = validar_mensaje(mensaje_valido);
            should_bool(resultado) be equal to(true);
        } end

        it("debería rechazar mensaje NULL") {
            bool resultado = validar_mensaje(NULL);
            should_bool(resultado) be equal to(false);
        } end

        it("debería rechazar mensaje vacío") {
            char* mensaje_vacio = "";
            bool resultado = validar_mensaje(mensaje_vacio);
            should_bool(resultado) be equal to(false);
        } end

        it("debería validar mensaje con espacios") {
            char* mensaje_espacios = "  mensaje con espacios  ";
            bool resultado = validar_mensaje(mensaje_espacios);
            should_bool(resultado) be equal to(true);
        } end

    } end

} end
```

## 🔗 Paso 2: Declarar en `test_runner.c`

```c
// Agregar en la sección de declaraciones
extern context(test_validacion_mensajes);

// Agregar en main(), en la sección de tests del cliente
printf("\n🔍 Ejecutando tests de validación de mensajes...\n");
cspec_run_context(test_validacion_mensajes, "", "");
```

## 🔨 Paso 3: Recompilar y Ejecutar

```bash
cd tests/
make clean && make test
```

## 📊 Ejemplo de Salida Esperada

```
🔍 Ejecutando tests de validación de mensajes...
  ✓ debería validar mensaje correcto
  ✓ debería rechazar mensaje NULL
  ✓ debería rechazar mensaje vacío
  ✓ debería validar mensaje con espacios
```

## 🧪 Tipos de Tests Comunes

### Tests de Funciones Puras

```c
it("debería calcular suma correctamente") {
    int resultado = sumar(2, 3);
    should_int(resultado) be equal to(5);
} end
```

### Tests con Setup/Teardown

```c
describe("Manejo de archivos") {

    before {
        // Crear archivo temporal
        FILE* temp = fopen("test_temp.txt", "w");
        fprintf(temp, "contenido de prueba");
        fclose(temp);
    } end

    after {
        // Limpiar archivo temporal
        unlink("test_temp.txt");
    } end

    it("debería leer archivo correctamente") {
        // Test que usa el archivo temporal
    } end

} end
```

### Tests de Manejo de Errores

```c
it("debería manejar división por cero") {
    int resultado = division_segura(10, 0);
    should_int(resultado) be equal to(-1);  // Código de error
} end
```

### Tests de Estructuras de Datos

```c
it("debería crear lista vacía") {
    t_list* lista = list_create();
    should_not_be_null(lista);
    should_int(list_size(lista)) be equal to(0);
    should_bool(list_is_empty(lista)) be equal to(true);
    list_destroy(lista);
} end
```

## 🎯 Mejores Prácticas

### 1. Nombres Descriptivos

```c
// ✅ Bueno
it("debería retornar NULL cuando el archivo no existe") {

// ❌ Malo
it("test archivo") {
```

### 2. Tests Independientes

```c
// ✅ Cada test limpia después de sí mismo
it("debería crear archivo temporal") {
    FILE* f = fopen("temp.txt", "w");
    // ... test ...
    fclose(f);
    unlink("temp.txt");  // Limpiar
} end
```

### 3. Verificar Múltiples Aspectos

```c
it("debería crear paquete con valores correctos") {
    t_paquete* paquete = crear_paquete();

    // Verificar que se creó
    should_not_be_null(paquete);

    // Verificar valores iniciales
    should_int(paquete->codigo_operacion) be equal to(PAQUETE);
    should_not_be_null(paquete->buffer);
    should_int(paquete->buffer->size) be equal to(0);

    eliminar_paquete(paquete);
} end
```

### 4. Tests de Edge Cases

```c
context(test_edge_cases) {
    describe("Casos límite") {

        it("debería manejar string muy largo") {
            char* string_largo = malloc(10000);
            memset(string_largo, 'A', 9999);
            string_largo[9999] = '\0';

            bool resultado = procesar_string(string_largo);
            should_bool(resultado) be equal to(true);

            free(string_largo);
        } end

        it("debería manejar valores negativos") {
            int resultado = funcion_matematica(-1);
            should_int(resultado) be equal to(0);
        } end

    } end
} end
```

## 🔧 Debugging Tests

### Agregar Prints Temporales

```c
it("debug de estructura compleja") {
    t_paquete* paquete = crear_paquete();

    // Print temporal para debugging
    printf("DEBUG: Paquete creado, size=%d\n", paquete->buffer->size);

    should_not_be_null(paquete);
    eliminar_paquete(paquete);
} end
```

### Usar Assertions Más Específicas

```c
// En lugar de solo verificar que no es NULL
should_not_be_null(puntero);

// Verificar el valor específico
should_ptr(puntero) not be equal to(NULL);
should_int((int)puntero) be greater than(0);
```

## 📚 Referencia Rápida de CSpec

```c
// Comparaciones básicas
should_bool(valor) be equal to(true);
should_int(valor) be equal to(42);
should_string(cadena) be equal to("esperado");
should_ptr(puntero) be equal to(NULL);

// Negaciones
should_not_be_null(puntero);
should_int(valor) not be equal to(0);

// Comparaciones numéricas
should_int(valor) be greater than(10);
should_int(valor) be less than(100);
should_double(valor) be equal to(3.14) with delta(0.01);

// Arrays (requiere función auxiliar)
should_bool(arrays_iguales(arr1, arr2, tam)) be equal to(true);
```

Este ejemplo te da una base sólida para extender los tests según las necesidades de tu proyecto.
