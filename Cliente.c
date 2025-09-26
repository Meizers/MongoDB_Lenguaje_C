#include <stdio.h>
#include <string.h>
#include "Cliente.h"
#include "mongodb.h"
#include "Utilidades.h"

#define COLLECTION_CLIENTE "cliente"

int cargarClientes() {
    bson_error_t error;
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_CLIENTE);

    cliente c;
    ingreso_cadena("Ingrese codigo cliente (0 para salir): ", c.codigo, 4);

    while (c.codigo[0] != '0') {
        ingreso_cadena("Ingrese nombre cliente: ", c.nombre, 64);

        // armar JSON
        char json[256];
        snprintf(json, sizeof(json),
                 "{\"codigo\": \"%s\", \"nombre\": \"%s\"}",
                 c.codigo, c.nombre);

        bson_t *bson = bson_new_from_json((const uint8_t *) json, -1, &error);
        if (bson == NULL) {
            printf("Error creando BSON Cliente: %s\n", error.message);
            return -1;
        }

        if (!mongoc_collection_insert_one(collection, bson, NULL, NULL, &error)) {
            printf("Error insertando Cliente %s: %s\n", c.codigo, error.message);
        }

        bson_destroy(bson);

        ingreso_cadena("Ingrese codigo cliente (0 para salir): ", c.codigo, 4);
    }

    mongoc_collection_destroy(collection);
    return 0;
}

void mostrarClientes() {
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_CLIENTE);
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_t *query = bson_new();

    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    while (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
}

void mostrarClientesJSON() {
    mostrarClientes();
}

void modificarCliente() {
    bson_error_t error;
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_CLIENTE);

    char codigo[4], nombre[64];
    ingreso_cadena("Codigo cliente a modificar: ", codigo, 4);
    ingreso_cadena("Nuevo nombre: ", nombre, 64);

    char json[256];
    snprintf(json, sizeof(json),
             "{\"nombre\": \"%s\"}", nombre);

    bson_t *update = bson_new_from_json((const uint8_t *) json, -1, &error);
    bson_t *query = BCON_NEW("codigo", BCON_UTF8(codigo));

    if (!mongoc_collection_update_one(collection, query, update, NULL, NULL, &error)) {
        printf("Error modificando Cliente: %s\n", error.message);
    }

    bson_destroy(update);
    bson_destroy(query);
    mongoc_collection_destroy(collection);
}

void borrarCliente() {
    bson_error_t error;
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_CLIENTE);

    char codigo[4];
    ingreso_cadena("Codigo cliente a borrar: ", codigo, 4);

    bson_t *query = BCON_NEW("codigo", BCON_UTF8(codigo));

    if (!mongoc_collection_delete_one(collection, query, NULL, NULL, &error)) {
        printf("Error borrando Cliente: %s\n", error.message);
    }

    bson_destroy(query);
    mongoc_collection_destroy(collection);
}

void borrarTodosClientes() {
    bson_error_t error;
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_CLIENTE);

    bson_t *query = bson_new();
    if (!mongoc_collection_delete_many(collection, query, NULL, NULL, &error)) {
        printf("Error borrando todos los clientes: %s\n", error.message);
    }

    bson_destroy(query);
    mongoc_collection_destroy(collection);
}