#include <stdio.h>                                   
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))            
#define LONGITUD_CLAVE 30                        
#define LONGITUD_SINONIMOS 300
#define TAM_TABLA  10007
#define TAM_TABLA2  11
#define TAM_TABLA3  100

double microsegundos() {                  
    struct timeval t;                         
    if (gettimeofday(&t, NULL) < 0 ){          
        return 0.0;}                               
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

typedef struct entrada {
    int ocupada;
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} entrada;      

typedef struct {
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} item;
    
typedef int pos;
typedef entrada *tabla_cerrada;


typedef struct nodo{
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
    struct nodo *siguiente; 
}nodo;

typedef struct{
    nodo **tabla; //array de punteros a nodo
    int tam;
}tabla_abierta;

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    int i;
    *diccionario = malloc(sizeof(entrada) * tam);
    if (*diccionario == NULL) {
        printf("Error al reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
        strcpy((*diccionario)[i].clave, "");
        strcpy((*diccionario)[i].sinonimos, "");
    }
}

int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir ’sinonimos.txt’\n");
        return(EXIT_FAILURE);
    }
    for (i = 0; i< TAM_TABLA && fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
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

unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));   
    unsigned int valor = clave[0];                
    for (i = 1; i < n; i++)
        valor += clave[i];
    return valor % tamTabla;   
}                  

unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));                         
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor = (valor<<5) + clave[i]; // el desplazamiento de 5 bits equivale a 
    return valor % tamTabla; // multipicar por 32 
}   

unsigned int dispersionC(char *clave, int tamTabla){
    int i;
    unsigned int valor = 0;
    for(i = 0; i < strlen(clave) - 1; i++){
        valor = (valor + (clave[i] *  (i+1)));
    }
    return valor % tamTabla;
}

unsigned int dispersionD(char *clave, int tamTabla){
    unsigned int valor = 0;
    for (int i = 0; clave[i] != '\0'; i++) {
        valor = valor * 31 + clave[i];
    }
    double k = 0.6180339887;
    double hash = valor * k;
    return (unsigned int)(tamTabla * (hash - (unsigned int)hash));
}



unsigned int resol_lineal(int pos_ini, int num_intento) {
    return (pos_ini + num_intento) % TAM_TABLA;
}           
    
unsigned int resol_cuadratica(int pos_ini, int num_intento) {
    return (pos_ini + num_intento * num_intento) % TAM_TABLA;
}                                             
            
unsigned int resol_doble(int pos_ini, int num_intento) {
    return (pos_ini + num_intento * (10007 - pos_ini % 10007))% TAM_TABLA;
}

unsigned int ndispersion(char *clave, int tamTabla){
    if(strcmp(clave, "ANA")==0) return 7;
    if(strcmp(clave, "JOSE")==0) return 7;
    if(strcmp(clave, "OLGA")==0) return 7;
    return 6;
}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {       
    int colisiones = 0;
    unsigned int pos_ini = dispersion(clave, tam);
    unsigned int pos = pos_ini;
    int intentos = 0;
    while ((*diccionario)[pos%tam].ocupada) {
        pos = resol_colisiones(pos_ini, ++intentos) %tam;
        (colisiones)++;
    }                                             
    (*diccionario)[pos%tam].ocupada = 1;
    strcpy((*diccionario)[pos%tam].clave, clave);
    strcpy((*diccionario)[pos%tam].sinonimos, sinonimos);
    return colisiones;
}  

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    int intentos = 0;
    unsigned int pos_ini = dispersion(clave, tam);
    unsigned int pos = pos_ini;
    *colisiones = 0;
    while (diccionario[pos % tam].ocupada && strcmp(diccionario[pos%tam].clave, clave) != 0) {
        pos = resol_colisiones(pos_ini, ++intentos)%tam;
        (*colisiones)++;
    }
    if(diccionario[pos%tam].ocupada && strcmp(diccionario[pos%tam].clave, clave) == 0){
        return pos%tam;
    }
    return -1;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d- (%s)\n", i, diccionario[i].clave);
        }else{
            printf("%d-\n", i);
        }
    }
    printf("\n");
}

