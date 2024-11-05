#include <stdio.h>                                   
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))            
#define LONGITUD_CLAVE 30                        
#define LONGITUD_SINONIMOS 300
#define TAM_TABLA  38197 // Ajustado para factor de carga 0.5 con N = 38197

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

void inicializar_tabla( tabla_cerrada *diccionario, int tam){
    int i;
    *diccionario = (entrada*) malloc(sizeof(entrada) * tam);
    for(i = 0; i < tam; i++){
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

unsigned int dispersion_secundariaA(char *clave, int tamTabla) {
    return (10007 - dispersionA(clave,tamTabla) % 10007);
}

unsigned int dispersion_secundariaB(char *clave, int tamTabla) {
    return (10007 - dispersionB(clave,tamTabla) % 10007);
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    *diccionario = malloc(sizeof(entrada) * tam);
    for (int i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
    }
}

unsigned int resol_lineal(int pos_ini, int num_intento) {
    return num_intento;
}           
    
unsigned int resol_cuadratica(int pos_ini, int num_intento) {
    return num_intento * num_intento;
}                                             
            
unsigned int resol_doble(int pos_ini, int num_intento) {
    return num_intento;
}

unsigned int ndispersion(char *clave, int tamTabla){
    if(strcmp(clave, "ANA")==0) return 7;
    if(strcmp(clave, "JOSE")==0) return 7;
    if(strcmp(clave, "OLGA")==0) return 7;
    return 6;
}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento), unsigned int (*dispersion_secundaria)(char *, int)) {
    int colisiones = 0;
    int pos_ini = dispersion(clave, tam);
    int hash2 = dispersion_secundaria(clave,tam);
    int pos = pos_ini;
    
    while ((*diccionario)[pos].ocupada) {
        colisiones++;
        pos = (pos_ini + resol_colisiones(pos_ini, colisiones) * hash2) %tam ;
    }
    
    (*diccionario)[pos  % tam].ocupada = 1;
    strcpy((*diccionario)[pos % tam].clave, clave);
    strcpy((*diccionario)[pos % tam].sinonimos, sinonimos);

    return colisiones;
}                      
    
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento), unsigned int (*dispersion_secundaria)(char *, int)) {
    int pos_ini = dispersion(clave, tam);
    int hash2 = dispersion_secundaria(clave,tam);
    int pos = pos_ini;
    *colisiones = 0;
    while (diccionario[pos % tam].ocupada && strcmp(diccionario[pos].clave, clave) != 0) {
        (*colisiones)++;
        pos = (pos_ini + resol_colisiones(pos_ini, *colisiones) * hash2) %tam ;
    }
    if(diccionario[pos].ocupada && strcmp(diccionario[pos].clave, clave) == 0){
        return pos;
    }
    
    return -1;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    for (int i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d- (%s)\n", i, diccionario[i].clave);
        }else{
            printf("%d-\n", i);
        }
    }
    printf("\n");
}
void mediciones(unsigned int (*dispersion) (char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento),unsigned int (*dispersion_secundaria)(char *,int), item datos[], int num_datos, char *nombre, double (*f1)(int), double (*f2)(int), double (*f3)(int)) {   
    tabla_cerrada diccionario;
    inicializar_cerrada(&diccionario, TAM_TABLA);
    int i, j, h;
    int k = 1000;           
    double t1, t2, ta, tb, t, x, y, z;       
    int colisiones_total = 0;                     
    int colisiones;

    for (i = 0; i < num_datos; i++) { // Cambiado a num_datos
        colisiones_total += insertar_cerrada(datos[i].clave, datos[i].sinonimos, &diccionario, TAM_TABLA, dispersion, resol_colisiones,dispersion_secundaria);
    }                        
    printf("* %s\nInsertando %d elementos... Numero total de colisiones: %d\n", nombre, num_datos, colisiones_total);
    printf("Buscando n elementos...\n");
    printf("n\t\tt(n)\t\tt(n)/n^0.8\t\tt(n)/n\t\tt(n)/n*log(n)\n");
    for (i = 125; i <= 16000; i = i * 2) {
        t1 = microsegundos();
        for (j = 0; j < MIN(i, num_datos); j++) { // Cambiado a MIN(i, num_datos)
            colisiones = 0;
            buscar_cerrada(datos[j].clave, diccionario, TAM_TABLA, &colisiones, dispersion, resol_colisiones,dispersion_secundaria);
        }
        t2 = microsegundos();
        t = (t2 - t1);
        if (t < 500) {
            ta = microsegundos();
            for (h = 0; h <= k; h++) {
                for (j = 0; j < MIN(i, num_datos); j++) {
                    colisiones = 0;
                    buscar_cerrada(datos[j].clave, diccionario, TAM_TABLA, &colisiones, dispersion, resol_colisiones, dispersion_secundaria);
                }
            }
            tb = microsegundos();
            t = (tb - ta) / k;
        }
        y = t / f1(i);
        x = t / f2(i);
        z = t / f3(i);
        printf("%d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, y, x, z);
    }
    printf("\n");
    free(diccionario);
}

void probar_combinaciones(item datos[], int num_datos, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento), unsigned int (*dispersion_secundaria)(char *, int), char *dispersion_name, char *resol_name, double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    mediciones(dispersion, resol_colisiones, dispersion_secundaria, datos, num_datos, dispersion_name, f1, f2, f3);
}

