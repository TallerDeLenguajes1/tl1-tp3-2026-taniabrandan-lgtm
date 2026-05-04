/*
 * ArregloDeNombres.c
 * TP3 - Taller de Lenguajes I - UNT 2026
 *
 * CONCEPTOS USADOS:
 *   - malloc(): reserva memoria dinámica en el heap
 *   - free(): libera la memoria reservada (evita "memory leaks")
 *   - strlen(): devuelve la cantidad de caracteres de una cadena
 *   - strcpy(): copia una cadena en otra
 *   - strstr(): busca si una cadena contiene otra (como "buscar en texto")
 *   - fgets(): lee una línea del teclado de forma segura
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 5   /* Cantidad de nombres. Con #define creamos una constante.
                 Si queremos cambiar la cantidad, solo cambiamos este número. */

/* =====================================================================
   FUNCION: MostrarPersonas
   QUE HACE: Recorre el vector e imprime cada nombre.
   PARAMETROS:
     - char *V[]  → el vector de punteros a char (cada uno apunta a un nombre)
     - int cant   → cuantos nombres hay en el vector
   RETORNA: nada (void)
   ===================================================================== */
void MostrarPersonas(char *V[], int cant) {
    int i;
    printf("\n--- Lista de personas ---\n");
    for (i = 0; i < cant; i++) {
        /* V[i] es un puntero que apunta al bloque de memoria donde
           esta guardado el nombre i-esimo. Con %s lo imprimimos. */
        printf("  [%d] %s\n", i, V[i]);
    }
    printf("-------------------------\n");
}

/* =====================================================================
   FUNCIÓN: BuscaNombrePorId   (viene del branch busca-nombre, integrado)
   QUÉ HACE: Busca un nombre según su índice (posición en el vector).
   PARÁMETROS:
     - char *V[]  → el vector de nombres
     - int cant   → tamaño del vector
     - int id     → índice a buscar
   RETORNA: nada (void), imprime resultado en pantalla
   ===================================================================== */
void BuscaNombrePorId(char *V[], int cant, int id) {
    /* Validamos que el id esté dentro del rango válido [0, cant-1] */
    if (id >= 0 && id < cant) {
        printf("  Nombre en posición %d: %s\n", id, V[id]);
    } else {
        printf("  no se encontró el valor buscado\n");
    }
}

/* =====================================================================
   FUNCIÓN: BuscaNombrePorPalabra   (viene del branch main, integrado)
   QUÉ HACE: Busca el PRIMER nombre que contenga la palabra clave dada.
   PARÁMETROS:
     - char *V[]        → el vector de nombres
     - int cant         → tamaño del vector
     - char *palabraClave → texto a buscar dentro de los nombres
   RETORNA: int → índice donde se encontró, o -1 si no se encontró
   
   SOBRE strstr():
     strstr(cadena, subcadena) devuelve un puntero a la primera
     aparición de "subcadena" dentro de "cadena", o NULL si no existe.
     Ej: strstr("Carlos Ruiz", "Ruiz") → no es NULL → lo encontró
   ===================================================================== */
int BuscaNombrePorPalabra(char *V[], int cant, char *palabraClave) {
    int i;
    for (i = 0; i < cant; i++) {
        /* Si strstr NO devuelve NULL, significa que encontró la palabra */
        if (strstr(V[i], palabraClave) != NULL) {
            return i;   /* Devolvemos el índice donde lo encontramos */
        }
    }
    return -1;  /* No lo encontramos en ningún nombre */
}

/* =====================================================================
   FUNCIÓN PRINCIPAL
   ===================================================================== */
int main() {
    /* -------------------------------------------------------------------
       DECLARACIONES
       char *V[N] → vector de N punteros a char.
       Cada V[i] empieza apuntando a "basura" (no inicializado),
       por eso debemos asignarle memoria dinámica antes de usarlo.
       ------------------------------------------------------------------- */
    char *V[N];

    /* Buff es el buffer auxiliar para leer del teclado.
       Lo hacemos grande (100) para que entre cualquier nombre.
       Después reservamos exactamente strlen(Buff)+1 bytes para el nombre.
       (+1 porque las cadenas en C terminan con el carácter '\0') */
    char Buff[100];

    int i, opcion, id, resultado;

    /* -------------------------------------------------------------------
       PARTE 1: Ingreso de nombres
       ------------------------------------------------------------------- */
    printf("=== Ingreso de %d nombres ===\n", N);
    for (i = 0; i < N; i++) {
        printf("Ingrese el nombre %d: ", i);

        /* fgets lee hasta 99 caracteres + '\0'. Es más seguro que gets().
           stdin significa "teclado" (standard input).
           Nota: fgets incluye el '\n' al final, lo eliminamos abajo. */
        fgets(Buff, sizeof(Buff), stdin);

        /* Eliminar el '\n' que fgets agrega al final */
        Buff[strcspn(Buff, "\n")] = '\0';

        /* Ahora reservamos memoria EXACTA para este nombre:
             strlen(Buff) caracteres + 1 para el '\0' final
           malloc devuelve un void*, lo casteamos a char* */
        V[i] = (char *) malloc((strlen(Buff) + 1) * sizeof(char));

        if (V[i] == NULL) {
            /* Siempre verificar que malloc no devolvió NULL.
               NULL significa que no había memoria disponible. */
            printf("Error: no se pudo reservar memoria.\n");
            return 1;
        }

        /* Copiamos el contenido de Buff en V[i] */
        strcpy(V[i], Buff);
    }

    /* Mostramos todos los nombres ingresados */
    MostrarPersonas(V, N);

    /* -------------------------------------------------------------------
       PARTE 3: Interfaz de usuario interactiva
       ------------------------------------------------------------------- */
    do {
        printf("\n=== MENU DE BUSQUEDA ===\n");
        printf("  1. Buscar por ID (posición)\n");
        printf("  2. Buscar por palabra clave\n");
        printf("  0. Salir\n");
        printf("Elija una opción: ");
        scanf("%d", &opcion);
        getchar(); /* Consumir el '\n' que quedó en el buffer tras scanf */

        switch (opcion) {
            case 1:
                printf("Ingrese el ID (0 a %d): ", N - 1);
                scanf("%d", &id);
                getchar();
                BuscaNombrePorId(V, N, id);
                break;

            case 2:
                printf("Ingrese la palabra clave: ");
                fgets(Buff, sizeof(Buff), stdin);
                Buff[strcspn(Buff, "\n")] = '\0';

                resultado = BuscaNombrePorPalabra(V, N, Buff);
                if (resultado == -1) {
                    printf("  No se encontró ningún nombre con esa palabra.\n");
                } else {
                    printf("  Encontrado en posición %d: %s\n", resultado, V[resultado]);
                }
                break;

            case 0:
                printf("Saliendo...\n");
                break;

            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 0);

    /* -------------------------------------------------------------------
       LIBERAR MEMORIA
       Muy importante: todo lo que reservamos con malloc() hay que
       liberarlo con free() para no tener "memory leaks" (fugas de memoria).
       ------------------------------------------------------------------- */
    for (i = 0; i < N; i++) {
        free(V[i]);
        V[i] = NULL; /* Buena práctica: poner en NULL después de liberar */
    }

    return 0;
}