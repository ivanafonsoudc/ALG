#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#define TAM 256000

struct monticulo {
    int ultimo;
    int vector[TAM];
};
typedef struct monticulo* pmonticulo;


double microsegundos() {                  
    struct timeval t;                         
    if (gettimeofday(&t, NULL) < 0 ){          
        return 0.0;}                               
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializar_semilla(){
    srand(time(NULL));
}



void ascendente(int v[], int n) { 
    for (int i = 0; i < n; i++) {
        v[i] = i;
    }
}

void descendente(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = n - i;
    }
}

void aleatorio(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = rand() % n;
    }
}


// Inicializa el montículo
void iniMonticulo(pmonticulo m) {
    m->ultimo = -1;
}

// Flotar un elemento hacia arriba
void flotar(pmonticulo m, int pos) {
    int padre = (pos - 1) / 2;
    while (pos > 0 && m->vector[padre] > m->vector[pos]) {
        int temp = m->vector[padre];
        m->vector[padre] = m->vector[pos];
        m->vector[pos] = temp;
        pos = padre;
        padre = (pos - 1) / 2;
    }
}

// Inserta un elemento en el montículo
void insertarMonticulo(pmonticulo m, int x) {
    if (m->ultimo == TAM - 1) {
        printf("Montículo lleno\n");
        return;
    }
    m->vector[++(m->ultimo)] = x;
    flotar(m, m->ultimo);
}

// Hundir un elemento hacia abajo
void hundir(pmonticulo m, int pos) {
    int menor = pos;
    int hijoIzq = 2 * pos + 1;
    int hijoDer = 2 * pos + 2;

    if (hijoIzq <= m->ultimo && m->vector[hijoIzq] < m->vector[menor]) {
        menor = hijoIzq;
    }
    if (hijoDer <= m->ultimo && m->vector[hijoDer] < m->vector[menor]) {
        menor = hijoDer;
    }
    if (menor != pos) {
        int temp = m->vector[pos];
        m->vector[pos] = m->vector[menor];
        m->vector[menor] = temp;
        hundir(m, menor);
    }
}

// Quita el menor elemento del montículo
void quitarMenor(pmonticulo m) {
    if (m->ultimo < 0) {
        printf("Montículo vacío\n");
        return;
    }
    m->vector[0] = m->vector[m->ultimo--];
    hundir(m, 0);
}

// Consulta el menor elemento del montículo
int consultarMenor(const pmonticulo m) {
    if (m->ultimo < 0) {
        printf("Montículo vacío\n");
        return INT_MAX;
    }
    return m->vector[0];
}

// Construye un montículo a partir de un vector
void crearMonticulo(pmonticulo m, int v[], int n) {
    iniMonticulo(m);
    for (int i = 0; i < n; i++) {
        m->vector[i] = v[i];
    }
    m->ultimo = n - 1;
    for (int i = (m->ultimo - 1) / 2; i >= 0; i--) {
        hundir(m, i);
    }
}

void ordenarPorMonticulos(int v[], int n) {
    struct monticulo m;
    crearMonticulo(&m, v, n);
    for (int i = 0; i < n; i++) {
        v[i] = consultarMenor(&m);
        quitarMenor(&m);
    }
}