double f1_B_cuad(int n) { return pow(n, 0.8); }
double f2_B_cuad(int n) { return pow(n, 1); }
double f3_B_cuad(int n) { return (n*log(n)); }

double f1_B_lin(int n) { return pow(n, 0.8); }
double f2_B_lin(int n) { return pow(n, 1); }
double f3_B_lin(int n) { return (n*log(n)); }

double f1_B_doble(int n) { return pow(n, 0.8); }
double f2_B_doble(int n) { return pow(n, 1); }
double f3_B_doble(int n) { return (n*log(n)); }

double f1_A_cuad(int n) { return pow(n, 0.8); }
double f2_A_cuad(int n) { return pow(n, 1); }
double f3_A_cuad(int n) { return (n*log(n)); }

double f1_A_lin(int n) { return pow(n, 0.8); }
double f2_A_lin(int n) { return pow(n, 1); }
double f3_A_lin(int n) { return (n*log(n)); }

double f1_A_doble(int n) { return pow(n, 0.8); }
double f2_A_doble(int n) { return pow(n, 1); }
double f3_A_doble(int n) { return (n*log(n)); }

/*void test_tabla_cerrada( unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    tabla_cerrada diccionario;
    inicializar_cerrada(&diccionario, TAM_TABLA);

    int colisiones_total = 0;
    colisiones_total += insertar_cerrada("ANA", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("LUIS", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("JOSE", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("OLGA", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("ROSA", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones);
    colisiones_total += insertar_cerrada("IVAN", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones);

    printf("Numero total de colisiones al insertar los elementos: %d\n", colisiones_total);
    mostrar_cerrada(diccionario, TAM_TABLA);

    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    int colisiones;
    for (int i = 0; i < 7; i++) {
        int pos = buscar_cerrada(claves[i], diccionario, TAM_TABLA, &colisiones, ndispersion, resol_colisiones);
        if (pos != -1) {
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", claves[i], diccionario[pos].clave, colisiones);
        } else {
            printf("No encuentro: %s, colisiones: %d\n", claves[i], colisiones);
        }
    }
    free(diccionario);
}*/



int main() {
    //printf("Prueba con resolucion lineal:\n");
    //test_tabla_cerrada(resol_lineal);

    //printf("\nPrueba con resolucion cuadratica:\n");
    //test_tabla_cerrada(resol_cuadratica);

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
    

    probar_combinaciones(datos,num_datos, dispersionA, resol_lineal, dispersion_secundariaA, "dispersionA", "resol_lineal", f1_A_lin, f2_A_lin, f3_A_lin);
    probar_combinaciones(datos,num_datos, dispersionA, resol_cuadratica, dispersion_secundariaA, "dispersionA", "resol_cuadratica", f1_A_cuad, f2_A_cuad, f3_A_cuad);
    probar_combinaciones(datos,num_datos, dispersionA, resol_doble, dispersion_secundariaA, "dispersionA", "resol_doble", f1_A_doble, f2_A_doble, f3_A_doble);

    probar_combinaciones(datos,num_datos, dispersionB, resol_lineal, dispersion_secundariaB, "dispersionB", "resol_lineal", f1_B_lin, f2_B_lin, f3_B_lin);
    probar_combinaciones(datos,num_datos, dispersionB, resol_cuadratica, dispersion_secundariaB, "dispersionB", "resol_cuadratica", f1_B_cuad, f2_B_cuad, f3_B_cuad);
    probar_combinaciones(datos,num_datos, dispersionB, resol_doble, dispersion_secundariaB, "dispersionB", "resol_doble", f1_B_doble, f2_B_doble, f3_B_doble);
    free(datos);
    return 0;
}


