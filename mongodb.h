#ifndef DB_H
#define DB_H

#define USER "myUserAdmin"
#define PASSWORD "abc123"
#define HOST "localhost"
#define PORT "27017"

#define MONGO_URL "mongodb://" USER ":" PASSWORD "@" HOST ":" PORT

#include <mongoc/mongoc.h>

extern mongoc_client_t *conn;
extern mongoc_database_t *database;

#define DB "Ejercicio_4"

int abrirBD();
void cerrarBD();

#endif