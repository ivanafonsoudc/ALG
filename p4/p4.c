#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define TAM 256000
struct monticulo {
    int ultimo;
    int vector[TAM];
};
typedef struct monticulo * pmonticulo;

void iniMonticulo(pmonticulo m);
void insertarMonticulo(pmonticulo m, int x);
void quitarMenor(pmonticulo m);
int consultarMenor(const pmonticulo m);
void crearMonticulo(pmonticulo m, int v [], int n);

void iniMonticulo(pmonticulo m) {
    m->ultimo = 0;
}   

void insertarMonticulo(pmonticulo m, int x) {
    int i = m->ultimo;
    m->vector[i] = x;
    while (i > 0 && m->vector[i] < m->vector[(i-1)/2]) {
        int aux = m->vector[i];
        m->vector[i] = m->vector[(i-1)/2];
        m->vector[(i-1)/2] = aux;
        i = (i-1)/2;
    }
    m->ultimo++;
}

void quitarMenor(pmonticulo m) {
    m->ultimo--;
    m->vector[0] = m->vector[m->ultimo];
    int i = 0;
    while (2*i+1 < m->ultimo) {
        int hijo = 2*i+1;
        if (hijo+1 < m->ultimo && m->vector[hijo+1] < m->vector[hijo]) {
            hijo++;
        }
        if (m->vector[i] <= m->vector[hijo]) {
            break;
        }
        int aux = m->vector[i];
        m->vector[i] = m->vector[hijo];
        m->vector[hijo] = aux;
        i = hijo;
    }
}

int consultarMenor(const pmonticulo m) {
    return m->vector[0];
}

void crearMonticulo(pmonticulo m, int v [], int n) {
    iniMonticulo(m);
    for (int i = 0; i < n; i++) {
        insertarMonticulo(m, v[i]);
    }
}


void mostrarMonticulo(pmonticulo m) {
    printf("Montículo: ");
    for (int i = 1; i <= m->ultimo; i++) {
        printf("%d ", m->vector[i]);
    }
    printf("\n");
}

void pruebasMonticulo() {               
    struct monticulo m;
    iniMonticulo(&m);                  
    
    // Prueba 1: Inicialización
    if (m.ultimo != 0) {               
        printf("Error: El montículo no se inicializó correctamente.\n");    
        return;               
    } else {  
printf("Prueba 1 superada: Montículo inicializado correctamente.\n");                                  
    }   
    
    // Prueba 2: Inserción de elementos
    insertarMonticulo(&m, 10);              
    insertarMonticulo(&m, 5);
    insertarMonticulo(&m, 20);
    insertarMonticulo(&m, 1);
printf("Después de insertar elementos:\n");
    mostrarMonticulo(&m);              
        
    if (consultarMenor(&m) != 1) {     
        printf("Error: La inserción no mantiene la propiedad del montículo.\n");                                    
        return;          
    } else {
        printf("Prueba 2 superada: Inserción y consulta del menor correctas.\n");                        
    }   
// Prueba 3: Eliminación del menor 
    quitarMenor(&m);                                                   
    printf("Después de quitar el menor:\n");
    mostrarMonticulo(&m);
    
    if (consultarMenor(&m) != 5) { 
printf("Error: La eliminación del menor no mantiene la propiedad del montículo.\n");                   
        return;                                               
    } else {                           
        printf("Prueba 3 superada: Eliminación del menor correcta.\n");
    } 
// Prueba 4: Construcción de un montículo desde un arreglo
    int array[] = {15, 3, 8, 10, 5, 2};
    int n = sizeof(array) / sizeof(array[0]);                 
    crearMonticulo(&m, array, n);      
    printf("Después de crear el montículo desde un arreglo:\n");
    mostrarMonticulo(&m);              
    
    if (consultarMenor(&m) != 2) {
        printf("Error: La construcción del montículo desde el arreglo falló.\n");                         
    } else {
        printf("Prueba 4 superada: Construcción desde un arreglo correcta.\n");     
    }   

    printf("Todas las pruebas finalizadas.\n");
}

void OrdenarPorMontículos(pmonticulo M, int v[], int n) {
    int i;
    crearMonticulo(M,v,n);
    for(i=0; i<n; i++) {
        v[i] = consultarMenor(M);
        quitarMenor(M);
    }
}

int main(){
    pruebasMonticulo();
}



//comparar con la practica 2