void generar_orden_aleatorio(int indices[], int num_datos) {
    int i;
    srand(time(NULL));
    for (i = 0; i < num_datos; i++) {
        indices[i] = rand() % num_datos;
    }
    
}

//TABLA DE DISPERSION CERRADA
unsigned int hash(char *clave, int tam){
    unsigned int valor = 0;
    for (int i = 0; clave[i] != '\0'; i++){
        valor = (valor * 31 + clave[i]) % tam;
    }
    return valor;
}

void inicializar_abierta(tabla_abierta *t, int tam ){
    t -> tabla = (nodo **) malloc ( sizeof (nodo * ) * tam);
    t -> tam = tam;
    for(int i = 0; i < tam; i++){
        t -> tabla[i] = NULL;
    }
}

void insertar_abierta(tabla_abierta *t,char *clave, char *sinonimos){
    unsigned int pos = hash(clave, t -> tam);
    nodo *nuevo = (nodo *) malloc(sizeof(nodo));
    strcpy(nuevo -> clave, clave);
    strcpy(nuevo -> sinonimos, sinonimos);
    nuevo -> siguiente = t -> tabla[pos];
    t -> tabla[pos] = nuevo;
}

nodo *buscar_abierta(tabla_abierta *t, char *clave){
    unsigned int pos = hash (clave, t->tam);
    nodo *aux = t -> tabla[pos];
    while(aux != NULL){
        if(strcmp(aux-> clave, clave)== 0){
            return aux;
        }
        aux = aux -> siguiente;
    }
    return NULL;
}

void mostrar_abierta ( tabla_abierta *t){
    for(int i = 0; i < t->tam; i++){
        printf("%d: ", i);
        nodo *aux = t -> tabla[i];
        while(aux != NULL){
            printf("(%s) ", aux -> clave);
            aux = aux -> siguiente;
        }
        printf("\n");
    }
}

void medir_tiempos(tabla_abierta *t, char *claves[], int num_claves){
    clock_t inicio,fin;
    inicio = clock();
    for(int i = 0; i < num_claves; i++){
        buscar_abierta(t, claves[i]);
    }
    fin = clock();
    printf("Tempo: %.6f segundos\n", (double)(fin - inicio) / CLOCKS_PER_SEC); 

}

void liberar_abierta(tabla_abierta *t){
    for(int i = 0; i < t -> tam; i++){
        nodo *aux = t -> tabla[i];
        while(aux != NULL){
            nodo *aux2 = aux;
            aux = aux -> siguiente;
            free(aux2);
        }
    }
    free(t -> tabla);   
}


