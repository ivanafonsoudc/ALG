

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
#define TAM_TABLA  19062
#define N 38197
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
    for (i = 0;fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
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
    return (pos_ini + num_intento);
}           
    
unsigned int resol_cuadratica(int pos_ini, int num_intento) {
    return (pos_ini + (num_intento * num_intento));
}                                             
            
unsigned int resol_doble(int pos_ini, int num_intento) {
    return (pos_ini + (num_intento * (10007 - (pos_ini % 10007))));
}

unsigned int ndispersion(char *clave, int tamTabla){
    if(strcmp(clave, "ANA")==0) return 7;
    if(strcmp(clave, "JOSE")==0) return 7;
    if(strcmp(clave, "OLGA")==0) return 7;
    return 6;
}

 

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, 
    int tam, int *colisiones, unsigned int (*dispersion)(char *, int), 
    unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) 
    {
    int x = dispersion(clave, tam);
    int pos = x;
    *colisiones = 0;
    while (diccionario[pos].ocupada && strcmp(diccionario[pos].clave, clave) != 0) {
        (*colisiones)++;
        pos = resol_colisiones(x, *colisiones) % tam;
        if (*colisiones > tam) {
            return -1;
        }
    }
    return pos;
}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario,
    int tam, unsigned int (*dispersion)(char *, int), 
    unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) 
    {       
    int colisiones = 0;
    pos pos = buscar_cerrada(clave, *diccionario, tam, 
    &colisiones, dispersion, resol_colisiones
    );
    if ((*diccionario)[pos].ocupada != 1 && pos != -1) {
        strcpy((*diccionario)[pos].clave, clave);
        strcpy((*diccionario)[pos].sinonimos, sinonimos);
        (*diccionario)[pos].ocupada = 1;
    }
    return colisiones;
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

void cotas(
    int n, double t, char ast,
    unsigned int (*dispersion)(char *, int),
    unsigned int (*resCol)(int posI, int nInt)
    ) {
    double x, y, z;

    x = t / pow(n, 0.8);
    z = t / (n * log(n));
    
    if (dispersion == dispersionA) {
        y = t / pow(n, 1.01);
    } else {
        if (resCol == resol_doble) {
            y = t / pow(n, 0.97);
        } else {    
            y = t / pow(n, 0.96);
        }
    }
    printf("%5c%8d %11.3f%18.8f%16.8f%16.8f\n",ast, n, t, x, y, z);
}


double medicionesAux(int n, item datos[], int colisiones, tabla_cerrada diccionario, 
    unsigned int (*resol_colisiones)(int pos_ini, int num_intento), 
    unsigned int (*dispersion)(char *, int)) 
    {
    double t, ta, tb;
    int i, j, rpos;
    int k = 1000;
    colisiones = 0;

    ta = microsegundos();
    for (i = 0; i < k; i++) {
        for (j = 0; j < n; j++) {
            rpos = rand() % TAM_TABLA;
            buscar_cerrada(datos[rpos].clave, diccionario, N, 
            &colisiones, dispersion, resol_colisiones);
        }
    }
    tb = microsegundos();
    t = (tb - ta);
    return (t / k);
}

void mediciones(int n, item datos[], tabla_cerrada diccionario,
    unsigned int (*resol_colisiones)(int pos_ini, int num_intento), 
    unsigned int (*dispersion)(char *, int))
    {
    double ta, tb, t;
    char ast = ' ';
    int i;
    int rpos, colisiones; 

    ta = microsegundos();
    for (i = 0; i < n; i++) {
        rpos = rand() % TAM_TABLA;
        buscar_cerrada(
            datos[rpos].clave, diccionario, N,
            &colisiones, dispersion, resol_colisiones
        );
    }
    tb = microsegundos();
    t = (tb - ta);

    if (t < 500) {
        ast = '*';
        t = medicionesAux(
                n, datos, colisiones,
                diccionario, resol_colisiones, dispersion
            );
    }
    cotas(n, t, ast, dispersion, resol_colisiones);

}

void tablasMediciones(unsigned int (*dispersion)(char *, int), 
    unsigned int (*resol_colisiones)(int posIni, int numIntento)) {
    int k,i;
    item datos[TAM_TABLA];
    int elementos;
    int colisiones = 0;

    tabla_cerrada diccionario = malloc(N * sizeof(entrada));
    inicializar_cerrada(&diccionario, N);

    elementos = leer_sinonimos(datos);
    if(elementos < TAM_TABLA){
        return;
    }

    for (i = 0; i < elementos; i++) {
        colisiones += insertar_cerrada(
                    datos[i].clave, datos[i].sinonimos,
                    &diccionario, N, dispersion, resol_colisiones
                );
    }
    printf("Insertando %d elementos... Numero total de colisiones: %d \n", elementos, colisiones);
    printf("Buscando n elementos...\n");
    printf("%5s\t%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    for(k = 125; k <= 16000; k *= 2 ){
        mediciones(k, datos, diccionario, resol_colisiones, dispersion);
    }
    printf("\n");
    free(diccionario);
}

void tablas(){
    printf("\nLineal con dispersion A:\n");
    tablasMediciones(dispersionA, resol_lineal);
    printf("\nCuadratica con dispersion A:\n");
    tablasMediciones(dispersionA, resol_cuadratica);
    printf("\nDoble con dispersion A:\n");
    tablasMediciones(dispersionA, resol_doble);
    printf("\nLineal con dispersion B:\n");
    tablasMediciones(dispersionB, resol_lineal);
    printf("\nCuadratica con dispersion B:\n");
    tablasMediciones(dispersionB, resol_cuadratica);
    printf("\nDoble con dispersion B:\n");
    tablasMediciones(dispersionB, resol_doble);
    printf("\nLineal con dispersion C:\n");
    tablasMediciones(dispersionC, resol_lineal);
    printf("\nCuadratica con dispersion C:\n");
    tablasMediciones(dispersionC, resol_cuadratica);
    printf("\nDoble con dispersion C:\n");
    tablasMediciones(dispersionC, resol_doble);
    printf("\nLineal con dispersion D:\n");
    tablasMediciones(dispersionD, resol_lineal);
    printf("\nCuadratica con dispersion D:\n");
    tablasMediciones(dispersionD, resol_cuadratica);
    printf("\nDoble con dispersion D:\n");
    tablasMediciones(dispersionD, resol_doble);
}


/*void test_tabla_cerrada( unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
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
}*/

int main() {
    printf("Prueba con resolucion lineal:\n");
    //test_tabla_cerrada(resol_lineal);

    printf("\nPrueba con resolucion cuadratica:\n");
    //test_tabla_cerrada(resol_cuadratica);

    tablas();

    
    

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






