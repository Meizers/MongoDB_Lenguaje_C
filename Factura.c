#include <stdio.h>
#include <string.h>
#include "factura.h"
#include "db.h"
#include "utils.h"

#define COLLECTION_FACTURA "factura"

int cargarFacturas() {
    bson_error_t error;
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_FACTURA);

    factura f;
    ingreso_cadena("Ingrese codigo factura (0 para salir): ", f.codigo_factura, 10);

    while (f.codigo_factura[0] != '0') {
        ingreso_cadena("Ingrese fecha (YYYY-MM-DD): ", f.fecha, 16);
        ingreso_double("Ingrese monto: ", &f.monto);
        ingreso_cadena("Ingrese codigo cliente (referencia): ", f.codigo_cliente, 4);

        char json[512];
        snprintf(json, sizeof(json),
                 "{\"codigo_factura\": \"%s\", \"fecha\": \"%s\", \"monto\": %.2f, \"codigo_cliente\": \"%s\"}",
                 f.codigo_factura, f.fecha, f.monto, f.codigo_cliente);

        bson_t *bson = bson_new_from_json((const uint8_t *) json, -1, &error);
        if (bson == NULL) {
            printf("Error creando BSON Factura: %s\n", error.message);
            return -1;
        }

        if (!mongoc_collection_insert_one(collection, bson, NULL, NULL, &error)) {
            printf("Error insertando Factura %s: %s\n", f.codigo_factura, error.message);
        }

        bson_destroy(bson);

        ingreso_cadena("Ingrese codigo factura (0 para salir): ", f.codigo_factura, 10);
    }

    mongoc_collection_destroy(collection);
    return 0;
}

void mostrarFacturas() {
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_FACTURA);
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

void modificarFactura() {
    bson_error_t error;
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_FACTURA);

    char codigo[10], fecha[16];
    double monto;
    ingreso_cadena("Codigo factura a modificar: ", codigo, 10);
    ingreso_cadena("Nueva fecha: ", fecha, 16);
    ingreso_double("Nuevo monto: ", &monto);

    char json[256];
    snprintf(json, sizeof(json),
             "{\"fecha\": \"%s\", \"monto\": %.2f}", fecha, monto);

    bson_t *update = bson_new_from_json((const uint8_t *) json, -1, &error);
    bson_t *query = BCON_NEW("codigo_factura", BCON_UTF8(codigo));

    if (!mongoc_collection_update_one(collection, query, update, NULL, NULL, &error)) {
        printf("Error modificando Factura: %s\n", error.message);
    }

    bson_destroy(update);
    bson_destroy(query);
    mongoc_collection_destroy(collection);
}

void borrarFactura() {
    bson_error_t error;
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_FACTURA);

    char codigo[10];
    ingreso_cadena("Codigo factura a borrar: ", codigo, 10);

    bson_t *query = BCON_NEW("codigo_factura", BCON_UTF8(codigo));

    if (!mongoc_collection_delete_one(collection, query, NULL, NULL, &error)) {
        printf("Error borrando Factura: %s\n", error.message);
    }

    bson_destroy(query);
    mongoc_collection_destroy(collection);
}

void borrarTodasFacturas() {
    bson_error_t error;
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_FACTURA);

    bson_t *query = bson_new();
    if (!mongoc_collection_delete_many(collection, query, NULL, NULL, &error)) {
        printf("Error borrando todas las facturas: %s\n", error.message);
    }

    bson_destroy(query);
    mongoc_collection_destroy(collection);
}

void mostrarFacturasDeCliente() {
    mongoc_collection_t *collection = mongoc_client_get_collection(conn, DB, COLLECTION_FACTURA);

    char codigo_cliente[4];
    ingreso_cadena("Codigo cliente para ver sus facturas: ", codigo_cliente, 4);

    bson_t *query = BCON_NEW("codigo_cliente", BCON_UTF8(codigo_cliente));
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    const bson_t *doc;
    while (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("Factura: %s\n", str);
        bson_free(str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
}
