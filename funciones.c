#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "funciones.h"

char nombresProductos[MAX_PRODUCTOS][MAX_NOMBRE] = {
    "Mesa", "Silla", "Cama", "Escritorio", "Armario", "", "", "", "", ""
};
char nombresComponentes[MAX_COMPONENTES][MAX_NOMBRE] = {
    "Madera", "Tornillos", "Clavos", "Barniz", "Bisagras", "", "", "", "", ""
};

int matrizComponentes[MAX_PRODUCTOS][MAX_COMPONENTES] = {
    {4, 20, 10, 2, 4}, 
    {2, 10, 5, 1, 2},   
    {6, 30, 15, 3, 6},  
    {5, 25, 12, 2, 5},  
    {7, 35, 18, 3, 8},  
    {0}, {0}, {0}, {0}, {0}
};
int tiempoFabricacion[MAX_PRODUCTOS] = {300, 200, 400, 350, 450};
int inventarioComponentes[MAX_COMPONENTES] = {100, 200, 150, 80, 120};
int pedidosConfirmados = 0;
int productosFacturados[MAX_FACTURADOS];
int cantidadesFacturadas[MAX_FACTURADOS];
int totalPedidos = 0;
int precios[MAX_PRODUCTOS] = {50, 30, 100, 80, 120};

void mostrarFactura() {
    srand(time(NULL));
    int id = rand() % 9000 + 1000;
    printf("\n====== FACTURA ======\n");
    printf("ID: #%d\n", id);
    printf("URQUIMUEBLES\n");
    printf("----------------------\n");

    float total = 0;

    for (int i = 0; i < totalPedidos; i++) {
       int producto = productosFacturados[i];
        int cantidad = cantidadesFacturadas[i];
        int precioUnitario = precios[producto];
        float subtotal = cantidad * precioUnitario;
        total += subtotal;

        printf("%s - Cantidad: %d - Precio: $%d - Subtotal: $%.2f\n",
               nombresProductos[producto], cantidad, precioUnitario, subtotal);
    }
 
    float iva = total * 0.15;
    float totalFinal = total + iva;

    printf("----------------------\n");
    printf("Total: $%.2f\n", total);
    printf("IVA (15%%): $%.2f\n", iva);
    printf("TOTAL A PAGAR: $%.2f\n", totalFinal);
    printf("======================\n");
}

int validacionInt() {
    int dato;
    int val;
    do {
        val = scanf("%d", &dato);
        if (val != 1 || dato <= 0) {
            printf("Ingrese un numero valido: ");
            while (getchar() != '\n');
        }
    } while (val != 1 || dato <= 0);
    while (getchar() != '\n');
    return dato;
}

int validacionRecursos() {
    int dato;
    int val;
    val = scanf("%d", &dato);
    while (val != 1 || dato < 0) {
        printf("Ingrese un numero valido (puede ser 0): ");
        while (getchar() != '\n');
        val = scanf("%d", &dato);
    }
    while (getchar() != '\n');
    return dato;
}

void ingresarPedido() {
    totalPedidos = 0;
    char continuarPedido = 's';
    while (continuarPedido == 's' || continuarPedido == 'S') {
        int tipo, cantidad, tiempoDisp;
        printf("\nSeleccione el tipo de mueble:\n");
        for (int i = 0; i < MAX_PRODUCTOS; i++)
            if (strlen(nombresProductos[i]) > 0)
                printf("%d. %s\n", i+1, nombresProductos[i]);

        printf("Opcion: ");
        tipo = validacionInt() - 1;

        if (tipo < 0 || tipo >= MAX_PRODUCTOS || strlen(nombresProductos[tipo]) == 0) {
            printf("Opcion invalida.\n");
        } else {
            printf("Ingrese la cantidad a producir: ");
            cantidad = validacionInt();

            printf("Ingrese el tiempo disponible (en minutos): ");
            tiempoDisp = validacionInt();

            int requeridos[MAX_COMPONENTES], tiempoTotal;
            calcularRequerimientos(tipo, cantidad, &tiempoTotal, requeridos);

            int esViable = verificarDisponibilidad(requeridos, tiempoDisp, tiempoTotal);

            if (esViable) {
                char confirmar;
                printf("El pedido se puede cumplir, desea confirmarlo? (s/n): ");
                confirmar = getchar();
                while (getchar() != '\n');
                while (confirmar != 's' && confirmar != 'S' && confirmar != 'n' && confirmar != 'N') {
                    printf("Respuesta invalida. Ingrese 's' o 'n': ");
                    confirmar = getchar();
                    while (getchar() != '\n');
                }
                if (confirmar == 's' || confirmar == 'S') {
                    aplicarPedido(requeridos);
                    pedidosConfirmados++;
                    productosFacturados[totalPedidos] = tipo;
                    cantidadesFacturadas[totalPedidos] = cantidad;
                    totalPedidos++;
                    printf("Pedido confirmado y aplicado.\n");
                } else {
                    printf("Pedido no confirmado.\n");
                }
            } else {
                mostrarSugerencia(requeridos, tiempoDisp, tiempoTotal);
            }
        }

        printf("Desea ingresar otro pedido? (s/n): ");
        continuarPedido = getchar();
        while (getchar() != '\n');
        while (continuarPedido != 's' && continuarPedido != 'S' && continuarPedido != 'n' && continuarPedido != 'N') {
            printf("Respuesta invalida. Ingrese 's' o 'n': ");
            continuarPedido = getchar();
            while (getchar() != '\n');
        }
    }
    if (totalPedidos > 0) {
        mostrarFactura();
        totalPedidos = 0;
    }
}

