/*
 * Ejercicio4_ArregloDinamico.c
 * Taller de Lenguajes I - TP Nro. 3
 *
 * MODIFICACIÓN RESPECTO AL EJERCICIO 2:
 * En lugar de usar N=5 fijo, el usuario ingresa primero cuántos nombres quiere.
 * Luego reservamos el vector de punteros también dinámicamente (char **).
 *
 * CONCEPTO NUEVO: char **
 * Un char* es un puntero a char (una cadena).
 * Un char** es un puntero a puntero a char (un arreglo dinámico de cadenas).
 * Es como el vector[N] del ejercicio anterior, pero sin el N fijo.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/* ---------------------------------------------------------------
 * Las mismas funciones del ejercicio 2, ahora reciben char **
 * en lugar de char *vector[] (son equivalentes en este contexto)
 * --------------------------------------------------------------- */
 
void MostrarPersonas(char **vector, int cantidad) {
    printf("\n=== Lista de personas ===\n");
    for (int i = 0; i < cantidad; i++) {
        printf("  [%d] %s\n", i, vector[i]);
    }
    printf("=========================\n\n");
}
 
void BuscarNombrePorId(char **vector, int cantidad, int id) {
    if (id >= 0 && id < cantidad) {
        printf("Nombre encontrado en posicion [%d]: %s\n", id, vector[id]);
    } else {
        printf("no se encontró el valor buscado\n");
    }
}
 
int BuscarNombrePorPalabra(char **vector, int cantidad, char *clave) {
    for (int i = 0; i < cantidad; i++) {
        if (strstr(vector[i], clave) != NULL) {
            return i;
        }
    }
    return -1;
}
 
/* ---------------------------------------------------------------
 * FUNCIÓN PRINCIPAL
 * --------------------------------------------------------------- */
int main() {
    char Buff[50];
 
    /* El usuario elige cuántos nombres ingresará */
    int n;
    printf("¿Cuántos nombres desea ingresar? ");
    scanf("%d", &n);
    getchar();
 
    if (n <= 0) {
        printf("La cantidad debe ser mayor a 0.\n");
        return 1;
    }
 
    /*
     * Reservamos el VECTOR dinámicamente.
     * Ahora no es char *vector[N] (tamaño fijo), sino char **vector.
     * Es un bloque de n punteros a char, cada uno apuntará a un nombre.
     *
     * Esquema de memoria:
     *
     *  vector (char**):
     *  +--------+        +----------+
     *  | ptr[0] |  --->  | nombre0\0|  (bloque dinámico para el nombre)
     *  +--------+        +----------+
     *  | ptr[1] |  --->  | nombre1\0|
     *  +--------+        +----------+
     *  ...
     */
    char **vector = (char **) malloc(n * sizeof(char *));
    if (vector == NULL) {
        printf("Error al reservar memoria para el vector.\n");
        return 1;
    }
 
    /* Ingreso de nombres */
    printf("\n=== Ingreso de %d nombres ===\n\n", n);
    for (int i = 0; i < n; i++) {
        printf("Ingrese el nombre de la persona %d: ", i);
        fgets(Buff, sizeof(Buff), stdin);
        Buff[strcspn(Buff, "\n")] = '\0';
 
        vector[i] = (char *) malloc((strlen(Buff) + 1) * sizeof(char));
        if (vector[i] == NULL) {
            printf("Error al reservar memoria.\n");
            return 1;
        }
        strcpy(vector[i], Buff);
    }
 
    MostrarPersonas(vector, n);
 
    /* Menú de búsqueda (igual al ejercicio 2) */
    int opcion;
    do {
        printf("--- Menu de busqueda ---\n");
        printf("  1. Buscar por ID\n");
        printf("  2. Buscar por palabra clave\n");
        printf("  0. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);
        getchar();
 
        if (opcion == 1) {
            int id;
            printf("Ingrese el ID (del 0 al %d): ", n - 1);
            scanf("%d", &id);
            getchar();
            BuscarNombrePorId(vector, n, id);
 
        } else if (opcion == 2) {
            printf("Ingrese la palabra clave: ");
            fgets(Buff, sizeof(Buff), stdin);
            Buff[strcspn(Buff, "\n")] = '\0';
 
            int resultado = BuscarNombrePorPalabra(vector, n, Buff);
            if (resultado == -1) {
                printf("No se encontró ningún nombre con la clave \"%s\".\n", Buff);
            } else {
                printf("Primer nombre encontrado: %s (posicion %d)\n",
                       vector[resultado], resultado);
            }
        } else if (opcion != 0) {
            printf("Opcion invalida.\n");
        }
        printf("\n");
 
    } while (opcion != 0);
 
    /* Liberamos toda la memoria */
    for (int i = 0; i < n; i++) {
        free(vector[i]);   /* Liberamos cada nombre */
    }
    free(vector);          /* Liberamos el vector en sí */
 
    printf("Memoria liberada correctamente.\n");
    return 0;
}