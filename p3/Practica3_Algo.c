#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>


#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define TAM_TABLA 19062
#define N 38197

typedef struct {
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} item;
typedef struct entrada_ {
    int ocupada;
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} entrada;
typedef int pos;
typedef entrada *tabla_cerrada;

double microsegundos(void) {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializar_semilla(void) {
    srand(time(NULL));
}   

unsigned int lineal(int posI, int nInt) {
    return (posI + nInt);
}
unsigned int cuadratica(int posI, int nInt) {
    return (posI + (nInt * nInt));
}
unsigned int doble(int posI, int nInt) {
    return (posI + (nInt * (10007 - (posI % 10007))));
}
unsigned int ndoble(int posI, int nInt) {
    return (posI + (nInt * (5 - (posI % 5))));
}

unsigned int ndispersion(char *clave, int tamTabla) {
    if (strcmp(clave, "ANA")==0) return 7;
    if (strcmp(clave, "JOSE")==0) return 7;
    if (strcmp(clave, "OLGA")==0) return 7;
    return 6;
}
unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++) {
        valor += clave[i];
    }
    return valor % tamTabla;
}
unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++) {
        valor = (valor<<5) + clave[i]; /* el desplazamiento de 5 bits equivale a */
    }
    return valor % tamTabla; /* multipicar por 32 */
}

int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir 'sinonimos.txt'\n");
        return(EXIT_FAILURE);
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador\n");
            return(EXIT_FAILURE);
        }
        for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j] = c;
        }
        datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return(EXIT_FAILURE);
    }
    return(i);
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
        strcpy((*diccionario)[i].clave, "");
        strcpy((*diccionario)[i].sinonimos, "");
    }
}

pos buscar_cerrada(
    char *clave, tabla_cerrada diccionario, int tam,
    int *colisiones, unsigned int (*dispersion)(char *, int),
    unsigned int (*resCol)(int posI, int nInt)
    ) {
    pos x = dispersion(clave, tam);
    pos pos = x;
    *colisiones = 0;

    while (
        diccionario[pos].ocupada && 
        strcmp(diccionario[pos].clave, clave) != 0
        ) {
        *colisiones += 1;
        pos = resCol(x, *colisiones) % tam;
        if (*colisiones > tam) {
            return -1;
        }
    }
    return pos;
}

int insertar_cerrada(
    char *clave, char *sinonimos,
    tabla_cerrada *diccionario, int tam,
    unsigned int (*dispersion)(char *, int),
    unsigned int (*resCol)(int posI, int nInt)
    ) {
    int cols = 0;
    pos pos = buscar_cerrada(
        clave, *diccionario, tam,
        &cols, dispersion, resCol
    );

    if ((*diccionario)[pos].ocupada != 1 && pos != -1) {
        strcpy((*diccionario)[pos].clave, clave);
        strcpy((*diccionario)[pos].sinonimos, sinonimos);
        (*diccionario)[pos].ocupada = 1;
    }

    return cols;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d - (%s %s)\n",
                    i, diccionario[i].clave, diccionario[i].sinonimos);
        } else {
            printf("%d -\n", i);
        }
    }
}

void cotas(
    int n, double t, char pr,
    unsigned int (*dispersion)(char *, int),
    unsigned int (*resCol)(int posI, int nInt)
    ) {
    double x, y, z;

    x = t / pow(n, 0.8);
    z = t / (n * log(n));
    
    if (dispersion == dispersionA) {
        y = t / pow(n, 1.01);
    } else {
        if (resCol == doble) {
            y = t / pow(n, 0.97);
        } else {    
            y = t / pow(n, 0.96);
        }
    }
    printf("%8d %11.3f%18.8f%16.8f%16.8f%5c\n", n, t, x, y, z, pr);
}

double contarT(
    int n, item datos[], int colisiones,tabla_cerrada diccionario,
    unsigned int (*resCol)(int posI, int nInt),
    unsigned int (*dispersion)(char *, int)
    ) {
    double t, ta, tb;
    int i, j, rpos;
    int k = 1000;

    colisiones = 0;

    ta = microsegundos();
    for (i = 0; i < k; i++) {
        for (j = 0; j < n; j++) {
            rpos = rand() % TAM_TABLA;
            buscar_cerrada(
                datos[rpos].clave, diccionario, N,
                &colisiones, dispersion, resCol
            );
        }
    }
    tb = microsegundos();

    t = (tb - ta);
    return (t / k);
}

