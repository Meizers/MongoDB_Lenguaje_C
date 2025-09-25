#ifndef DB_H
#define DB_H

#include <mongoc/mongoc.h>

extern mongoc_client_t *conn;
extern mongoc_database_t *database;

#define DB "Ejercicio_4"

int abrirBD();
void cerrarBD();

#endif