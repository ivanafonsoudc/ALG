/*
Grupo 1.2
Iván Afonso Cerdeira ivan.afonso@udc.es
Minerva Antía Lago López minerva.lago.lopez@udc.es  
*/


#include <stdio.h>                                                                                                            
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

/* obtiene la hora actual en microsegundos */
double microsegundos() {                  
    struct timeval t;                         
    if (gettimeofday(&t, NULL) < 0 )          
        return 0.0;                               
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

int fib1 (int n) {               
    if (n < 2){                  
        return n;                
    }else{                       
    return fib1(n-1) + fib1(n-2);
    }                            
}                                
int fib2(int n){                 
    int i, j;                    
    i=1;j=0;                     
    for (int k = 1; k <= n; k++){
        j = i + j;
        i = j - i;
    }        
    return j;
}
 
 
int fib3(int n){              
    int i,j,k,h,t;            
    i=1;j=0;k=0;h=1;t=0;      
    while(n>0){               
        if(n%2!=0){           
            t = j*h;          
            j = i*h + j*k + t;
            i = i*k + t;
        }             
        t = h*h;      
        h = 2*k*h + t;
        k = k*k + t;
        n = n/2;
    }        
    return j;
}

int fibBinet(int n){
    double phy = (1 + sqrt(5)/2);
    double psy = (1 - sqrt(5)/2);
    return round((pow(phy,n) - pow(psy,n))/sqrt(5));
}

int fibMemoAux(int n , int *memo){
    if(n<=1){
        return n;
    }
    if(memo[n] != -1){
        return memo[n];
    }
    memo[n] = (fibMemoAux(n-1,memo) + fibMemoAux(n-2,memo));
    return memo[n];
}

int fibMemo(int n){
    int *memo = (int *)malloc((n+1) * sizeof(int));
    for(int i = 0; i <= n; i++){
        memo[i] = -1;
    }
    int result = fibMemoAux(n,memo);
    free(memo);
    return result; 
}

void test(){        
    int max = 20;                                               
    printf("\tn\t\tfib1(n)\t\tfib2(t)\t\tfib3(t)\n");                      
    for(int n=1;n<=max;n++){                                        
        printf("\t\t%d\t\t%d\t\t%d\t\t%d\n",n,fib1(n),fib2(n),fib3(n));
    }
}



void medicionesfib1(){
    double ta,tb,t1,t2, t,x,y,z;  
    int k = 10000;
    int n;
    int i;

    printf("\nfib1\n\tn\t\tt(n)\tt(n)/f(n)\tt(n)/g(n)\tt(n)/h(n)\n");
    for(n = 2; n <= 32; n=n*2){
        ta = microsegundos();
        fib1(n);
        tb = microsegundos();
        t = tb - ta;
        if(t < 500) {
            ta = microsegundos();
            for (i = 0; i < k; i++) {
                fib1(n);
            }
            tb = microsegundos();
            t1 = tb - ta;
            ta = microsegundos();
            for (i = 0; i < k; i++) {
            }
            tb = microsegundos();
            t2 = tb - ta;
            t = (t1 - t2) / k;
            x=t/pow(1.1,n);   
            y=t/pow((1+sqrt(5))/2,n);
            z=t/pow(2,n);  
            printf("%12d%15.5f*%15.6f%15.6f%15.8f\n", n, t, x, y, z);
        }else{
            x=t/pow(1.1,n);   
            y=t/pow((1+sqrt(5))/2,n);
            z=t/pow(2,n);  
            printf("%12d%15.5f%15.6f%15.6f%15.8f\n", n, t, x, y, z);
        }
    }
}

void medicionesfib2(){
    double ta,tb,t1,t2, t,x,y,z;  
    int k = 10000;
    int n;
    int i;

    printf("\nfib2\n\tn\t\tt(n)\tt(n)/f(n)\tt(n)/g(n)\tt(n)/h(n)\n");
    for(n = 1000; n <= 10000000; n=n*10){

        ta = microsegundos();
        fib2(n);
        tb = microsegundos();
        t = tb - ta;

        if(t < 500) {

            ta = microsegundos();

            for (i = 0; i < k; i++) {
                fib2(n);
            }

            tb = microsegundos();
            t1 = tb - ta;
            ta = microsegundos();

            for (i = 0; i < k; i++) {
            }

            tb = microsegundos();
            t2 = tb - ta;
            t = (t1 - t2) / k;
            x=t/pow(n,0.8);   
            y=t/n; 
            z=t/n*log(n); 
            printf("%12d%15.4f*%15.6f%15.6f%15.8f\n", n, t, x, y,z);
        }else{
            x=t/pow(n,0.8);   
            y=t/n; 
            z=t/n*log(n); 
            printf("%12d%15.4f%15.6f%15.6f%15.8f\n", n, t, x, y, z);
        }
    }
}


void medicionesfib3(){
    double ta,tb,t1,t2, t,x,y,z;  
    int k = 100000;
    int n;
    int i;

    printf("\nfib3\n\tn\t\tt(n)\tt(n)/f(n)\tt(n)/g(n)\tt(n)/h(n)\n");
    for(n = 1000; n <= 10000000; n=n*10){

        ta = microsegundos();
        fib3(n);
        tb = microsegundos();
        t = tb - ta;

        if(t < 500) {

            ta = microsegundos();

            for (i = 0; i < k; i++) {
                fib3(n);
            }

            tb = microsegundos();
            t1 = tb - ta;
            ta = microsegundos();

            for (i = 0; i < k; i++) {
            }

            tb = microsegundos();
            t2 = tb - ta;
            t = (t1 - t2) / k;
            x = t / sqrt(log(n));
            y = t / log(n);
            z = t / pow(n, 0.5); 
            printf("%12d%15.5f*%15.6f%15.6f%15.8f\n", n, t, x, y, z);
        }else{
            x = t / sqrt(log(n));
            y = t / log(n);
            z = t / pow(n, 0.5); 
            printf("%12d%15.5f%15.6f%15.6f%15.8f\n", n, t, x, y, z);
        }    
    } 

}

void medicionesfibBinet() {
    double ta, tb, t1, t2, t, x, y, z;
    int k = 100000; // Número de repeticiones para suavizar tiempos pequeños
    int n, i;

    printf("\nfibBinet\n\tn\t\tt(n)\tt(n)/f(n)\tt(n)/g(n)\tt(n)/h(n)\n");

    for (n = 1000; n <= 10000000; n=n*10) { // Valores de n a probar
        ta = microsegundos();
        fibBinet(n); // Cálculo del Fibonacci con fórmula de Binet
        tb = microsegundos();
        t = tb - ta;

        if (t < 500) { // Si el tiempo es muy pequeño, repetir para obtener mayor precisión
            ta = microsegundos();

            for (i = 0; i < k; i++) {
                fibBinet(n);
            }

            tb = microsegundos();
            t1 = tb - ta;

            ta = microsegundos();

            for (i = 0; i < k; i++) {
                // Bucle vacío para medir la sobrecarga
            }

            tb = microsegundos();
            t2 = tb - ta;

            t = (t1 - t2) / k; // Tiempo promedio
        }

        // Cálculos de proporciones
        x = t / pow(n, 0.1);
        y = t / (1 + sqrt(5)/2);
        z = t / pow(n, 0.5); 

        printf("%12d%15.5f%15.6f%15.6f%15.8f\n", n, t, x, y, z);
    }
}

void medicionesfibMemo() {
    double ta, tb, t1, t2, t, x, y, z;
    int k = 100000; // Número de repeticiones para suavizar tiempos pequeños
    int n, i;

    printf("\nfibBinet\n\tn\t\tt(n)\tt(n)/f(n)\tt(n)/g(n)\tt(n)/h(n)\n");

    for (n = 2; n <= 32; n=n*2) { // Valores de n a probar
        ta = microsegundos();
        fibMemo(n); // Cálculo del Fibonacci con fórmula de Binet
        tb = microsegundos();
        t = tb - ta;

        if (t < 500) { // Si el tiempo es muy pequeño, repetir para obtener mayor precisión
            ta = microsegundos();

            for (i = 0; i < k; i++) {
                fibMemo(n);
            }

            tb = microsegundos();
            t1 = tb - ta;

            ta = microsegundos();

            for (i = 0; i < k; i++) {
                // Bucle vacío para medir la sobrecarga
            }

            tb = microsegundos();
            t2 = tb - ta;

            t = (t1 - t2) / k; // Tiempo promedio
        }

        // Cálculos de proporciones
         x = t / pow(n,0.5);
            y = t / pow(n,0.875);
            z = t / pow(n, 1.2); 

        printf("%12d%15.5f%15.6f%15.6f%15.8f\n", n, t, x, y, z);
    }
}


int main(){                                                          
    test();                                                          
    medicionesfib1();      
    medicionesfib2();
    medicionesfib3();   
    medicionesfibBinet();
    medicionesfibMemo();
    return 0;
}

