/*
Grupo 1.2
Iván Afonso Cerdeira ivan.afonso@udc.es
Minerva Antía Lago López minerva.lago.lopez@udc.es  
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

double microsegundos() {                  
    struct timeval t;                         
    if (gettimeofday(&t, NULL) < 0 ){          
        return 0.0;}                               
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializar_semilla() {
    srand(time(NULL)); 
}

void aleatorio(int v [], int n){
    int i, m=2*n+1;
    for (i=0; i < n; i++){
         v[i] = (rand() % m) - n;
    }
}
void ascendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++){
        v[i] = i;}
}

void descendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++){
        v[i] = n-i;}
}

void ordenacionPorInsercion(int v[], int n, void(*generador)(int[], int)) {
    generador(v, n);
    int x,j,i;
    for(i=0;i<n;i++) {
        x = v[i];
        j = i-1;
        while(j>=0 && v[j]>x) {
            v[j+1] = v[j];
            j--;
        }
        v[j+1]=x;
    }
}

void intercambiar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void ordenacionRapidaAuxiliar(int v[], int iz, int dr) {
    int i,j,x,pivote;
    if(iz<dr) {
        x= iz + (rand() % (dr - iz + 1)); //Crear número aleatorio
        pivote=v[x];
        intercambiar(&v[iz],&v[x]);
        i=iz+1;
        j=dr;
        while(i<=j) {
            while(i<=dr && v[i]<pivote) {
                i=i+1;
            }
            while(v[j]>pivote) {
                j=j-1;
            }
            if(i<=j) {
                intercambiar(&v[i],&v[j]);
                i=i+1;
                j=j-1;
            }
        }
        intercambiar(&v[iz],&v[j]);
        ordenacionRapidaAuxiliar(v,iz,j-1);
        ordenacionRapidaAuxiliar(v,j+1,dr);
    }
}

void ordenacionRapida(int v[], int n, void(*generador)(int[], int)) {
    generador(v, n);
    ordenacionRapidaAuxiliar(v,0,n-1);
}

void imprimirArray(int v[], int n) {
    int i;
    for(i=0; i<n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void mediciones(void (*generarDatos)(int[], int), void (*ordenar)(int[], int, void (*)(int[], int)), const char* nombre, double (*f1)(int), double (*f2)(int), double (*f3)(int)) {
    int i, j;
    int k = 100;
    double t1, t2, ta, tb, t, x, y, z;
    int v[64000];
    printf("\n%s\n%s\t%15s%15s%15s%15s\n", nombre, "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    for (i = 500; i <= 64000; i = i * 2) {
        t1 = microsegundos();
        ordenar(v, i, generarDatos);
        t2 = microsegundos();
        t = (t2 - t1);
        if (t < 500) {
            ta = microsegundos();
            for (j = 0; j <= k; j++) {
                ordenar(v, i, generarDatos);
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
}

// Funciones para calcular f(n), g(n) y h(n)
double f1_rapida_aleatorio(int n) { return pow(n, 1); }
double f2_rapida_aleatorio(int n) { return pow(n, 1.1); }
double f3_rapida_aleatorio(int n) { return pow(n, 1.2); }

double f1_rapida_descendente(int n) { return pow(n, 0.90); }
double f2_rapida_descendente(int n) { return pow(n, 1.05); }
double f3_rapida_descendente(int n) { return pow(n, 1.10); }

double f1_rapida_ascendente(int n) { return pow(n, 0.90); }
double f2_rapida_ascendente(int n) { return pow(n, 1.10); }
double f3_rapida_ascendente(int n) { return pow(n, 1.20); }

double f1_insercion_aleatorio(int n) { return pow(n, 1.9); }
double f2_insercion_aleatorio(int n) { return pow(n, 2); }
double f3_insercion_aleatorio(int n) { return pow(n, 2.1); }

double f1_insercion_descendente(int n) { return pow(n, 1.8); }
double f2_insercion_descendente(int n) { return pow(n, 2); }
double f3_insercion_descendente(int n) { return pow(n, 2.2); }

double f1_insercion_ascendente(int n) { return pow(n, 0.9); }
double f2_insercion_ascendente(int n) { return pow(n, 1); }
double f3_insercion_ascendente(int n) { return pow(n, 1.1); }




void testaux(void(*ordenacion)(int[], int), void(*generador)(int[], int),int v[],int n, const char* nombre){
    printf("%s\n", nombre);
    generador(v, n);
    printf("Array before sorting:\n");
    imprimirArray(v, n);
    ordenacion(v, n);
    printf("Array after sorting:\n");
    imprimirArray(v, n);
    printf("\n");
}

void test(){
    int n = 10;
    int v[n];
    inicializar_semilla();
    testaux(ordenacionRapida, aleatorio, v, n, "Rápida Aleatorio");
    testaux(ordenacionRapida, ascendente, v, n, "Rápida Ascendente");
    testaux(ordenacionRapida, descendente, v, n, "Rápida Descendente");
    testaux(ordenacionPorInsercion, aleatorio, v, n, "Insercion Aleatorio");
    testaux(ordenacionPorInsercion, ascendente, v, n, "Insercion Ascendente");
    testaux(ordenacionPorInsercion, descendente, v, n, "Insercion Descendente");
}


int main() {
    inicializar_semilla();
    //test();
    mediciones(aleatorio, ordenacionRapida, "Rápida Aleatorio", f1_rapida_aleatorio, f2_rapida_aleatorio, f3_rapida_aleatorio);
    mediciones(descendente, ordenacionRapida, "Rápida Descendente", f1_rapida_descendente, f2_rapida_descendente, f3_rapida_descendente);
    mediciones(ascendente, ordenacionRapida, "Rápida Ascendente", f1_rapida_ascendente, f2_rapida_ascendente, f3_rapida_ascendente);

    mediciones(aleatorio, ordenacionPorInsercion, "Insercion Aleatorio", f1_insercion_aleatorio, f2_insercion_aleatorio, f3_insercion_aleatorio);
    mediciones(descendente, ordenacionPorInsercion, "Insercion Descendente", f1_insercion_descendente, f2_insercion_descendente, f3_insercion_descendente);
    mediciones(ascendente, ordenacionPorInsercion, "Insercion Ascendente", f1_insercion_ascendente, f2_insercion_ascendente, f3_insercion_ascendente);

    return 0;
}
