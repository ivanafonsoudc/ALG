#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300

typedef struct {
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} item;

typedef struct entrada_ {
    int ocupada;
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} entrada;

typedef unsigned int pos;
typedef entrada *tabla_cerrada;
//AUXILIARES PARA MEDICION DE TIEMPOS
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializar_semilla() {
    srand(time(NULL));
}

//COTAS
void cota_lineal(int i, double tn);

//FUNCIONES DE DISPERSION
pos ndispersion(char *clave, int tamTabla);

pos dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    pos valor = (unsigned char) clave[0];
    for (i = 1; i < n; i++)
        valor += clave[i];
    return valor % tamTabla;
}

pos dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    pos valor = (unsigned char) clave[0];
    for (i = 1; i < n; i++)
        valor = (valor << 5) + clave[i]; /* el desplazamiento de 5 bits equivale a */
    return valor % tamTabla;             /* multipicar por 32 */
}

//OPERACIONES SOBRE TABLAS
void inicializar_cerrada(tabla_cerrada *diccionario, int tam);

pos buscar_cerrada(char *clave, tabla_cerrada *diccionario, int tam, int *colisiones,
                   pos (*dispersion)(char *, int), pos (*resol_colisiones)(pos pos_ini, int num_intento));

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam,
                     pos (*dispersion)(char *, int), pos (*resol_colisiones)(pos pos_ini, int num_intento));

void mostrar_cerrada(tabla_cerrada diccionario, int tam);

//FUNCIONES DE RESOLUCION DE COLISIONES
pos resol_colisiones_lineal(pos pos_ini, int num_intento);

pos resol_colisiones_cuadratica(pos pos_ini, int num_intento);

pos resol_colisiones_doble_mod5(pos pos_ini, int num_intento);

pos resol_colisiones_doble_mod10007(pos pos_ini, int num_intento);

void test(char* nombre_resol_colisiones,pos (*resol_colisiones)(pos pos_ini, int num_intento));
//AUXILIARES DE TABLA
void tabla(char *nombre_funcion_dispersion, pos (*funcion_dispersion)(char *clave, int tamTabla),
           char *nombre_resol_colisiones, pos (*resol_colisiones)(pos pos_ini, int num_intento), int tamTabla,
           void (*cota)(int i, double tn),
           int tam_inicio, int filas_tabla,
           int iteraciones_bucle);
void inicializar_secuencia(int *secuencia, int tamSecuencia);
void buscar_secuencia(item *datos, tabla_cerrada *diccionario, int tamTabla, int iteraciones, int *secuencia,
                      pos (*resol_colisiones)(pos pos_ini, int num_intento),
                      pos (*funcion_dispersion)(char *clave, int tamTabla),int* colisiones);
int insertar_sinonimos_en_tabla(item *datos, tabla_cerrada *diccionario,
                                pos (*funcion_dispersion)(char *clave, int tamTabla),
                                pos (*resol_colisiones)(pos pos_ini, int num_intento));
int leer_sinonimos(item datos[]);
#include <stdio.h>

