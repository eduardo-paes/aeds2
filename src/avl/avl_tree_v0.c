#include "function.h"

void main (){
    node p = NULL;
    int k, val;

    k = 5;

    while (k--) {
        scanf("%d", &val);
        p = inserir(p, val);
    }

    printf("Lista de Valores:\n");
    imprimir(p);

    scanf("%d", &val);
    remover(p, val);

    printf("\nLista de Valores após Remoção:\n");
    imprimir(p);
}
