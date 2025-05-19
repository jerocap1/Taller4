
#define MAX_PRODUCTOS 5
#define MAX_COMPONENTES 5
#define MAX_NOMBRE 50
#define MAX_FACTURADOS 20

extern char nombresProductos[MAX_PRODUCTOS][MAX_NOMBRE];
extern char nombresComponentes[MAX_COMPONENTES][MAX_NOMBRE];
extern int matrizComponentes[MAX_PRODUCTOS][MAX_COMPONENTES];
extern int tiempoFabricacion[MAX_PRODUCTOS];
extern int inventarioComponentes[MAX_COMPONENTES];
extern int pedidosConfirmados;

int validacionRecursos();
int validacionInt();
void ingresarPedido();
void calcularRequerimientos(int tipoProducto, int cantidad, int* tiempoTotal, int requeridos[MAX_COMPONENTES]);
int verificarDisponibilidad(int requeridos[MAX_COMPONENTES], int tiempoDisponible, int tiempoNecesario);
void aplicarPedido(int requeridos[MAX_COMPONENTES]);
void mostrarSugerencia(int requeridos[MAX_COMPONENTES], int tiempoDisponible, int tiempoNecesario);
void mostrarProductos();
void mostrarInventario();
void reabastecerInventario();
void mostrarResumenFinal();
void editarOEliminarProducto();