void imprimir_10_primeros(int *vector, int tamVector) {
    // Verificar que el vector no sea NULL y tenga al menos 10 elementos
    if (vector == NULL || tamVector <= 0) {
        printf("Vector no válido o vacío.\n");
        return;
    }

    // Determinar cuántos elementos imprimir (máximo 10 o el tamaño real del vector)
    int cantidad = (tamVector < 10) ? tamVector : 10;

    printf("Primeros %d elementos del vector:\n", cantidad);
    for (int i = 0; i < cantidad; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    inicializar_semilla();
    if (argc > 1 && !strcmp(argv[1], "test")) {
        test("TABLA CERRADA LINEAL", resol_colisiones_lineal);
        test("TABLA CERRADA CUADRATICA", resol_colisiones_cuadratica);
        test("TABLA CERRADA DOBLE (MOD 5)", resol_colisiones_doble_mod5);
        return 0;
    }

    tabla("dispersion A", dispersionA,
    "Dispersion cerrada lineal", resol_colisiones_lineal,
              38197, cota_lineal, 125, 8, 10000);
    tabla("dispersion A", dispersionA,
    "Dispersion cerrada cuadratica", resol_colisiones_cuadratica,
              38197, cota_lineal, 125, 8, 10000);
    tabla("dispersion A", dispersionA,
    "Dispersion cerrada doble", resol_colisiones_doble_mod10007,
              38197, cota_lineal, 125, 8, 10000);

    tabla("dispersion B", dispersionB,
    "Dispersion cerrada lineal", resol_colisiones_lineal,
              38197, cota_lineal, 125, 8, 10000);
    tabla("dispersion B", dispersionB,
          "Dispersion cerrada cuadrática", resol_colisiones_cuadratica,
          38197, cota_lineal, 125, 8, 10000);
    tabla("dispersion B", dispersionB,
          "Dispersion cerrada doble", resol_colisiones_doble_mod10007,
          38197, cota_lineal, 125, 8, 10000);


    return 0;
}

pos ndispersion(char *clave, int tamTabla) {
    if (strcmp(clave, "ANA") == 0) return 7;
    if (strcmp(clave, "JOSE") == 0) return 7;
    if (strcmp(clave, "OLGA") == 0) return 7;
    return 6;
}

int insertar_sinonimos_en_tabla(item *datos, tabla_cerrada *diccionario,
                                pos (*funcion_dispersion)(char *clave, int tamTabla),
                                pos (*resol_colisiones)(pos pos_ini, int num_intento)) {
    int i, colisiones = 0;
    inicializar_cerrada(diccionario, 38197); //Inicializamos tabla
    printf("Insertando 19062 elementos... ");
    for (i = 0; i < 19062; i++) {
        colisiones += insertar_cerrada(datos[i].clave, datos[i].sinonimos,
                                       diccionario, 38197, funcion_dispersion, resol_colisiones);
    }
    return colisiones;
}

void inicializar_secuencia(int *secuencia, int tamSecuencia) {

    int i;
    for (i = 0; i < tamSecuencia; i++) {
        secuencia[i] = rand()%19062;
    }
}

void buscar_secuencia(item *datos, tabla_cerrada *diccionario, int tamTabla, int iteraciones, int *secuencia,
                      pos (*resol_colisiones)(pos pos_ini, int num_intento),
                      pos (*funcion_dispersion)(char *clave, int tamTabla), int* colisiones) {
    int i;
    for (i = 0; i < iteraciones; i++) {
        buscar_cerrada(datos[secuencia[i]].clave, diccionario, tamTabla, colisiones, funcion_dispersion,
                       resol_colisiones);
    }
}

void tabla(char *nombre_funcion_dispersion, pos (*funcion_dispersion)(char *clave, int tamTabla),
           char *nombre_resol_colisiones, pos (*resol_colisiones)(pos pos_ini, int num_intento), int tamTabla,
           void (*cota)(int i, double tn),
           int tam_inicio, int filas_tabla,
           int iteraciones_bucle) {
    //Algoritmo fib2
    int i, j, k,colisiones = 0;
    int *secuencia_aleatoria;
    double tiempo_entrada= 0, tiempo_salida=0, tiempo_doble=0;
    double tn;
    item datos[19062];
    leer_sinonimos(datos);
    tabla_cerrada d = malloc(tamTabla * sizeof(entrada)); //Asignamos memoriaww
    secuencia_aleatoria = malloc(tam_inicio * (int)pow(2, filas_tabla) * sizeof(int));
    printf("\n***%s con %s\n", nombre_resol_colisiones, nombre_funcion_dispersion);
    colisiones = insertar_sinonimos_en_tabla(datos, &d, funcion_dispersion, resol_colisiones);
    printf("Numero total de colisiones: %d\n", colisiones);

    printf("Midiendo tiempos menores a 500 con %d it.\n", iteraciones_bucle);
    cota(-1, -1);
    for (i = tam_inicio; i < tam_inicio * pow(2, filas_tabla); i = 2 * i) {

        inicializar_secuencia(secuencia_aleatoria, i);
        //imprimir_10_primeros(secuencia_aleatoria, i);
        tiempo_entrada = microsegundos();
        buscar_secuencia(datos, &d, tamTabla, i, secuencia_aleatoria, resol_colisiones, funcion_dispersion, &colisiones);
        tiempo_salida = microsegundos();
        tn = tiempo_salida - tiempo_entrada;
        if (tn < 500) {
            //Si el tiempo es menor de 500
            // Repetir la ejecución varias veces para promediar el tiempo
            printf("*");
            tiempo_entrada = microsegundos();
            for (j = 0; j < iteraciones_bucle; j++) {
                inicializar_secuencia(secuencia_aleatoria, i);
                buscar_secuencia(datos, &d, tamTabla, i, secuencia_aleatoria, resol_colisiones, funcion_dispersion,&colisiones);
            }
            tiempo_salida = microsegundos();
            tiempo_doble = (tiempo_salida - tiempo_entrada);
            tiempo_entrada = microsegundos();
            for (j = 0; j < iteraciones_bucle; j++) {
                inicializar_secuencia(secuencia_aleatoria, i);
            }
            tiempo_salida = microsegundos();
            tn = (tiempo_doble - (tiempo_salida - tiempo_entrada)) / iteraciones_bucle; // Promedio por repetición
        } else printf(" ");
        //Imprimir la fila de la tabla con los valores calculados

        cota(i, tn);


    }
    free(secuencia_aleatoria);
    free(d);
}

void test(char* nombre_resol_colisiones,pos (*resol_colisiones)(pos pos_ini, int num_intento)) {
    int colisiones = 0, i;
    pos posicion;
    printf("-----------------------------------------------------------\n");
    printf("***%s\n", nombre_resol_colisiones);
    char *nombres[] = {
        "ANA",
        "LUIS",
        "JOSE",
        "OLGA",
        "ROSA",
        "IVAN",
        "CARLOS"
    };
    tabla_cerrada test = malloc(11 * sizeof(entrada));
    inicializar_cerrada(&test, 11);
    for (i = 0; i < 6; i++) {
        colisiones += insertar_cerrada(nombres[i], "",
                                       &test, 11, ndispersion, resol_colisiones);
    }

    mostrar_cerrada(test, 11);
    printf("Numero total de colisiones al insertar los elementos: %d\n\n", colisiones);
    for (i = 0; i < 7; i++) {
        colisiones = 0;
        posicion = buscar_cerrada(nombres[i], &test, 11,
                                  &colisiones, ndispersion, resol_colisiones);
        if (test[posicion].ocupada)
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", nombres[i],
                   test[posicion].clave, colisiones);
        else
            printf("No encuentro: %s, colisiones: %d\n", nombres[i], colisiones);
    }
    printf("\n");
    free(test);
}

