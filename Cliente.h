#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct cliente {
    char codigo[4];
    char nombre[64];
} cliente;

int cargarClientes();
void mostrarClientes();
void mostrarClientesJSON();
void modificarCliente();
void borrarCliente();
void borrarTodosClientes();

#endif