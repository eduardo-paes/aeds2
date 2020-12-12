#include <stdio.h>

// Bottom_Up

void troca(int vet[], int i, int j) {
    int tmp = vet[i];
    vet[i] = vet[j];
    vet[j] = tmp;
}

void sobe_heap (int aux[], int i) {
    int tmp = aux[i];
    while (i > 0 && aux[(i-1)/2] < tmp) {
        aux[i] = aux[(i-1)/2];
        i = (i-1)/2;
    }
    aux[i] = tmp;
}

void desce_heap (int aux[], int pai, int n) {
    int fe = 2*pai+1;
    int fd = 2*pai+2;
    int filho;

    while (fe <= n || fd <= n) {
        if (fe > n) filho = fd;
        else if (fd > n) filho = fe;
        else {
            if (aux[fd] >= aux[fe]) filho = fd;
            else filho = fe;
        }
        if (aux[pai] < aux[filho]) {
            troca(aux, pai, filho);
            pai = filho;
            fe = 2*pai+1;
            fd = 2*pai+2;
        }
        else break;
    }
}

void build_min_heap (int vet[], int aux[], int n) {
    for (int i = 0; i <= n; i++) {
        aux[i] = vet[i];
        sobe_heap(aux, i);
    }
}

void heapsort (int vet[], int n){
    int tam = n;
    int aux[n];
    build_min_heap(vet, aux, n);
    for (int i = n; i >= 1; --i) {
        troca(aux, 0, i);
        --tam;
        desce_heap (aux, 0, tam);
    }
    for (int i = 0; i <= n; i++) {
        vet[i] = aux[i];
        printf("%d ", aux[i]);
    }
    puts("");
}

void main (){
    int num;
    scanf("%d", &num);
    int vet[num];
    for (int i = 0; i < num; i++) scanf("%d", &vet[i]);
    heapsort(vet, num-1);
}