void calcularRequerimientos(int tipoProducto, int cantidad, int* tiempoTotal, int requeridos[MAX_COMPONENTES]) {
    *tiempoTotal = tiempoFabricacion[tipoProducto] * cantidad;
    for (int i = 0; i < MAX_COMPONENTES; i++)
        requeridos[i] = matrizComponentes[tipoProducto][i] * cantidad;
}

int verificarDisponibilidad(int requeridos[MAX_COMPONENTES], int tiempoDisponible, int tiempoNecesario) {
    if (tiempoNecesario > tiempoDisponible)
        return 0;
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        if (requeridos[i] > inventarioComponentes[i])
            return 0;
    }
    return 1;
}

void aplicarPedido(int requeridos[MAX_COMPONENTES]) {
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        inventarioComponentes[i] -= requeridos[i];
    }
}

void mostrarSugerencia(int requeridos[MAX_COMPONENTES], int tiempoDisponible, int tiempoNecesario) {
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        if (requeridos[i] > inventarioComponentes[i]) {
            printf("Faltan %d unidades de %s.\n", requeridos[i] - inventarioComponentes[i], nombresComponentes[i]);
        }
    }
    if (tiempoNecesario > tiempoDisponible) {
        printf("El tiempo es insuficiente (%d minutos requerido).\n", tiempoNecesario);
    }
}

void mostrarProductos() {
    printf("\n--- Productos y recursos por unidad ---\n");

    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        if (strlen(nombresProductos[i]) > 0) {
            printf("%s (Tiempo: %d minutos) - Precio: $%d\n", 
                   nombresProductos[i], tiempoFabricacion[i], precios[i]);

            for (int j = 0; j < MAX_COMPONENTES; j++) {
                if (strlen(nombresComponentes[j]) > 0 && matrizComponentes[i][j] > 0) {
                    printf(" - %s: %d\n", 
                           nombresComponentes[j], matrizComponentes[i][j]);
                }
            }
        }
    }
}

void mostrarInventario() {
    printf("\n--- Inventario actual ---\n");
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        if (strlen(nombresComponentes[i]) > 0) {
            printf("%s: %d unidades\n", nombresComponentes[i], inventarioComponentes[i]);
        }
    }
}

void reabastecerInventario() {
    char recurso[MAX_NOMBRE];
    int encontrado = -1;

    printf("\n--- Reabastecer componente ---\n");
    printf("Ingrese el nombre del recurso que desea reabastecer: ");
    fgets(recurso, MAX_NOMBRE, stdin);
    recurso[strcspn(recurso, "\n")] = 0;

    for (int i = 0; i < MAX_COMPONENTES; i++) {
        if (strcmp(recurso, nombresComponentes[i])== 0 ) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Recurso no encontrado.\n");
        return;
    }

    printf("Agregar unidades de %s: ", nombresComponentes[encontrado]);
    int extra = validacionInt();
    inventarioComponentes[encontrado] += extra;

    printf("Reabastecimiento completado para %s.\n", nombresComponentes[encontrado]);
}

