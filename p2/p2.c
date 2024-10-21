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

void ordenacionPorInsercion(int v[], int n) {
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

void ordenacionRapida(int v[], int n) {
    ordenacionRapidaAuxiliar(v,0,n-1);
}

void imprimirArray(int v[], int n) {
    int i;
    for(i=0; i<n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void medicionesRapidaAleatorio(){
    int i,j;
    int k = 100;
    double t1, t2,ta,tb,t,x,y,z;
    int v[64000];
    printf("\nRápida Aleatorio\n%s\t%15s%15s%15s%15s\n","n","t(n)","t(n)/f(n)","t(n)/g(n)","t(n)/h(n)");
    for(i=500; i<=64000; i=i*2){
        t1 = microsegundos();
        aleatorio(v, i);
        ordenacionRapida(v, i);
        t2 = microsegundos();
        t = (t2-t1);
        if(t<500){
            ta=microsegundos();
            for(j=0; j<=k; j++){
                aleatorio(v, i);
                ordenacionRapida(v, i);
            }
            tb=microsegundos();
            t = (tb-ta)/k;
        }
        y = t/pow(i,1);
        x = t/pow(i,1.1);
        z = t/pow(i,1.2);
        printf("%d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, y,x,z);
    }
    printf("\n");
}

void medicionesRapidaDescendente(){
    int i,j;
    int k = 100;
    double t1, t2,ta,tb,t,x,y,z;
    int v[64000];
    printf("Rapida Descendente\n%s\t%15s%15s%15s%15s\n","n","t(n)","t(n)/f(n)","t(n)/g(n)","t(n)/h(n)");
    for(i=500; i<=64000; i=i*2) {
        t1 = microsegundos();
        descendente(v, i);
        ordenacionRapida(v, i);
        t2 = microsegundos();
        t = (t2-t1);
        if(t<500){
            ta=microsegundos();
            for(j=0; j<=k; j++){
                descendente(v, i);
                ordenacionRapida(v, i);
            }
            tb=microsegundos();
            t = (tb-ta)/k;
        }
        y = t/pow(i,0.90);
        x = t/pow(i,1.05);
        z = t/pow(i,1.10);
        printf("%d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, y,x,z);
    }
    printf("\n");
}

void medicionesRapidaAscendente(){
    int i,j;
    int k = 100;
    double t1, t2,ta,tb,t,x,y,z;
    int v[64000];
    printf("Rapida Ascendente\n%s\t%15s%15s%15s%15s\n","n","t(n)","t(n)/f(n)","t(n)/g(n)","t(n)/h(n)");
    for(i=500; i<=64000; i=i*2) {
        t1 = microsegundos();
        ascendente(v, i);
        ordenacionRapida(v, i);
        t2 = microsegundos();
        t = (t2-t1);
        if(t<500){
            ta=microsegundos();
            for(j=0; j<=k; j++){
                ascendente(v, i);
                ordenacionRapida(v, i);
            }
            tb=microsegundos();
            t = (tb-ta)/k;
        }
        y = t/pow(i,0.90);
        x = t/pow(i,1.10);
        z = t/pow(i,1.20);
        printf("%d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, y,x,z);
    }
    printf("\n");
}

void medicionesInsercionAleatorio(){
    int i,j;
    int k = 100;
    double t1, t2,ta,tb,t,x,y,z;
    int v[64000];
    printf("Insercion Aleatorio\n%s\t%15s%15s%15s%15s\n","n","t(n)","t(n)/f(n)","t(n)/g(n)","t(n)/h(n)");
    for(i=500; i<=64000; i=i*2) {
        t1 = microsegundos();
        aleatorio(v, i);
        ordenacionPorInsercion(v, i);
        t2 = microsegundos();
        t = (t2-t1);
        if(t<500){
            ta=microsegundos();
            for(j=0; j<=k; j++){
                aleatorio(v, i);
                ordenacionPorInsercion(v, i);
            }
            tb=microsegundos();
            t = (tb-ta)/k;
        }
        y = t/pow(i,1.9);
        x = t/pow(i,2);
        z = t/pow(i,2.1);
        printf("%d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, y,x,z);
    }
    printf("\n");
}

void medicionesInsercionDescendente(){
    int i,j;
    int k = 100;
    double t1, t2,ta,tb,t,x,y,z;
    int v[64000];
    printf("Insercion Descendente\n%s\t%15s%15s%15s%15s\n","n","t(n)","t(n)/f(n)","t(n)/g(n)","t(n)/h(n)");
    for(i=500; i<=64000; i=i*2){
        t1 = microsegundos();
        descendente(v, i);
        ordenacionPorInsercion(v, i);
        t2 = microsegundos();
        t = (t2-t1);
        if(t<500){
            ta=microsegundos();
            for(j=0; j<=k; j++){
                descendente(v, i);
                ordenacionPorInsercion(v, i);
            }
            tb=microsegundos();
            t = (tb-ta)/k;
        }
        y = t/pow(i,1.8);
        x = t/pow(i,2);
        z = t/pow(i,2.2);
        printf("%d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, y,x,z);
    }
    printf("\n");
}

void medicionesInsercionAscendente(){
    int i,j;
    int k = 1000;
    double t1, t2,ta,tb,t,x,y,z;
    int v[64000];
    printf("Insercion Ascendente\n%s\t%15s%15s%15s%15s\n","n","t(n)","t(n)/f(n)","t(n)/g(n)","t(n)/h(n)");
    for(i=500; i<=64000; i=i*2) {
        t1 = microsegundos();
        ascendente(v, i);
        ordenacionPorInsercion(v, i);
        t2 = microsegundos();
        t = (t2-t1);
        if(t<500){
            ta=microsegundos();
            for(j=0; j<=k; j++){
                ascendente(v, i);
                ordenacionPorInsercion(v, i);
            }
            tb=microsegundos();
            t = (tb-ta)/k;
        }
        y = t/pow(i,0.9);
        x = t/pow(i,1);
        z = t/pow(i,1.1);
        printf("%d\t%15.3f%15.8f%15.8f%15.8f\n", i, t, y,x,z);
    }
    printf("\n");
}

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
    test();
    medicionesRapidaAleatorio();
    medicionesRapidaDescendente();
    medicionesRapidaAscendente();
    medicionesInsercionAleatorio();
    medicionesInsercionDescendente();
    medicionesInsercionAscendente();
    return 0;
}
