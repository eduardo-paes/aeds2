#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Recebe valores necessários para criação, leitura, atualização 
// e remoção de uma matriz de adjacências de um grafo

void imprimeMatriz (int *matriz, int length) {
    int grauDoNo;
    for (int i = 0; i < length; i++) {
        grauDoNo = 0;
        printf("Nó %d: ", i);
        for (int j = 0; j < length; j++) {
            printf("%d ", *((matriz+i*length) + j)); 
            if (*((matriz+i*length) + j) != 0) {
                grauDoNo++;
            }
        }
        printf(" | Grau do nó: %d\n", grauDoNo);
    }
}

void matAdjGrafo () {
    int numVertices, numLinhas, end;
    
    scanf("%d %d %d", &numVertices, &numLinhas, &end);      // Recebe número de vértices e linhas do usuário
    int matriz [numVertices][numVertices];                  // Cria matriz correspondente
    memset(matriz, 0, sizeof matriz);                       // Zero todos os campos da matriz

    int aux = numLinhas;
    
    // imprimeMatriz((int *)matriz, numVertices);

    int valA, valB;

    // Lê relacionamento entre os vértices
    while(numLinhas) {
        scanf("%d %d", &valA, &valB);
        matriz[valA][valB] = 1;
        matriz[valB][valA] = 1;
        numLinhas--;
    }

    imprimeMatriz((int *)matriz, numVertices);
}