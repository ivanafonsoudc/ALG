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

int mediana3(int v[], int iz, int dr){
    int medio = (iz + dr) / 2;
    if(v[iz] > v[medio]){
        intercambiar(&v[iz], &v[medio]);
    }
    if(v[iz] > v[dr]){
        intercambiar(&v[iz], &v[dr]);
    }
    if(v[medio] > v[dr]){
        intercambiar(&v[medio], &v[dr]);
    }
    return medio;
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

void ordenacionRapidaAuxiliarM3(int v[], int iz, int dr) {
    int i,j,pivote, indpivote;
    if(iz<dr) {
        indpivote = mediana3(v,iz,dr);
        pivote=v[indpivote];
        intercambiar(&v[iz],&v[indpivote]);
        i=iz+1;
        j=dr;
        while(i<=j) {
            while(i<=dr && v[i]<pivote) {
                i++;
            }
            while(v[j]>pivote) {
                j--;
            }
            if(i<=j) {
                intercambiar(&v[i],&v[j]);
                i++;
                j--;
            }
        }
        intercambiar(&v[iz],&v[j]);
        ordenacionRapidaAuxiliar(v,iz,j-1);
        ordenacionRapidaAuxiliar(v,j+1,dr);
    }
}

void ordenacionRapidaM3(int v[], int n, void(*generador)(int[], int)) {
    generador(v, n);
    ordenacionRapidaAuxiliar(v,0,n-1);
}

void ordenacionBurbuja(int v[], int n, void(*generador)(int[], int)) {
    generador(v, n);
    int i, j;
    for(i=0; i<n-1; i++) {
        for(j=0; j<n-i-1; j++) {
            if(v[j] > v[j+1]) {
                intercambiar(&v[j], &v[j+1]);
            }
        }
    }
}

void ordenacionBurbujaOptimizada(int v[], int n, void(*generador)(int[], int)) {
    generador(v, n);
    int i, j;
    int cambio = 1;
    for(i=0; i<n-1 && cambio; i++) {
        cambio = 0;
        for(j=0; j<n-i-1; j++) {
            if(v[j] > v[j+1]) {
                intercambiar(&v[j], &v[j+1]);
                cambio = 1;
            }
        }
    }
}

void ordenacionShell(int v[], int n, void(*generador)(int[], int)) {
    generador(v, n);
    int i, j, x;
    for(x=n/2; x>0; x/=2) {
        for(i=x; i<n; i++) {
            j = i;
            while(j>=x && v[j-x]>v[j]) {
                intercambiar(&v[j-x], &v[j]);
                j = j - x;
            }
        }
    }
}



void ordenacionFusionAuxiliar(int v[], int iz, int dr) {
    int i, j, k, m;
    int *aux;
    if (iz < dr) {
        m = (iz + dr) / 2;
        ordenacionFusionAuxiliar(v, iz, m);
        ordenacionFusionAuxiliar(v, m + 1, dr);
        aux = (int *)malloc((dr - iz + 1) * sizeof(int));
        if (aux == NULL) {
            fprintf(stderr, "Error al asignar memoria\n");
            exit(EXIT_FAILURE);
        }
        for (i = iz; i <= dr; i++) {
            aux[i - iz] = v[i];
        }
        i = iz;
        j = m + 1;
        k = iz;
        while (i <= m && j <= dr) {
            if (aux[i - iz] <= aux[j - iz]) {
                v[k++] = aux[i++ - iz];
            } else {
                v[k++] = aux[j++ - iz];
            }
        }
        while (i <= m) {
            v[k++] = aux[i++ - iz];
        }
        while (j <= dr) {
            v[k++] = aux[j++ - iz];
        }
        free(aux);
    }
}

void ordenacionFusion(int v[], int n, void (*generador)(int[], int)) {
    generador(v, n);
    ordenacionFusionAuxiliar(v, 0, n - 1);
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

double f1_rapida_med3(int n) {return pow(n,0.9);}
double f2_rapida_med3(int n) {return pow(n,1.1);}
double f3_rapida_med3(int n) {return pow(n,1.2);}

double f1_insercion_aleatorio(int n) { return pow(n, 1.9); }
double f2_insercion_aleatorio(int n) { return pow(n, 2); }
double f3_insercion_aleatorio(int n) { return pow(n, 2.1); }

double f1_insercion_descendente(int n) { return pow(n, 1.8); }
double f2_insercion_descendente(int n) { return pow(n, 2); }
double f3_insercion_descendente(int n) { return pow(n, 2.2); }

double f1_insercion_ascendente(int n) { return pow(n, 0.9); }
double f2_insercion_ascendente(int n) { return pow(n, 1); }
double f3_insercion_ascendente(int n) { return pow(n, 1.1); }

double f1_burbuja_aleatorio(int n) { return pow(n, 2); }
double f2_burbuja_aleatorio(int n) { return pow(n, 2.1); }
double f3_burbuja_aleatorio(int n) { return pow(n, 2.2); }

double f1_burbuja_descendente(int n) { return pow(n, 2); }
double f2_burbuja_descendente(int n) { return pow(n, 2.1); }
double f3_burbuja_descendente(int n) { return pow(n, 2.2); }

double f1_burbuja_ascendente(int n) { return pow(n, 2); }
double f2_burbuja_ascendente(int n) { return pow(n, 2.1); }
double f3_burbuja_ascendente(int n) { return pow(n, 2.2); }

double f1_burbuja_optimizada_aleatorio(int n) { return pow(n, 2); }
double f2_burbuja_optimizada_aleatorio(int n) { return pow(n, 2.1); }
double f3_burbuja_optimizada_aleatorio(int n) { return pow(n, 2.2); }

double f1_burbuja_optimizada_descendente(int n) { return pow(n, 1.8 ); }
double f2_burbuja_optimizada_descendente(int n) { return pow(n, 2); }
double f3_burbuja_optimizada_descendente(int n) { return pow(n, 2.2); }

double f1_burbuja_optimizada_ascendente(int n) { return pow(n, 0.8); }
double f2_burbuja_optimizada_ascendente(int n) { return pow(n, 1); }
double f3_burbuja_optimizada_ascendente(int n) { return pow(n, 1.3); }

double f1_shell_aleatorio(int n) { return pow(n, 0.9); }
double f2_shell_aleatorio(int n) { return pow(n, 1.2); }
double f3_shell_aleatorio(int n) { return pow(n, 1.4); }

double f1_shell_descendente(int n) { return pow(n, 0.9); }
double f2_shell_descendente(int n) { return pow(n, 1.1); }
double f3_shell_descendente(int n) { return pow(n, 1.3); }

double f1_shell_ascendente(int n) { return pow(n, 0.9); }
double f2_shell_ascendente(int n) { return pow(n, 1.1); }
double f3_shell_ascendente(int n) { return pow(n, 1.3); }

double f1_fusion_aleatorio(int n) { return pow(n, 0.9); }
double f2_fusion_aleatorio(int n) { return pow(n, 1.1); }
double f3_fusion_aleatorio(int n) { return pow(n, 1.3); }

double f1_fusion_descendente(int n) { return pow(n, 0.9); }
double f2_fusion_descendente(int n) { return pow(n, 1.1); }
double f3_fusion_descendente(int n) { return pow(n, 1.3); }

double f1_fusion_ascendente(int n) { return pow(n, 0.9); }
double f2_fusion_ascendente(int n) { return pow(n, 1.1); }
double f3_fusion_ascendente(int n) { return pow(n, 1.3); }




/*void testaux(void(*ordenacion)(int[], int), void(*generador)(int[], int),int v[],int n, const char* nombre){
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
}*/


int main() {
    inicializar_semilla();
    //test();
 // mediciones(aleatorio, ordenacionRapida, "Rápida Aleatorio", f1_rapida_aleatorio, f2_rapida_aleatorio, f3_rapida_aleatorio);
    //mediciones(descendente, ordenacionRapida, "Rápida Descendente", f1_rapida_descendente, f2_rapida_descendente, f3_rapida_descendente);
    //mediciones(ascendente, ordenacionRapida, "Rápida Ascendente", f1_rapida_ascendente, f2_rapida_ascendente, f3_rapida_ascendente);
    //mediciones(aleatorio, ordenacionRapidaM3, "Rapida Mediana de 3", f1_rapida_med3,f2_rapida_med3,f3_rapida_med3 );

    //mediciones(aleatorio, ordenacionPorInsercion, "Insercion Aleatorio", f1_insercion_aleatorio, f2_insercion_aleatorio, f3_insercion_aleatorio);
   // mediciones(descendente, ordenacionPorInsercion, "Insercion Descendente", f1_insercion_descendente, f2_insercion_descendente, f3_insercion_descendente);
    //mediciones(ascendente, ordenacionPorInsercion, "Insercion Ascendente", f1_insercion_ascendente, f2_insercion_ascendente, f3_insercion_ascendente);

    //mediciones(aleatorio, ordenacionBurbuja, "Burbuja Aleatorio", f1_burbuja_aleatorio, f2_burbuja_aleatorio, f3_burbuja_aleatorio);
  //  mediciones(descendente, ordenacionBurbuja, "Burbuja Descendente", f1_burbuja_descendente, f2_burbuja_descendente, f3_burbuja_descendente);
    //mediciones(ascendente, ordenacionBurbuja, "Burbuja Ascendente", f1_burbuja_ascendente, f2_burbuja_ascendente, f3_burbuja_ascendente);

//    mediciones(aleatorio, ordenacionBurbujaOptimizada, "Burbuja Optimizada Aleatorio", f1_burbuja_optimizada_aleatorio, f2_burbuja_optimizada_aleatorio, f3_burbuja_optimizada_aleatorio);
    mediciones(descendente, ordenacionBurbujaOptimizada, "Burbuja Optimizada Descendente", f1_burbuja_optimizada_descendente, f2_burbuja_optimizada_descendente, f3_burbuja_optimizada_descendente);
    //mediciones(ascendente, ordenacionBurbujaOptimizada, "Burbuja Optimizada Ascendente", f1_burbuja_optimizada_ascendente, f2_burbuja_optimizada_ascendente, f3_burbuja_optimizada_ascendente);

//    mediciones(aleatorio, ordenacionShell, "Shell Aleatorio", f1_shell_aleatorio, f2_shell_aleatorio, f3_shell_aleatorio);
  //  mediciones(descendente, ordenacionShell, "Shell Descendente", f1_shell_descendente, f2_shell_descendente, f3_shell_descendente);
    //mediciones(ascendente, ordenacionShell, "Shell Ascendente", f1_shell_ascendente, f2_shell_ascendente, f3_shell_ascendente);

//    mediciones(aleatorio, ordenacionFusion, "Fusion Aleatorio", f1_fusion_aleatorio, f2_fusion_aleatorio, f3_fusion_aleatorio);
  //  mediciones(descendente, ordenacionFusion, "Fusion Descendente", f1_fusion_descendente, f2_fusion_descendente, f3_fusion_descendente);
    //mediciones(ascendente, ordenacionFusion, "Fusion Ascendente", f1_fusion_ascendente, f2_fusion_ascendente, f3_fusion_ascendente);

    return 0;
}