#include <stdio.h>
#include "funciones.h"

int main() {
    int opcion;
    do {
        printf("\n=== URQUIMUEBLES ===\n");
        printf("1. Ingresar pedido\n");
        printf("2. Mostrar productos\n");
        printf("3. Ver inventario\n");
        printf("4. Reabastecer componentes\n");
        printf("5. Editar o eliminar producto\n");
        printf("6. Agregar nuevo producto\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        opcion = validacionInt();

        switch (opcion) {
            case 1: ingresarPedido(); break;
            case 2: mostrarProductos(); break;
            case 3: mostrarInventario(); break;
            case 4: reabastecerInventario(); break;
            case 5: editarOEliminarProducto(); break;
            case 6: agregarProducto(); break;
            case 7:
                printf("Saliendo del programa...\n");
                break;
            default: printf("Opcion invalida.\n");
        }
    } while (opcion != 7);
    return 0;
}
