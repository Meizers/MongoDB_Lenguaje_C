#include <stdio.h>
#include "mongodb.h"
#include "Cliente.h"
#include "Factura.h"
#include "Utilidades.h"


int menu() {
    int opcion;
    printf("\nMenu Principal\n");
    printf("1. Cargar cliente\n");
    printf("2. Mostrar clientes\n");
    printf("3. Modificar cliente\n");
    printf("4. Borrar cliente\n");
    printf("5. Borrar todos los clientes\n");
    printf("6. Cargar factura\n");
    printf("7. Mostrar facturas\n");
    printf("8. Modificar factura\n");
    printf("9. Borrar factura\n");
    printf("10. Borrar todas las facturas\n");
    printf("11. Mostrar facturas de un cliente\n");
    printf("0. Salir\n");
    ingreso_entero("Ingrese opcion: ", &opcion);
    return opcion;
}

int main() {
    if (abrirBD() == -1) {
        printf("Error abriendo base de datos\n");
        return -1;
    }

    int opcion;
    while ((opcion = menu()) != 0) {
        switch(opcion) {
            case 1: cargarClientes(); break;
            case 2: mostrarClientes(); break;
            case 3: modificarCliente(); break;
            case 4: borrarCliente(); break;
            case 5: borrarTodosClientes(); break;
            case 6: cargarFacturas(); break;
            case 7: mostrarFacturas(); break;
            case 8: modificarFactura(); break;
            case 9: borrarFactura(); break;
            case 10: borrarTodasFacturas(); break;
            case 11: mostrarFacturasDeCliente(); break;
        }
    }

    cerrarBD();
    return 0;
}