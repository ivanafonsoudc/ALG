#include <stdio.h>                                   
#include <stdlib.h>
#include <string.h>
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))            
#define LONGITUD_CLAVE 30                        
#define LONGITUD_SINONIMOS 300

    
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

typedef struct entrada {
    int ocupada;
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} entrada;                                        
    
typedef int pos;
typedef entrada *tabla_cerrada;
tabla_cerrada d = malloc(38197 * sizeof(entrada));

void inicializar_cerrada(tabla_cerrada *diccionario, int tam);
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento));
int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento));
void mostrar_cerrada(tabla_cerrada diccionario, int tam);

void incializar_cerrada(tabla_cerrada *diccionario, int tam) { //Tamaño
inicial: N = 38197
    int N = 38197;                 
    int i;                                        
    for (i = 0; i < N; i++) {          
        i=0;
    }
}

unsigned int resol_colisiones_lineal(int pos_ini, int num_intento) { //Exploración linal                                         
    return num_intento;                       
}           
    
unsigned int resol_colisiones_cuadratica(int pos_ini, int num_intento) { //Exploración cuadrática
    return num_intento * num_intento;                    
}                                             
            
unsigned int resol_colisiones_doble(int pos_ini, int num_intento, unsigned int hash2) { //Exploración doble
    return num_intento * hash2;
}  

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    int index = dispersion(clave, tam);
    int contador = 0;                             
    int i;                             
    for (i = 0; i < tam; i++) {                   
        //Inserta la clave si la posición está vacía
        int pos=(index + resol_colisiones(index,i)) % tam; //resol_colisiones subsistuir para cada una(lineal,cuadratica y doble)
        if((*diccionario)[pos].ocupada == 0) {
            strncpy((*diccionario)[pos].clave, clave, LONGITUD_CLAVE -1);                                             
            strncpy((*diccionario)[pos].sinonimos, sinonimos, LONGITUD_SINONIMOS);                                    
            (*diccionario)[pos].ocupada = 1;
            return contador;
        } else {                            
            contador++;     
        }                                   
    }                       
    perror("Error de inserción");
}                      
    
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento)){}

void mostrar_cerrada(tabla_cerrada diccionario, int tam){}