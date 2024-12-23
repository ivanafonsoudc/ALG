#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <stdlib.h>
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
    int i;
    for (i = 0; i < n; i++) {
        v[i] = i;
    }
}

void descendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = n - i;
    }
}

void aleatorio(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = rand() % n;
    }
}

void iniMonticulo(pmonticulo m) {
    m->ultimo = -1;
}

void flotar(pmonticulo m, int pos) {
    int padre = (pos - 1) / 2;
    int temp;
    while (pos > 0 && m->vector[padre] > m->vector[pos]) {
        temp = m->vector[padre];
        m->vector[padre] = m->vector[pos];
        m->vector[pos] = temp;
        pos = padre;
        padre = (pos - 1) / 2;
    }
}

void insertarMonticulo(pmonticulo m, int x) {
    if (m->ultimo == TAM - 1) {
        printf("Montículo lleno\n");
        return;
    }
    m->vector[++(m->ultimo)] = x;
    flotar(m, m->ultimo);
}

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

void quitarMenor(pmonticulo m) {
    if (m->ultimo < 0) {
        printf("Montículo vacío\n");
        return;
    }
    m->vector[0] = m->vector[m->ultimo--];
    hundir(m, 0);
}

int consultarMenor(const pmonticulo m) {
    if (m->ultimo < 0) {
        printf("Montículo vacío\n");
        return INT_MAX;
    }
    return m->vector[0];
}

void crearMonticulo(pmonticulo m, int v[], int n) {
    int i;
    iniMonticulo(m);
    for (i = 0; i < n; i++) {
        m->vector[i] = v[i];
    }
    m->ultimo = n - 1;
    for (i = (m->ultimo - 1) / 2; i >= 0; i--) {
        hundir(m, i);
    }
}

void ordenarPorMonticulos(int v[], int n) {
    struct monticulo m;
    int i;
    crearMonticulo(&m, v, n);
    for (i = 0; i < n; i++) {
        v[i] = consultarMenor(&m);
        quitarMenor(&m);
    }
}

