#include <stdio.h>
#include <string.h>
#include <stdio_ext.h>
#include "Utilidades.h"

void ingreso_cadena(const char *etiqueta, char *cadena, int max) {
    printf("%s", etiqueta);
    fgets(cadena, max, stdin);
    cadena[strlen(cadena)-1] = '\0';
}

void ingreso_entero(const char *etiqueta, int *entero) {
    printf("%s", etiqueta);
    scanf("%d", entero);
    __fpurge(stdin);
}

void ingreso_double(const char *etiqueta, double *numero) {
    printf("%s", etiqueta);
    scanf("%lf", numero);
    __fpurge(stdin);
}