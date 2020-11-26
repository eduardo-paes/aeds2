#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lista encadeada contendo as palavras referente a cada letra capitular
struct linkedList {
    int val;
    struct linkedList *prox; // Ponteiro ocupa 8 bytes
};

typedef struct linkedList *lista;

// Aloca valor na memória
lista alocarValor (int valor){
    lista tmp = calloc(16, sizeof(lista));
    tmp->val = valor;
    return tmp;
}

void listAdjGrafo () {
    lista grafo;


}