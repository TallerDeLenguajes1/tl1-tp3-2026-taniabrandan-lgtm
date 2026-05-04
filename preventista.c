/*
 * Ejercicio3_Preventistas.c
 * Taller de Lenguajes I - TP Nro. 3
 *
 * CONCEPTOS NUEVOS EN ESTE EJERCICIO:
 * - struct: permite agrupar datos de distintos tipos bajo un mismo nombre.
 * - rand() y srand(): generan números pseudoaleatorios en C.
 * - time(NULL): devuelve el tiempo actual, se usa como "semilla" para rand()
 *               para que los números sean distintos cada vez que se ejecuta.
 * - Punteros a struct: se puede reservar dinámicamente un arreglo de structs.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   /* Para usar time(NULL) como semilla del generador aleatorio */
 
/* ---------------------------------------------------------------
 * Arreglo global con los tipos de productos disponibles.
 * Es un arreglo de punteros a cadenas constantes (literales).
 * --------------------------------------------------------------- */
char *TiposProductos[] = {"Galletas", "Snack", "Cigarrillos", "Caramelos", "Bebidas"};
#define CANT_TIPOS 5   /* Cantidad de tipos de productos */
 
/* ---------------------------------------------------------------
 * STRUCT Producto
 * Un struct agrupa variables de distintos tipos en una sola entidad.
 * Es como una "ficha" que describe un producto.
 * --------------------------------------------------------------- */
struct Producto {
    int   ProductoID;       /* Número de producto (se asigna en el ciclo) */
    int   Cantidad;         /* Entre 1 y 10 */
    char *TipoProducto;     /* Apunta a uno de los strings de TiposProductos */
    float PrecioUnitario;   /* Entre 10.0 y 100.0 */
};
 
/* ---------------------------------------------------------------
 * STRUCT Cliente
 * Agrupa los datos de un cliente.
 * Notar que contiene un puntero a un arreglo dinámico de Productos.
 * --------------------------------------------------------------- */
struct Cliente {
    int             ClienteID;
    char           *NombreCliente;          /* Reservado dinámicamente */
    int             CantidadProductosAPedir;/* Aleatorio entre 1 y 5 */
    struct Producto *Productos;             /* Arreglo dinámico de Productos */
};
 
/* ---------------------------------------------------------------
 * FUNCIÓN: CostoProducto
 * Recibe un producto (por valor, es decir una copia).
 * Devuelve Cantidad * PrecioUnitario.
 *
 * Recibimos el struct por valor porque solo necesitamos leerlo,
 * no modificarlo. Para structs grandes sería mejor pasar por puntero,
 * pero aquí es suficiente así.
 * --------------------------------------------------------------- */
float CostoProducto(struct Producto p) {
    return p.Cantidad * p.PrecioUnitario;
}
 
/* ---------------------------------------------------------------
 * FUNCIÓN: GenerarProductoAleatorio
 * Rellena los campos de un Producto con valores aleatorios.
 *
 * ¿Cómo funciona rand() % N?
 * rand() devuelve un entero aleatorio grande.
 * Con % N obtenemos el resto de dividir por N → resultado entre 0 y N-1.
 * Sumándole 1 obtenemos entre 1 y N.
 * --------------------------------------------------------------- */
void GenerarProductoAleatorio(struct Producto *p, int id) {
    p->ProductoID = id;
 
    /* Cantidad aleatoria entre 1 y 10 */
    p->Cantidad = (rand() % 10) + 1;
 
    /* Tipo aleatorio: elegimos un índice entre 0 y CANT_TIPOS-1 */
    int tipoIdx = rand() % CANT_TIPOS;
    p->TipoProducto = TiposProductos[tipoIdx];
 
    /* Precio aleatorio entre 10.0 y 100.0
     * rand() % 91 da entre 0 y 90, sumamos 10 → entre 10 y 100 */
    p->PrecioUnitario = (float)((rand() % 91) + 10);
}
 
/* ---------------------------------------------------------------
 * FUNCIÓN PRINCIPAL
 * --------------------------------------------------------------- */