void mediciones(unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int, int), item datos[], int num_datos, char *nombre, char *resol, double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    int i, j, l, colisiones;
    int k = 1000; // Número de repeticiones
    double t1, t2,t3,t4, ta, tb, t, x, y, z;
    int colisiones_total = 0;
    int indices[num_datos];
    tabla_cerrada diccionario;
    inicializar_cerrada(&diccionario, TAM_TABLA);
    int limite;

    // Insertar elementos en la tabla
    for (l = 0; l < num_datos; l++) {
        colisiones_total += insertar_cerrada(datos[l].clave, datos[l].sinonimos, &diccionario, TAM_TABLA, dispersion, resol_colisiones);
    }

    printf("* %s: %s\nInsertando %d elementos... Numero total de colisiones: %d\n", nombre, resol, num_datos, colisiones_total);
    printf("*Buscando n elementos...\n");
    printf("%5s\t%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    generar_orden_aleatorio(indices, num_datos);

    for (i = 125; i <= 16000; i *= 2) {
        limite = MIN(i, num_datos);
        t1 = microsegundos();
        for (j = 0; j < limite; j++) {
            colisiones = 0;
            buscar_cerrada(datos[indices[j]].clave, diccionario, TAM_TABLA, &colisiones, dispersion, resol_colisiones);
        }
        t2 = microsegundos();
        t = (t2 - t1);

        if (t < 500) {
            // Cálculo de tiempo con operaciones reales
            ta = microsegundos();
            for (int h = 0; h < k; h++) {
                for (int p = 0; p < limite; p++) {
                    colisiones = 0;
                    buscar_cerrada(datos[indices[p]].clave, diccionario, TAM_TABLA, &colisiones, dispersion, resol_colisiones);
                }
            }
            tb = microsegundos();
            t3 = (tb - ta) / k;

            // Cálculo de sobrecarga
            ta = microsegundos();
            for (int h = 0; h < k; h++) {
                for (int p = 0; p < limite; p++) {
                    // Bucle vacío con operaciones básicas
                    colisiones = 0;
                }
            }
            tb = microsegundos();
            t4 = (tb - ta) / k;

            // Calcular tiempo neto
            t = t3 - t4;
            if (t < 0) t = 0; // Evita tiempos negativos
            printf("*");
        } else {
            printf(" ");

        }

        y = t / f1(i);
        x = t / f2(i);
        z = t / f3(i);
        printf("%5d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, y, x, z);
    }

    printf("\n");
    free(diccionario);
}

double f1_A_lin(int n) { return pow(n, 0.8); }
double f2_A_lin(int n) { return pow(n, 1); }
double f3_A_lin(int n) { return (n * log(n)); }

double f1_A_cuad(int n) { return pow(n, 0.8); }
double f2_A_cuad(int n) { return pow(n, 1); }
double f3_A_cuad(int n) { return (n * log(n)); }

double f1_A_doble(int n) { return pow(n, 0.8); }
double f2_A_doble(int n) { return pow(n, 1); }
double f3_A_doble(int n) { return (n * log(n)); }

double f1_B_lin(int n) { return pow(n, 0.8); }
double f2_B_lin(int n) { return pow(n, 1); }
double f3_B_lin(int n) { return (n * log(n)); }

double f1_B_cuad(int n) { return pow(n, 0.8); }
double f2_B_cuad(int n) { return pow(n, 1); }
double f3_B_cuad(int n) { return (n * log(n)); }


double f1_B_doble(int n) { return pow(n, 0.8); }
double f2_B_doble(int n) { return pow(n, 1); }
double f3_B_doble(int n) { return (n * log(n)); }

double f1_C_lin(int n) { return pow(n, 0.8); }
double f2_C_lin(int n) { return pow(n, 1); }
double f3_C_lin(int n) { return (n * log(n)); }

double f1_C_cuad(int n) { return pow(n, 0.8); }
double f2_C_cuad(int n) { return pow(n, 1); }
double f3_C_cuad(int n) { return (n * log(n)); }

double f1_C_doble(int n) { return pow(n, 0.8); }
double f2_C_doble(int n) { return pow(n, 1); }
double f3_C_doble(int n) { return (n * log(n)); }

double f1_D_lin(int n) { return pow(n, 0.8); }
double f2_D_lin(int n) { return pow(n, 1); }
double f3_D_lin(int n) { return (n * log(n)); }

double f1_D_cuad(int n) { return pow(n, 0.8); }
double f2_D_cuad(int n) { return pow(n, 1); }
double f3_D_cuad(int n) { return (n * log(n)); }


double f1_D_doble(int n) { return pow(n, 0.8); }
double f2_D_doble(int n) { return pow(n, 1); }
double f3_D_doble(int n) { return (n * log(n)); }



void test_tabla_cerrada( unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    int colisiones_total = 0;
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    int colisiones;
    tabla_cerrada diccionario;
    inicializar_cerrada(&diccionario, TAM_TABLA2);
    colisiones_total += insertar_cerrada("ANA", "", &diccionario, TAM_TABLA2, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("LUIS", "", &diccionario, TAM_TABLA2, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("JOSE", "", &diccionario, TAM_TABLA2, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("OLGA", "", &diccionario, TAM_TABLA2, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("ROSA", "", &diccionario, TAM_TABLA2, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("IVAN", "", &diccionario, TAM_TABLA2, ndispersion, resol_colisiones);
    printf("Numero total de colisiones al insertar los elementos: %d\n", colisiones_total);
    mostrar_cerrada(diccionario, TAM_TABLA2);
    for (int i = 0; i < 7; i++) {
        int pos = buscar_cerrada(claves[i], diccionario, TAM_TABLA2, &colisiones, ndispersion, resol_colisiones);
        if (pos != -1) {
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", claves[i], diccionario[pos].clave, colisiones);
        } else {
            printf("No encuentro: %s, colisiones: %d\n", claves[i], colisiones);
        }
    }
    free(diccionario);
}

int main() {
    srand(time(NULL));
    printf("Prueba con resolucion lineal:\n");
    test_tabla_cerrada(resol_lineal);

    printf("\nPrueba con resolucion cuadratica:\n");
    test_tabla_cerrada(resol_cuadratica);

    

    item *datos= malloc(sizeof(item) * TAM_TABLA);
    if(datos == NULL){
        printf("Error al reservar memoria\n");
        return EXIT_FAILURE;
    }
    int num_datos = leer_sinonimos(datos);
    if(num_datos == EXIT_FAILURE){
        free(datos);
        return EXIT_FAILURE;
    }

    mediciones(dispersionA, resol_lineal, datos, num_datos, "dispersionA","Lineal", f1_A_lin, f2_A_lin, f3_A_lin);
    mediciones(dispersionA, resol_cuadratica, datos, num_datos, "dispersionA","Cuadratica", f1_A_cuad, f2_A_cuad, f3_A_cuad);
    mediciones(dispersionA, resol_doble, datos, num_datos, "dispersionA","Doble", f1_A_doble, f2_A_doble, f3_A_doble);

    mediciones(dispersionB, resol_lineal, datos, num_datos, "dispersionB","Lineal", f1_B_lin, f2_B_lin, f3_B_lin);
    mediciones(dispersionB, resol_cuadratica, datos, num_datos, "dispersionB","Cuadratica", f1_B_cuad, f2_B_cuad, f3_B_cuad);
    mediciones(dispersionB, resol_doble, datos, num_datos, "dispersionB","Doble", f1_B_doble, f2_B_doble, f3_B_doble);

    mediciones(dispersionC, resol_lineal, datos, num_datos, "dispersionC","Lineal", f1_C_lin, f2_C_lin, f3_C_lin);
    mediciones(dispersionC, resol_cuadratica, datos, num_datos, "dispersionC","Cuadratica", f1_C_cuad, f2_C_cuad, f3_C_cuad);
    mediciones(dispersionC, resol_doble, datos, num_datos, "dispersionC","Doble", f1_C_doble, f2_C_doble, f3_C_doble);

    mediciones(dispersionD, resol_lineal, datos, num_datos, "dispersionD","Lineal", f1_D_lin, f2_D_lin, f3_D_lin);
    mediciones(dispersionD, resol_cuadratica, datos, num_datos, "dispersionD","Cuadratica", f1_D_cuad, f2_D_cuad, f3_D_cuad);
    mediciones(dispersionD, resol_doble, datos, num_datos, "dispersionD","Doble", f1_D_doble, f2_D_doble, f3_D_doble);

    free(datos);

    //TABLA DE DISPERSION ABIERTA
    tabla_abierta diccionario;
    inicializar_abierta(&diccionario, TAM_TABLA3);
    insertar_abierta(&diccionario, "perro", "can, cachhoro, chucho");
    insertar_abierta(&diccionario, "gato", "felino, michi, minino");
    insertar_abierta(&diccionario, "pajaro", "ave, volador, plumifero");

    printf("Contenido de la tabla: \n");
    mostrar_abierta(&diccionario);

    char *claves[] = {"perro", "gato", "pajaro", "pez"};
    medir_tiempos(&diccionario, claves, 4);

    liberar_abierta(&diccionario);  

    return 0;
}