void mediciones(void (*ordenacion)(int[], int), double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    struct monticulo m;
    int n = 256000;
    int i;
    int k = 10000; // Número de repeticiones para promediar tiempos pequeños
    int v[n], h;
    double t1, t2, t, ta, tb, t_aux1, t_aux2;
    printf("%5s\t%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    ordenacion(v, n); // Inicialización del vector
    for (i = 4000; i <= n; i *= 2) {
        iniMonticulo(&m);
        t1 = microsegundos();
        ordenarPorMonticulos(v, i);
        t2 = microsegundos();
        t = (t2 - t1);
        if (t < 500) {
            ta = microsegundos();
            for (h = 0; h < k; h++) {
                iniMonticulo(&m);
                ordenarPorMonticulos(v, i);
            }
            tb = microsegundos();
            t_aux1 = (tb - ta);
            ta = microsegundos();
            for (h = 0; h < k; h++) {
                iniMonticulo(&m);
            }
            tb = microsegundos();
            t_aux2 = (tb - ta);
            t = (t_aux1 - t_aux2) / k;
        }
        printf("%5d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, t / f1(i), t / f2(i), t / f3(i));

    }
    printf("\n");
}

void medicionInsertarMonticulo(double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    struct monticulo m;
    double t, t1, t2, t_aux1, t_aux2;
    int k = 1000;
    int h,n, i;
    int *v;
    for (n = 4000; n <= TAM; n *= 2) {
        v = malloc(n * sizeof(int));
        for (i = 0; i < n; i++) {
            v[i] = rand() % n;
        }
        iniMonticulo(&m);
        t1 = microsegundos();
        for (i = 0; i < n; i++) {
            insertarMonticulo(&m, v[i]);
        }
        t2 = microsegundos();
        t = t2 - t1;
        if (t < 500) {
            iniMonticulo(&m);
            t1 = microsegundos();
            for (int i = 0; i < k; i++) {
                iniMonticulo(&m);
                for (int j = 0; j < n; j++) {
                    insertarMonticulo(&m, v[j]);
                }
            }
            t2 = microsegundos();
            t_aux1 = (t2 - t1) ;
            t1 = microsegundos();
            for (h = 0; h < k; h++) {
                iniMonticulo(&m);
            }
            t2 = microsegundos();
            t_aux2 = (t2 - t1);
            t = (t_aux1 - t_aux2) / k;
            printf("*");
        }
        printf("%5d\t%15.3f\t%15.8f\t%15.8f\t%15.8f\n",
               n, t, t / f1(n), t / f2(n), t / f3(n));
        free(v);
    }
    printf("\n");
}


void medicionCrearMonticulo(double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    struct monticulo m;
    double t, t1, t2, t_aux1, t_aux2;
    int k = 1000;
    int n, i,h;
    int *v;
    for (n = 4000; n <= 256000; n *= 2) {
        v = malloc(n * sizeof(int));
        aleatorio(v, n);
        iniMonticulo(&m);
        t1 = microsegundos();
        crearMonticulo(&m, v, n);
        t2 = microsegundos();
        t = t2 - t1;
        if (t < 500) {
            t1 = microsegundos();
            for (i = 0; i < k; i++) {
                iniMonticulo(&m);
                crearMonticulo(&m, v, n);
            }
            t2 = microsegundos();
            t_aux1 = (t2 - t1) ;
            t1 = microsegundos();
            for (h = 0; h < k; h++) {
                iniMonticulo(&m);
            }
            t2 = microsegundos();
            t_aux2 = (t2 - t1);
            t = (t_aux1 - t_aux2) / k;
            printf("*");
        }
        printf("%5d\t%15.3f\t%15.8f\t%15.8f\t%15.8f\n",
               n, t, t / f1(n), t / f2(n), t / f3(n));
        free(v);
    }
    printf("\n");
}

double f1_desc(int n) { return pow(n, 0.7); }
double f2_desc(int n) { return (n*log(n)); }
double f3_desc(int n) { return pow(n, 1.44); }

double f1_asc(int n) { return pow(n, 1.025); }
double f2_asc(int n) { return (n*log(n)); }
double f3_asc(int n) { return pow(n, 1.421); }

double f1_aleat(int n) { return pow(n, 0.9); }
double f2_aleat(int n) { return (n*log(n)); }
double f3_aleat(int n) { return pow(n,1.452); }

double f1_insertarMonticulo(int n) { return pow(n, 0.8); }
double f2_insertarMonticulo(int n) { return pow(n,1); }
double f3_insertarMonticulo(int n) { return pow(n, 1.35); }

double f1_crearMonticulo(int n) { return pow(n,0.8); }
double f2_crearMonticulo(int n) { return n*log(n); }
double f3_crearMonticulo(int n) { return pow(n, 1.45); }

bool estaOrdenado(int v[], int n) {
    int i;
    for (i = 1; i < n; i++) {
        if (v[i] < v[i - 1]) {
            return false;
        }
    }
    return true;
}

void imprimirArreglo(int v[], int n) {
    int i;
    printf("[ ");
    for (i = 0; i < n; i++) {
        printf("%4d ", v[i]);
    }
    printf("]\n");
}

void imprimirMonticulo(pmonticulo m) {
    int i;
    printf("[ ");
    for (i = 0; i < m->ultimo; i++) {
        printf("%4d ", m->vector[i]);
    }
    printf("]\n");
}
void probarMonticulos() {
    struct monticulo m;
    int v[TAM];
    int n = 10;
    int i;
    int operaciones = 0;
    printf("Inicializacion aleatoria:\n");
    for (i = 0; i < n; i++) {
        v[i] = (rand() % 21) - 10;
    }
    imprimirArreglo(v, n);
    printf("Ordenado? %d\n", estaOrdenado(v, n));
    printf("Ordenacion por montículos:\n");
    ordenarPorMonticulos(v, n);
    imprimirArreglo(v, n);
    printf("Ordenado? %d\n", estaOrdenado(v, n));
    printf("Test creación montículo:\n");
    for (i = 0; i < n; i++) {
        v[i] = (rand() % 21) - 10;
    }
    imprimirArreglo(v, n);
    crearMonticulo(&m, v, n);
    imprimirMonticulo(&m);
    printf("Test quitarMenor:\n");
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
    printf("%5s\t%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    medicionInsertarMonticulo( f1_insertarMonticulo, f2_insertarMonticulo, f3_insertarMonticulo);
    printf("\nMediciones para crearMonticulo:\n");
    printf("%5s\t%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    medicionCrearMonticulo(f1_crearMonticulo, f2_crearMonticulo, f3_crearMonticulo);
    return 0;
}