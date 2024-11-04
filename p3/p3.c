#include <stdio.h>                                   
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))            
#define LONGITUD_CLAVE 30                        
#define LONGITUD_SINONIMOS 300
#define TAM_TABLA 76394 // Ajustado para factor de carga 0.5 con N = 38197

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

int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir ’sinonimos.txt’\n");
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
        valor = (valor<<5) + clave[i]; /* el desplazamiento de 5 bits equivale a */
    return valor % tamTabla; /* multipicar por 32 */
}    

unsigned int dispersion_secundaria(char *clave, int tamTabla, unsigned int (*dispersion_primaria)(char *, int)) {
    unsigned int x = dispersion_primaria(clave, tamTabla);
    return (10007 - x % 10007);
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    *diccionario = malloc(sizeof(entrada) * tam);
    for (int i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
    }
}

unsigned int resol_colisiones_lineal(int pos_ini, int num_intento) {
    return num_intento;
}           
    
unsigned int resol_colisiones_cuadratica(int pos_ini, int num_intento) {
    return num_intento * num_intento;
}                                             
            
unsigned int resol_colisiones_doble(int pos_ini, int num_intento) {
    return num_intento * pos_ini;
}  

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    int index = dispersion(clave, tam);
    for(int i = 0; i < tam; i++){
        int pos = (index + resol_colisiones(index, i)) % tam;
        if(!(*diccionario)[pos].ocupada){
            (*diccionario)[pos].ocupada = 1;
            strncpy((*diccionario)[pos].clave, clave, LONGITUD_CLAVE);
            strncpy((*diccionario)[pos].sinonimos, sinonimos, LONGITUD_SINONIMOS);
            return pos;
        }
    }
    return -1; // Return -1 if insertion fails
}                      
    
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    int index = dispersion(clave, tam);
    for (int i = 0; i < tam; i++) {
        int pos = (index + resol_colisiones(index, i)) % tam;
        if (diccionario[pos].ocupada && strncmp(diccionario[pos].clave, clave, LONGITUD_CLAVE) == 0) {
            return pos;
        }
        (*colisiones)++;
    }
    return -1;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    for (int i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("Posición %d: Clave = %s, Sinónimos = %s\n", i, diccionario[i].clave, diccionario[i].sinonimos);
        }
    }
}

unsigned int ndispersion(char *clave, int tamTabla){
    if(strcmp(clave, "ANA")==0) return 7;
    if(strcmp(clave, "JOSE")==0) return 7;
    if(strcmp(clave, "OLGA")==0) return 7;
    return 6;
}

void leer_datos(char *filename, tabla_cerrada diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
    while (fscanf(file, "%s %s", clave, sinonimos) != EOF) {
        insertar_cerrada(clave, sinonimos, &diccionario, tam, dispersion, resol_colisiones);
    }
    fclose(file);
}   

void inicializar_tabla( tabla_cerrada diccionario, int tam){
    for(int i = 0; i < tam; i++){
        diccionario[i].ocupada = 0;
    }
}


void test_tabla_cerrada_lineal() {
    tabla_cerrada diccionario;
    int colisiones = 0;
    inicializar_cerrada(&diccionario, TAM_TABLA);
    
    insertar_cerrada("IVAN", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_lineal);
    insertar_cerrada("LUIS", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_lineal);
    insertar_cerrada("ANA", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_lineal);
    insertar_cerrada("JOSE", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_lineal);
    insertar_cerrada("OLGA", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_lineal);
    insertar_cerrada("ROSA", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_lineal);
    
    printf("***TABLA CERRADA LINEAL\n{\n");
    for (int i = 0; i < 11; i++) {
        if (diccionario[i].ocupada) {
            printf("%d- (%s %s)\n", i, diccionario[i].clave, diccionario[i].sinonimos);
        } else {
            printf("%d-\n", i);
        }
    }
    printf("}\n");
    
    printf("Numero total de colisiones al insertar los elementos: %d\n", colisiones);
    
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    for (int i = 0; i < 7; i++) {
        colisiones = 0;
        pos p = buscar_cerrada(claves[i], diccionario, TAM_TABLA, &colisiones, ndispersion, resol_colisiones_lineal);
        if (p != -1) {
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", claves[i], diccionario[p].clave, colisiones);
        } else {
            printf("No encuentro: %s, colisiones: %d\n", claves[i], colisiones);
        }
    }
}

void test_tabla_cerrada_cuadratica() {
    tabla_cerrada diccionario;
    int colisiones = 0;
    inicializar_cerrada(&diccionario, TAM_TABLA);
    
    insertar_cerrada("IVAN", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_cuadratica);
    insertar_cerrada("LUIS", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_cuadratica);
    insertar_cerrada("ANA", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_cuadratica);
    insertar_cerrada("JOSE", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_cuadratica);
    insertar_cerrada("OLGA", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_cuadratica);
    insertar_cerrada("ROSA", "", &diccionario, TAM_TABLA, ndispersion, resol_colisiones_cuadratica);
    
    printf("***TABLA CERRADA CUADRÁTICA\n{\n");
    for (int i = 0; i < 11; i++) {
        if (diccionario[i].ocupada) {
            printf("%d- (%s %s)\n", i, diccionario[i].clave, diccionario[i].sinonimos);
        } else {
            printf("%d-\n", i);
        }
    }
    printf("}\n");
    
    printf("Numero total de colisiones al insertar los elementos: %d\n", colisiones);
    
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    for (int i = 0; i < 7; i++) {
        colisiones = 0;
        pos p = buscar_cerrada(claves[i], diccionario, TAM_TABLA, &colisiones, ndispersion, resol_colisiones_cuadratica);
        if (p != -1) {
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", claves[i], diccionario[p].clave, colisiones);
        } else {
            printf("No encuentro: %s, colisiones: %d\n", claves[i], colisiones);
        }
    }
}

int main(){
    test_tabla_cerrada_lineal();
    test_tabla_cerrada_cuadratica();
    return 0;
}
