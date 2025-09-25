#ifndef FACTURA_H
#define FACTURA_H

typedef struct factura {
    char codigo_factura[10];
    char fecha[16];
    double monto;
    char codigo_cliente[4]; // Relación con cliente
} factura;

int cargarFacturas();
void mostrarFacturas();
void modificarFactura();
void borrarFactura();
void borrarTodasFacturas();
void mostrarFacturasDeCliente();

#endif
