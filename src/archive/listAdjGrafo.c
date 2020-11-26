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

void inserirNaLista (lista *grafo, int valA, int valB) {
    lista aux = grafo[valA];
    lista tmp = alocarValor(valB);
    lista ant = NULL;
    
    printf("%d", tmp->val);

    if (aux->prox == NULL) {
        aux->prox = tmp;
    } else {
        int flag = 1;
        ant = aux;

        while (aux->prox != NULL) {
            if (aux->val < valB) {
                ant = aux;
                aux = aux->prox;
            } else {
                tmp->prox = aux;
                ant->prox = tmp;
                flag = 0;
                break;
            }
        }
        
        printf("%d", ant->val);

        if (flag) {
            aux->prox = tmp;
        }
    }
}

void imprimirLista (lista *grafo, int numVertices) {
    lista aux;
    for (int i = 0; i < numVertices; i++) {
        printf("Vértice %d:", i);
        aux = grafo[i];
        while (aux != NULL) {
            printf(" %d", aux->val);
            aux = aux->prox;
        }
        printf("\n");
    }
}

void main () {
    int numVertices, numLinhas, end;                        // Declarações iniciais
    scanf("%d %d %d", &numVertices, &numLinhas, &end);      // Recebe número de vértices e linhas do usuário
    lista grafo[numVertices];                               // Cria lista conforme número de vértices fornecido
    int valA, valB;                                         // Valores correspondentes a cada linha

    while (numLinhas) {
        scanf("%d %d", &valA, &valB);
        inserirNaLista(grafo, valA, valB);
        --numLinhas;
    }

    imprimirLista(grafo, numVertices);                      // Imprime valores da lista de adjacências
}