int main() {
    /*
     * Inicializamos el generador de números aleatorios con la hora actual.
     * Sin esto, rand() siempre daría la misma secuencia de números.
     * srand(semilla) "mezcla" el generador de forma distinta cada vez.
     */
    srand((unsigned int) time(NULL));
 
    char Buff[100];   /* Buffer auxiliar para leer nombres */
 
    /* ---- i) Solicitar cantidad de clientes ---- */
    int cantClientes;
    printf("=== Sistema de Preventistas ===\n\n");
    printf("Ingrese la cantidad de clientes (max 5): ");
    scanf("%d", &cantClientes);
    getchar();   /* Consumir el '\n' del buffer */
 
    /* Validación básica */
    if (cantClientes < 1 || cantClientes > 5) {
        printf("Cantidad invalida. Debe ser entre 1 y 5.\n");
        return 1;
    }
 
    /* ---- ii) Reservar el arreglo de clientes dinámicamente ---- */
    /*
     * Reservamos un arreglo de 'cantClientes' structs Cliente.
     * Cada struct ocupa sizeof(struct Cliente) bytes.
     * malloc nos devuelve un puntero al inicio del bloque.
     */
    struct Cliente *clientes = (struct Cliente *) malloc(cantClientes * sizeof(struct Cliente));
    if (clientes == NULL) {
        printf("Error al reservar memoria para clientes.\n");
        return 1;
    }
 
    /* ---- ii y iii) Cargar cada cliente y generar sus productos ---- */
    for (int i = 0; i < cantClientes; i++) {
        clientes[i].ClienteID = i + 1;   /* IDs desde 1 */
 
        printf("\n--- Cargando cliente %d ---\n", clientes[i].ClienteID);
        printf("Ingrese el nombre del cliente: ");
        fgets(Buff, sizeof(Buff), stdin);
        Buff[strcspn(Buff, "\n")] = '\0';   /* Eliminamos el '\n' final */
 
        /* Reservamos memoria exacta para el nombre */
        clientes[i].NombreCliente = (char *) malloc((strlen(Buff) + 1) * sizeof(char));
        if (clientes[i].NombreCliente == NULL) {
            printf("Error al reservar memoria para el nombre.\n");
            return 1;
        }
        strcpy(clientes[i].NombreCliente, Buff);
 
        /* Cantidad de productos: aleatoria entre 1 y 5 */
        clientes[i].CantidadProductosAPedir = (rand() % 5) + 1;
        printf("Cantidad de productos asignada aleatoriamente: %d\n",
               clientes[i].CantidadProductosAPedir);
 
        /* Reservamos el arreglo dinámico de productos para este cliente */
        clientes[i].Productos = (struct Producto *) malloc(
            clientes[i].CantidadProductosAPedir * sizeof(struct Producto)
        );
        if (clientes[i].Productos == NULL) {
            printf("Error al reservar memoria para productos.\n");
            return 1;
        }
 
        /* Generamos cada producto aleatoriamente */
        for (int j = 0; j < clientes[i].CantidadProductosAPedir; j++) {
            GenerarProductoAleatorio(&clientes[i].Productos[j], j + 1);
        }
    }
 
    /* ---- v) Mostrar todo lo cargado ---- */
    printf("\n\n========================================\n");
    printf("       RESUMEN DE PREVENTISTAS\n");
    printf("========================================\n");
 
    for (int i = 0; i < cantClientes; i++) {
        printf("\nCliente ID: %d\n", clientes[i].ClienteID);
        printf("Nombre    : %s\n", clientes[i].NombreCliente);
        printf("Productos (%d):\n", clientes[i].CantidadProductosAPedir);
 
        float totalCliente = 0.0;
 
        for (int j = 0; j < clientes[i].CantidadProductosAPedir; j++) {
            struct Producto p = clientes[i].Productos[j];
            float costo = CostoProducto(p);
            totalCliente += costo;
 
            printf("  Producto nro %d:\n", j + 1);
            printf("    ProductoID    : %d\n", p.ProductoID);
            printf("    Cantidad      : %d\n", p.Cantidad);
            printf("    TipoProducto  : %s\n", p.TipoProducto);
            printf("    PrecioUnitario: %.2f\n", p.PrecioUnitario);
            printf("    Subtotal      : %.2f\n", costo);
        }
 
        printf("  ----------------------------------------\n");
        printf("  TOTAL A PAGAR por %s: $%.2f\n",
               clientes[i].NombreCliente, totalCliente);
    }
 
    printf("\n========================================\n");
 
    /* ---- Liberamos toda la memoria dinámica ---- */
    /*
     * Liberamos en orden inverso a como reservamos:
     * primero los productos de cada cliente,
     * luego el nombre de cada cliente,
     * finalmente el arreglo de clientes.
     */
    for (int i = 0; i < cantClientes; i++) {
        free(clientes[i].Productos);       /* Liberamos el arreglo de productos */
        free(clientes[i].NombreCliente);   /* Liberamos el nombre */
    }
    free(clientes);   /* Liberamos el arreglo de clientes */
 
    printf("\nMemoria liberada correctamente.\n");
    return 0;
}