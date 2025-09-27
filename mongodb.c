#include <stdio.h>
#include "mongodb.h"

mongoc_client_t *conn = NULL;
mongoc_database_t *database = NULL;

int abrirBD() {
    mongoc_init();
    bson_error_t error;

    conn = mongoc_client_new(MONGO_URL);
    if (!conn) {
        fprintf(stderr, "Error conectando a MongoDB\n");
        return -1;
    }

    mongoc_client_set_appname(conn, "ABMCMongo");
    database = mongoc_client_get_database(conn, DB);

    return 0;
}

void cerrarBD() {
    mongoc_database_destroy(database);
    mongoc_client_destroy(conn);
    mongoc_cleanup();
}