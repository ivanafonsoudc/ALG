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
    int tmp;
    int hijoIzq = 2 * pos + 1;
    int hijoDer = 2 * pos + 2;

    if (hijoIzq <= m->ultimo && m->vector[hijoIzq] < m->vector[menor]) {
        menor = hijoIzq;
    }
    if (hijoDer <= m->ultimo && m->vector[hijoDer] < m->vector[menor]) {
        menor = hijoDer;
    }
    if (menor != pos) {
        tmp = m->vector[pos];
        m->vector[pos] = m->vector[menor];
        m->vector[menor] = tmp;
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

int k_esimoMAYOR(int v[], int n, int k){
    int i;
    struct monticulo m;
    crearMonticulo(&m, v, k);
    for (i = k; i < n; i++) {
        if (v[i] > consultarMenor(&m)) {
            quitarMenor(&m);
            insertarMonticulo(&m, v[i]);
        }
    }
    return consultarMenor(&m);
}

void probarKEsimoMayor() {
    int n = 10;
    int v[n];
    int i;
    int k_esimo;
    aleatorio(v, n);
    printf("Vector original: ");
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
    printf("Elementos extraidos en orden descendente: ");
    for (i = 1; i <= n; i++) {
        k_esimo = k_esimoMAYOR(v, n, i);
        printf("%d ", k_esimo);
    }
    printf("\n");
}

double f1_aleat(int n) { return pow(n, 0.8); }
double f2_aleat(int n) { return pow(n,0.97); }
double f3_aleat(int n) { return pow(n,1.2); }
double f1_aleatN2(int n) { return pow(n, 0.9); }
double f2_aleatN2(int n) { return n*log(n); }
double f3_aleatN2(int n) { return pow(n,1.5); }

void medicionesMayor9(void (*ordenacion)(int[], int), double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    int n = 256000;
    int i;
    int ks = 9; 
    int k = 10000; 
    int v[n], h;
    double t1, t2, t, ta, tb;
    printf("%5s\t%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    ordenacion(v, n); 
    for (i = 4000; i <= n; i *= 2) {
        t1 = microsegundos();
        k_esimoMAYOR(v, i, ks);
        t2 = microsegundos();
        t = (t2 - t1);
        if (t < 500) {
            ta = microsegundos();
            for (h = 0; h < k; h++) {
                k_esimoMAYOR(v, i, ks);
            }
            tb = microsegundos();
            t = (tb - ta) / k;
            printf("*");
        }
        printf("%5d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, t / f1(i), t / f2(i), t / f3(i));
    }
    printf("\n");
}

void medicionesMayorN2(void (*ordenacion)(int[], int), double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    int n = 256000;
    int i,ks;
    int k = 10000; 
    int v[n], h;
    double t1, t2, t, ta, tb;
    printf("%5s\t%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    ordenacion(v, n); 
    for (i = 4000; i <= n; i *= 2) {
        ks = i / 2;
        t1 = microsegundos();
        k_esimoMAYOR(v, i, ks);
        t2 = microsegundos();
        t = (t2 - t1);
        if (t < 500) {
            ta = microsegundos();
            for (h = 0; h < k; h++) {
                k_esimoMAYOR(v, i, ks);
            }
            tb = microsegundos();
            t = (tb - ta) / k;
            printf("*");
        }
        printf("%5d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, t / f1(i), t / f2(i), t / f3(i));
    }
    printf("\n");
}



int main(){
    inicializar_semilla();
    probarKEsimoMayor();
    inicializar_semilla();
    medicionesMayor9(aleatorio, f1_aleat, f2_aleat, f3_aleat);
    medicionesMayorN2(aleatorio, f1_aleatN2, f2_aleatN2, f3_aleatN2);
    return 0;
}