void mediciones(void (*ordenacion)(int[], int), double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    struct monticulo m;                                  
    int n = 64000;                                         
    int i;                                           
    int k = 500; // Número de repeticiones para promediar tiempos pequeños                                                                  
    int v[n], h;                                        
    double t1, t2, t3, t, ta, tb, t_aux1, t_aux2;                     
                                                                    
    printf("%5s\t%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");                                                  
            
    ordenacion(v, n); // Inicialización del vector                         
            
    for (i = 500; i <= n; i *= 2) {
        iniMonticulo(&m); // Inicialización del montículo
        t1 = microsegundos(); // Captura del tiempo inicial
        ordenarPorMonticulos(v, i);                    
        t2 = microsegundos(); // Captura del tiempo final
        t = (t2 - t1); // Tiempo de ejecución directo      
                                                     
        if (t < 500) { // Si el tiempo es menor al umbral de confianza
            // Medición con K repeticiones del algoritmo              
            ta = microsegundos();                       
            for (h = 0; h < k; h++) {                                 
                iniMonticulo(&m); // Re-inicialización del montículo
                ordenarPorMonticulos(v, i);                     
            }                                                       
            tb = microsegundos();
            t_aux1 = (tb - ta); // Tiempo total de K repeticiones
                                                                 
            // Medición del costo adicional de inicialización
            ta = microsegundos();                            
            for (h = 0; h < k; h++) {
                iniMonticulo(&m); // Solo inicialización
            }                                           
            tb = microsegundos();
            t_aux2 = (tb - ta); // Tiempo total de K inicializaciones
                                                                     
            t3 = (t_aux1 - t_aux2) / k; // Tiempo promedio descontando inicialización
            printf("%5d*\t%15.3f%15.8f%15.8f%15.8f\n", i, t3, t3 / f1(i), t3 / f2(i), t3 / f3(i));
        } else {          
            // Si el tiempo es mayor o igual al umbral, se reporta directamente     
            printf("%5d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, t / f1(i), t / f2(i), t / f3(i));
        }            
    }    
    printf("\n");
}  

void medicionCrearMonticulo(double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    int l = 1000;
    int v[l];
    int n;
    struct monticulo m;
    double t, t1, t2;
    double promedio = 0.0;
    int k=100;

    for (n=500; n <= 64000; n *= 2) {
        promedio = 0.0;
        t1 = microsegundos();
        for (int i = 0; i < n; i++) {
            iniMonticulo(&m);
            for (int j = 0; j < l; j++) {
                insertarMonticulo(&m, v[j]); //Añade l nodos al monticulo
            }
        }
        t2 = microsegundos();
        t = t2 - t1;
        if (t < 500) {
            promedio = 0.0;
            do {
                t1 = microsegundos();
                for (int i = 0; i < k; i++) {
                    for (int j = 0; j < n; j++) {
                        iniMonticulo(&m);
                        for (int k = 0; k < l; k++) {
                            insertarMonticulo(&m, v[k]); //Añade l nodos al monticulo
                        }
                    }
                }
                t2 = microsegundos();
                promedio = t2 - t1;

            } while (promedio < 500);

            t = promedio / k;
        }
        printf("%5d\t%15.3f\t%15.8f\t%15.8f\t%15.8f\n",
               n, t, t / f1(n), t / f2(n), t / f3(n));
    }
    printf("\n");
}

void medicionInsertarMonticulo(double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    int l = 10000;
    int v[l];
    int n;
    struct monticulo m;
    double t, t1, t2;
    int k = 100;

    for (n = 500; n <= 64000; n *= 2) {
        t1 = microsegundos();
        for (int i = 0; i < n; i++) {
            iniMonticulo(&m);
            for (int j = 0; j < l; j++) {
                insertarMonticulo(&m, v[j]); //Añade l nodos
            }
        }
        t2 = microsegundos();
        t = t2 - t1;

        if (t < 500) {
            do {
                t1 = microsegundos();
                for (int i = 0; i < k; i++) {
                    for (int j = 0; j < n; j++) {
                        iniMonticulo(&m);
                        for (int z = 0; z < l; z++) {
                            insertarMonticulo(&m, v[z]); //Añade l nodos
                        }
                    }
                }
                t2 = microsegundos();
                t = (t2 - t1);
            } while (t < 500);
            t = t / k;
        }
        printf("%5d\t%15.3f\t%15.8f\t%15.8f\t%15.8f\n",
               n, t, t / f1(n), t / f2(n), t / f3(n));
    }

    printf("\n");
}

double f1_desc(int n) { return pow(n, 0.8); }
double f2_desc(int n) { return (n*log(n)); }
double f3_desc(int n) { return pow(n, 1.5); }

double f1_asc(int n) { return pow(n, 0.8); }
double f2_asc(int n) { return (n*log(n)); }
double f3_asc(int n) { return pow(n, 1.5); }

double f1_aleat(int n) { return pow(n, 0.8); }
double f2_aleat(int n) { return (n*log(n)); }
double f3_aleat(int n) { return pow(n,1.5); }

double f1_insertarMonticulo(int n) { return /*log(n)*/ pow(n, 0.8); }
double f2_insertarMonticulo(int n) { return n; }
double f3_insertarMonticulo(int n) { return pow(n, 1.05); }

double f1_crearMonticulo(int n) { return pow(n,0.5); /*n * log(pow(n, 0.2));*/ }
double f2_crearMonticulo(int n) { return pow(n,0.972); }
double f3_crearMonticulo(int n) { return pow(n, 1.485); }

bool estaOrdenado(int v[], int n) {
    for (int i = 1; i < n; i++) {
        if (v[i] < v[i - 1]) {
            return false;
        }
    }
    return true;
}

void imprimirArreglo(int v[], int n) {
    printf("[ ");
    for (int i = 0; i < n; i++) {
        printf("%4d ", v[i]);
    }
    printf("]\n");
}

void imprimirMonticulo(pmonticulo m) {
    printf("[ ");
    for (int i = 0; i < m->ultimo; i++) {
        printf("%4d ", m->vector[i]);
    }
    printf("]\n");
}


void probarMonticulos() {
    struct monticulo m;
    int v[TAM];
    int n = 10; // Tamaño de prueba

    // Generar un arreglo aleatorio
    printf("Inicializacion aleatoria:\n");
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        v[i] = (rand() % 21) - 10; // Valores entre -10 y 10
    }
    imprimirArreglo(v, n);
    printf("Ordenado? %d\n", estaOrdenado(v, n));

    // Probar la ordenación por montículos
    printf("Ordenacion por montículos:\n");
    ordenarPorMonticulos(v, n);
    imprimirArreglo(v, n);
    printf("Ordenado? %d\n", estaOrdenado(v, n));

    // Generar un nuevo arreglo aleatorio para probar la creación del montículo
    printf("Test creación montículo:\n");
    for (int i = 0; i < n; i++) {
        v[i] = (rand() % 21) - 10; // Valores entre -10 y 10
    }
    imprimirArreglo(v, n);
    crearMonticulo(&m, v, n);
    imprimirMonticulo(&m);

    // Probar quitarMenor
    printf("Test quitarMenor:\n");
    int operaciones = 0;
    while (m.ultimo > 0) {
        imprimirMonticulo(&m);
        quitarMenor(&m);
        operaciones++;
    }
    printf("Se han llevado a cabo %d operaciones quitarMenor\n", operaciones);
}


int main(){
    inicializar_semilla();
    probarMonticulos();
    printf("Prueba con orden descendente:\n");
    mediciones(descendente, f1_desc, f2_desc, f3_desc);
    printf("\nPrueba con orden ascendente:\n");
    mediciones(ascendente, f1_asc, f2_asc, f3_asc);
    printf("\nPrueba con orden aleatorio:\n");
    mediciones(aleatorio, f1_aleat, f2_aleat, f3_aleat);
    printf("Mediciones para insertarMonticulo:\n");
    medicionInsertarMonticulo(f1_insertarMonticulo, f2_insertarMonticulo, f3_insertarMonticulo);
    printf("\nMediciones para crearMonticulo:\n");
    medicionCrearMonticulo(f1_crearMonticulo, f2_crearMonticulo, f3_crearMonticulo);
    return 0;
}

//comparar con la practica 2