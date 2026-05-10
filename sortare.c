#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<direct.h>

long long memorie_extra_folosita = 0;
clock_t timp_start_algoritm;
double timeout_limita_secunde = 10.0;
int timeout_atins = 0; // 1 - a dat timeout, 0 - altfel

void swap(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}

// parcurge 
void BubbleSort(int v[], int n) {
    int i, j, schimbat;
    for(i = 0; i < n - 1; i++){

        if(((double)(clock() - timp_start_algoritm)) / CLOCKS_PER_SEC > timeout_limita_secunde){
            timeout_atins = 1;
            return; 
        }

        schimbat = 0;
        for(j = 0; j < n - i - 1; j++) {

            if(v[j] > v[j + 1]){
                swap(&v[j], &v[j + 1]);
                schimbat = 1;
            }
        }

        if(schimbat == 0)
           break;
    }
}


void SelectionSort(int v[], int n) {
    int i, j, index_min;
    for(i  = 0; i < n - 1; i++){

        if(((double)(clock() - timp_start_algoritm)) / CLOCKS_PER_SEC > timeout_limita_secunde){
            timeout_atins = 1;
            return; 
        }

        index_min = i;
        for(j = i + 1; j < n; j++){
            if(v[j] < v[index_min])
               index_min = j;
        }

        if(index_min != i)
           swap(&v[index_min], &v[i]);
    } 
}

void InsertionSort( int v[], int n) {
    int i, key, j;
    for(i = 1; i < n; i++){

        if(((double)(clock() - timp_start_algoritm)) / CLOCKS_PER_SEC > timeout_limita_secunde){
            timeout_atins = 1;
            return; 
        }

        key = v[i];
        j = i - 1;
        while(j >= 0 && v[j] > key){
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = key;
    }
}

//quick sort
int partition(int v[], int low, int high){
    int pivot = v[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++){
        if(v[j] < pivot) {
            i++;
            swap(&v[i], &v[j]);
        }
    }
    swap(&v[i + 1], &v[high]);
    return (i + 1);
}

void quickSort(int v[], int low, int high) {
    if (low < high) {
        int pi = partition(v, low, high);
        quickSort(v, low, pi - 1);  
        quickSort(v, pi + 1, high); 
    }
}

//merge sort, interclasare
void merge(int v[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    memorie_extra_folosita += (n1 + n2) * sizeof(int);

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    for (i = 0; i < n1; i++) L[i] = v[l + i];
    for (j = 0; j < n2; j++) R[j] = v[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            v[k] = L[i];
            i++;
        } else {
            v[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) { v[k] = L[i]; i++; k++; }
    while (j < n2) { v[k] = R[j]; j++; k++; }

    free(L);
    free(R);
}

void mergeSort(int v[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(v, l, m);
        mergeSort(v, m + 1, r);
        merge(v, l, m, r);
    }
}

void copiazaVector(int sursa[], int destinatie[], int n) {
    for (int i = 0; i < n; i++) {
        destinatie[i] = sursa[i];
    }
}

void SalveazaDateInFisier(int arr[], int n, const char* cale_folder, const char* nume_fisier){
    char cale_completa[200];
    sprintf(cale_completa, "%s/%s", cale_folder, nume_fisier);
    FILE *f = fopen(cale_completa, "w");
    if(f != NULL){
        for(int i = 0; i < n; i++){
            fprintf(f, "%d\n", arr[i]);
        }
        fclose(f);
    }
}

void afiseazaTimpDinamic(double secunde, char* buffer_rezultat){
    if(secunde >= 1.0) {
        sprintf(buffer_rezultat, "%.4f s", secunde);
    } else if(secunde >= 0.001) {
        sprintf(buffer_rezultat, "%.4f ms", secunde * 1000.0);
    } else if(secunde >= 0.000001){
       sprintf(buffer_rezultat, "%.4f us", secunde * 1000000.0); 
    } else {
        sprintf(buffer_rezultat, "%.4f ns", secunde * 1000000000.0);
    }
}

void genereazaAleator(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = rand() % 100000; 
    }
}

void genereazaSortat(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = i; 
    }
}

void genereazaDescrescator(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = n - i; 
    }
}