void editarOEliminarProducto() {
    char nombre[MAX_NOMBRE];
    int encontrado = -1;

    printf("Ingrese el nombre del producto a editar o eliminar: ");
    fgets(nombre, MAX_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        if (strcmp(nombre, nombresProductos[i]) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Producto no encontrado.\n");
        return;
    }

    printf("Producto encontrado: %s\n", nombresProductos[encontrado]);
    printf("Desea (1) Editar o (2) Eliminar? ");
    int opcion = validacionInt();

    if (opcion == 1) {
        printf("Nuevo nombre del producto: ");
        fgets(nombresProductos[encontrado], MAX_NOMBRE, stdin);
        nombresProductos[encontrado][strcspn(nombresProductos[encontrado], "\n")] = 0;

        printf("Nuevo tiempo de fabricacion (en minutos): ");
        tiempoFabricacion[encontrado] = validacionInt();

        for (int j = 0; j < MAX_COMPONENTES; j++) {
            if (strlen(nombresComponentes[j]) > 0) {
                printf("Cantidad de %s por unidad: ", nombresComponentes[j]);
                matrizComponentes[encontrado][j] = validacionRecursos();
            }
        }

        printf("Nuevo precio del producto: ");
        precios[encontrado] = validacionInt();

        printf("Producto actualizado.\n");

    } else if (opcion == 2) {
        strcpy(nombresProductos[encontrado], "");
        tiempoFabricacion[encontrado] = 0;
        precios[encontrado] = 0;
        for (int j = 0; j < MAX_COMPONENTES; j++) {
            matrizComponentes[encontrado][j] = 0;
        }
        printf("Producto eliminado.\n");

    } else {
        printf("Opcion invalida.\n");
    }
}

void agregarProducto() {
    int nuevo = -1;
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        if (strlen(nombresProductos[i]) == 0) {
            nuevo = i;
            break;
        }
    }

    if (nuevo == -1) {
        printf("No hay espacio para agregar mas productos.\n");
        return;
    }

    printf("Ingrese el nombre del nuevo producto: ");
    fgets(nombresProductos[nuevo], MAX_NOMBRE, stdin);
    nombresProductos[nuevo][strcspn(nombresProductos[nuevo], "\n")] = 0;

    printf("Ingrese el tiempo de fabricacion del producto (en minutos): ");
    tiempoFabricacion[nuevo] = validacionInt();

    printf("Ingrese el precio del producto: ");
    precios[nuevo] = validacionInt();

    printf("Ingrese la cantidad de cada recurso que necesita este producto:\n");
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        if (strlen(nombresComponentes[i]) > 0) {
            printf("- %s: ", nombresComponentes[i]);
            matrizComponentes[nuevo][i] = validacionRecursos();
        }
    }

    printf("Este producto necesita nuevos recursos? (s/n): ");
    char respuesta = getchar();
    while (getchar() != '\n');
    while (respuesta != 's' && respuesta != 'S' && respuesta != 'n' && respuesta != 'N') {
        printf("Respuesta invalida. Ingrese 's' o 'n': ");
        respuesta = getchar();
        while (getchar() != '\n');
    }
    if (respuesta == 's' || respuesta == 'S') {
        int espaciosLibres = 0;
        for (int i = 0; i < MAX_COMPONENTES; i++) {
            if (strlen(nombresComponentes[i]) == 0)
                espaciosLibres++;
        }
        if (espaciosLibres == 0) {
            printf("No hay espacio para agregar mas recursos.\n");
        } else {
            printf("Cuantos recursos nuevos desea agregar? (max %d): ", espaciosLibres);
            int nuevosRecursos = validacionInt();
            if (nuevosRecursos > espaciosLibres) nuevosRecursos = espaciosLibres;

            for (int r = 0; r < nuevosRecursos; r++) {
                int pos = -1;
                for (int i = 0; i < MAX_COMPONENTES; i++) {
                    if (strlen(nombresComponentes[i]) == 0) {
                        pos = i;
                        break;
                    }
                }
                if (pos == -1) break;

                printf("Ingrese el nombre del nuevo recurso #%d: ", r + 1);
                fgets(nombresComponentes[pos], MAX_NOMBRE, stdin);
                nombresComponentes[pos][strcspn(nombresComponentes[pos], "\n")] = 0;

                printf("Cuantas unidades de %s requiere el producto?: ", nombresComponentes[pos]);
                matrizComponentes[nuevo][pos] = validacionInt();
            }
        }
    }

    printf("Producto agregado correctamente.\n");
}