void cota_lineal(int i, double tn) {
    if (i == -1) {
        printf("         n            t(n)      t(n)/n^0.8          t(n)/n      t(n)/nlogn\n");
        printf("-------------------------------------------------------------"
               "--------------------------------------\n");
    } else
        printf("%9d %15.4f %15.5f %15.5f %15.6f\n", i, tn, tn / pow(i, 0.8),
            tn / pow(i, 1), tn / (i*log(i)));
}

pos resol_colisiones_lineal(pos pos_ini, int num_intento) {
    return num_intento;
}

pos resol_colisiones_cuadratica(pos pos_ini, int num_intento) {
    return num_intento * num_intento;
}

pos resol_colisiones_doble_mod10007(pos pos_ini, int num_intento) {
    return num_intento * (10007 - pos_ini % 10007);
}

pos resol_colisiones_doble_mod5(pos pos_ini, int num_intento) {
    return num_intento * (5 - pos_ini % 5);
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
    }
}

pos buscar_cerrada(char *clave, tabla_cerrada *diccionario, int tam, int *colisiones,
                   pos (*dispersion)(char *, int), pos (*resol_colisiones)(pos pos_ini, int num_intento)) {
    int i = 0;
    pos x = dispersion(clave, tam);
    pos PosActual = x;
    while ((*diccionario)[PosActual].ocupada && strcmp((*diccionario)[PosActual].clave, clave) != 0) {
        i++; //FIXME importa i++ ++i?
        PosActual = (x+resol_colisiones(x, i)) % tam;
    }
    *colisiones = i;
    return PosActual;
}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam,
                     pos (*dispersion)(char *, int), pos (*resol_colisiones)(pos pos_ini, int num_intento)) {
    int colisiones = 0;
    pos Pos = buscar_cerrada(clave, diccionario, tam, &colisiones, dispersion, resol_colisiones);
    if (!(*diccionario)[Pos].ocupada) {
        strcpy((*diccionario)[Pos].clave, clave);
        strcpy((*diccionario)[Pos].sinonimos, sinonimos);
        (*diccionario)[Pos].ocupada = 1;
    }
    return colisiones;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    printf("{\n");
    for (int i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d- (%s)\n", i, diccionario[i].clave);
        } else {
            printf("%d-\n", i);
        }
    }
    printf("}\n");
}

int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir ’sinonimos.txt’\n");
        return (EXIT_FAILURE);
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador\n");
            return (EXIT_FAILURE);
        }
        for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j] = c;
        }
        datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return (EXIT_FAILURE);
    }
    return (i);
}