void genereazaAproapeSortat(int v[], int n) {
    genereazaSortat(v, n); 
    int nr_schimbari = n * 0.20; 
    for (int i = 0; i < nr_schimbari; i++) {
        int pos1 = rand() % n;
        int pos2 = rand() % n;
        swap(&v[pos1], &v[pos2]); 
    }
}

void genereazaPlate(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = rand() % 5; 
    }
}


int main() {
    srand(time(NULL));

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char nume_folder[100];

    sprintf(nume_folder, "Test_%d-%02d-%02d_%02d-%02d-%02d", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    _mkdir(nume_folder);     

    char cale_csv[150];
    sprintf(cale_csv, "%s/rezultate.csv", nume_folder);

    int dimensiuni[] = {10, 20, 50, 100, 1000, 5000, 10000, 50000, 100000, 500000, 1000000}; 
    int nr_dimensiuni =sizeof(dimensiuni) / sizeof(dimensiuni[0]);
    const char* nume_tipuri[] = {"Aleator", "Sortat", "Descrescator", "Aproape_Sortat", "Plat"};

    FILE *f_csv = fopen(cale_csv, "w"); // "w" pentru că e fișier nou în folder nou
    if(f_csv != NULL){
        fprintf(f_csv, "N,Tip_Lista,Algoritm,Timp_Sortare,Memorie_Extra_Bytes,Status\n");
    }

    
    for(int tip = 0; tip < 5; tip++) {
        
        for(int d = 0; d < nr_dimensiuni; d++) {
            int n = dimensiuni[d];

            int *original = (int *)malloc(n * sizeof(int));
            int *copie = (int *)malloc(n * sizeof(int));

            
            if (tip == 0) genereazaAleator(original, n);
            else if (tip == 1) genereazaSortat(original, n);
            else if (tip == 2) genereazaDescrescator(original, n);
            else if (tip == 3) genereazaAproapeSortat(original, n);
            else if (tip == 4) genereazaPlate(original, n);

            
            char nume_fisier[100];
            sprintf(nume_fisier, "date_test_%d_%s.txt", n, nume_tipuri[tip]);
            SalveazaDateInFisier(original, n, nume_folder, nume_fisier);

            printf("\n==============================================================\n");
            printf("Testam n = %d | Tip: %s (Timeout: %.0f sec)\n", n, nume_tipuri[tip], timeout_limita_secunde);
            printf("==============================================================\n");

            clock_t start, end;
            double timp_total, timp_mediu;
            char buffer_timp[50];
            int r;

            // ---  BUBBLE SORT ---
            memorie_extra_folosita = 0; timeout_atins = 0; timp_start_algoritm = clock();
            start = clock(); r = 0;
            do {
                copiazaVector(original, copie, n); BubbleSort(copie, n); r++;
                end = clock();
                timp_total = ((double)(end - start)) / CLOCKS_PER_SEC;
            } while (timp_total < 0.1 && !timeout_atins); 
            
            timp_mediu = timp_total / r; 
            afiseazaTimpDinamic(timp_mediu, buffer_timp);
            if (timeout_atins) strcpy(buffer_timp, "TIMEOUT");
            printf("Bubble Sort:    Timp: %-15s | Repetari: %-6d | Mem: %lld bytes\n", buffer_timp, r, memorie_extra_folosita);
            if (f_csv) { fprintf(f_csv, "%d,%s,Bubble Sort,%s,%lld,%s\n", n, nume_tipuri[tip], buffer_timp, memorie_extra_folosita, timeout_atins ? "TIMEOUT" : "OK"); fflush(f_csv); }


            // ---  SELECTION SORT ---
            memorie_extra_folosita = 0; timeout_atins = 0; timp_start_algoritm = clock();
            start = clock(); r = 0;
            do {
                copiazaVector(original, copie, n); SelectionSort(copie, n); r++;
                end = clock();
                timp_total = ((double)(end - start)) / CLOCKS_PER_SEC;
            } while (timp_total < 0.1 && !timeout_atins);
            
            timp_mediu = timp_total / r; 
            afiseazaTimpDinamic(timp_mediu, buffer_timp);
            if (timeout_atins) strcpy(buffer_timp, "TIMEOUT");
            printf("Selection Sort: Timp: %-15s | Repetari: %-6d | Mem: %lld bytes\n", buffer_timp, r, memorie_extra_folosita);
            if (f_csv) { fprintf(f_csv, "%d,%s,Selection Sort,%s,%lld,%s\n", n, nume_tipuri[tip], buffer_timp, memorie_extra_folosita, timeout_atins ? "TIMEOUT" : "OK"); fflush(f_csv); }

            // --- INSERTION SORT ---
            memorie_extra_folosita = 0; timeout_atins = 0; timp_start_algoritm = clock();
            start = clock(); r = 0;
            do {
                copiazaVector(original, copie, n); InsertionSort(copie, n); r++;
                end = clock();
                timp_total = ((double)(end - start)) / CLOCKS_PER_SEC;
            } while (timp_total < 0.1 && !timeout_atins);
            
            timp_mediu = timp_total / r; 
            afiseazaTimpDinamic(timp_mediu, buffer_timp);
            if (timeout_atins) strcpy(buffer_timp, "TIMEOUT");
            printf("Insertion Sort: Timp: %-15s | Repetari: %-6d | Mem: %lld bytes\n", buffer_timp, r, memorie_extra_folosita);
            if (f_csv) { fprintf(f_csv, "%d,%s,Insertion Sort,%s,%lld,%s\n", n, nume_tipuri[tip], buffer_timp, memorie_extra_folosita, timeout_atins ? "TIMEOUT" : "OK"); fflush(f_csv); }

            // ---  QUICK SORT ---
            // Protecție împotriva Stack Overflow pentru liste mari gata ordonate
            if ((tip == 1 || tip == 2 || tip == 3 || tip == 4) && n > 5000) {
                printf("Quick Sort:     Timp: SKIPPED (Crash Risk)| Repetari: 0      | Mem: 0 bytes\n");
                if (f_csv) { fprintf(f_csv, "%d,%s,Quick Sort,SKIPPED,0,CRASH_PREVENTED\n", n, nume_tipuri[tip]); fflush(f_csv); }
            } else {
                memorie_extra_folosita = 0; timeout_atins = 0; timp_start_algoritm = clock();
                start = clock(); r = 0;
                do {
                    copiazaVector(original, copie, n); quickSort(copie, 0, n - 1); r++;
                    end = clock();
                    timp_total = ((double)(end - start)) / CLOCKS_PER_SEC;
                } while (timp_total < 0.1);
                
                timp_mediu = timp_total / r; 
                afiseazaTimpDinamic(timp_mediu, buffer_timp);
                printf("Quick Sort:     Timp: %-15s | Repetari: %-6d | Mem: O(log N) stack\n", buffer_timp, r);
                if (f_csv) { fprintf(f_csv, "%d,%s,Quick Sort,%s,Stack,OK\n", n, nume_tipuri[tip], buffer_timp); fflush(f_csv); }
            }

            // ---  MERGE SORT ---
            memorie_extra_folosita = 0; timeout_atins = 0; timp_start_algoritm = clock();
            start = clock(); r = 0;
            do {
                copiazaVector(original, copie, n); mergeSort(copie, 0, n - 1); r++;
                end = clock();
                timp_total = ((double)(end - start)) / CLOCKS_PER_SEC;
            } while (timp_total < 0.1);
            
            timp_mediu = timp_total / r; 
            afiseazaTimpDinamic(timp_mediu, buffer_timp);
            printf("Merge Sort:     Timp: %-15s | Repetari: %-6d | Mem: %lld bytes\n", buffer_timp, r, memorie_extra_folosita / r);
            if (f_csv) { fprintf(f_csv, "%d,%s,Merge Sort,%s,%lld,OK\n", n, nume_tipuri[tip], buffer_timp, memorie_extra_folosita / r); fflush(f_csv); }
            
            
            free(original);
            free(copie);
        } 
    } 

    if (f_csv) fclose(f_csv);
    printf("\n>>> EXPERIMENT COMPLET! Verifica fisierele txt si rezultate.csv <<<\n");

    return 0;
}

    