void contar(
    int n, item datos[], tabla_cerrada diccionario,
    unsigned int (*resCol)(int posI, int nInt),
    unsigned int (*dispersion)(char *, int)
    ) {
    double ta, tb, t;
    char pr = ' ';
    int i;
    int rpos, colisiones; 

    ta = microsegundos();
    for (i = 0; i < n; i++) {
        rpos = rand() % TAM_TABLA;
        buscar_cerrada(
            datos[rpos].clave, diccionario, N,
            &colisiones, dispersion, resCol
        );
    }
    tb = microsegundos();
    t = (tb - ta);

    if (t < 500) {
        pr = '*';
        t = contarT(
                n, datos, colisiones,
                diccionario, resCol, dispersion
            );
    }
    cotas(n, t, pr, dispersion, resCol);
}

void imprimirTest(unsigned int (*resCol)(int pos, int n)) {
    char *palabras[7] = {"ANA", "LUIS", "JOSE", "OLGA",
                        "ROSA", "IVAN", "CARLOS"};
    int tam = 11;
    int i;
    pos pos;
    int colT = 0, cols = 0;

    tabla_cerrada diccionario;
    diccionario = malloc(tam * sizeof(entrada));
    inicializar_cerrada(&diccionario, tam);

    for (i = 0; i < 6; i++) {
        colT += insertar_cerrada(
                    palabras[i], "", &diccionario,
                    tam, ndispersion, resCol
                );
    }

    mostrar_cerrada(diccionario, tam);

    printf("Número total de colisiones al insertar: %d\n\n", colT);

    for (i = 0; i < 7; i++) {
        pos = buscar_cerrada(
                palabras[i], diccionario, tam,
                &cols, ndispersion, resCol
                );

        if (diccionario[pos].ocupada) {
            printf("Al buscar: %s, encuentro: %s, Colisiones: %d\n",
                    palabras[i], diccionario[pos].clave, cols);
        } else {
            printf("No se pudo encontrar: %s, Colisiones: %d\n",
                    palabras[i], cols);
        }
    }

    free(diccionario);
}

void imprimirTablas(
    unsigned int (*resCol)(int posI, int nInt), 
    unsigned int (*dispersion)(char *, int)
    ) {
    int k, i;
    item datos[TAM_TABLA];
    int palT;
    int colT = 0;

    tabla_cerrada diccionario = malloc(N * sizeof(entrada));
    inicializar_cerrada(&diccionario, N);

    palT = leer_sinonimos(datos);
    if (palT < TAM_TABLA) {
        return;
    }

    for (i = 0; i < palT; i++) {
        colT += insertar_cerrada(
                    datos[i].clave, datos[i].sinonimos,
                    &diccionario, N, dispersion, resCol
                );
    }
    
    printf("Insertando %d elementos... Numero total de colisiones: %d \n",
            palT, colT);
    printf("%6s %10s %21s %14s %17s\n",
           "n", "t", "C. subestimada", "C. ajustada", "C. sobreestimada");
    
    for (k = 125; k <= 16000; k *= 2) {
        contar(k, datos, diccionario, resCol, dispersion);
    }
    printf("-\n");

    free(diccionario);
}

void tablas(void) {
    printf("\nDispersion cerrada lineal con dispersion A\n");
    imprimirTablas(lineal, dispersionA);

    printf("\nDispersion cerrada cuadratica con dispersion A\n");
    imprimirTablas(cuadratica, dispersionA);

    printf("\nDispersion cerrada doble con dispersion A\n");
    imprimirTablas(doble, dispersionA);

    printf("\nDispersion cerrada lineal con dispersion B\n");
    imprimirTablas(lineal, dispersionB);

    printf("\nDispersion cerrada cuadratica con dispersion B\n");
    imprimirTablas(cuadratica, dispersionB);

    printf("\nDispersion cerrada doble con dispersion B\n");
    imprimirTablas(doble, dispersionB);
}

void tests(void) {
    printf("\nTest con resolución lineal\n");
    imprimirTest(lineal);

    printf("\nTest con resolución cuadrática\n");
    imprimirTest(cuadratica);

    printf("\nTest con resolución doble\n");
    imprimirTest(ndoble);

    printf("\n---\n");
}

int main(void) {
    inicializar_semilla();

    tests();
    tablas();
}
