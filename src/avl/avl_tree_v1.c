#include <stdio.h>
#include <stdlib.h>

struct noAVL {
    int info;
    int fb;
    struct noAVL *pai;
    struct noAVL *esq;
    struct noAVL *dir;
};

typedef struct noAVL *node;

node alocar () {
    node tmp = calloc(20, sizeof(node));
    tmp->pai = tmp;
    return tmp;
}

node buscar (node p, int info) {
    if (p != NULL) {
        if (p->info == info) {
            return NULL;
        }
        // Verifica Esquerda
        else if (p->info > info) {
            if (p->esq != NULL) {
                buscar(p->esq, info);
            } else {
                return p;
            }
        }
        // Verifica Direita
        else {
            if (p->dir != NULL) {
                buscar(p->dir, info);
            } else {
                return p;
            }
        }
    }
}

// Rotações
void rot_RR (node d) {
    node aux;

    aux = d->esq;
    if (aux->dir != NULL) {
        aux->dir->pai = d;
    }
    d->esq = aux->dir;
    aux->dir = d;
    aux->pai = d->pai;
    d->pai = aux;
    d = aux;
}

void rot_LL (node d) {
    node aux;

    aux = d->dir;
    if (aux->esq != NULL) {
        aux->esq->pai = d;
    }
    d->dir = aux->esq;
    aux->esq = d;
    aux->pai = d->pai;
    d->pai = aux;
    d = aux;
}

void rot_RL (node d) {
    rot_RR(d->dir);
    rot_LL(d);
}

void rot_LR (node d) {
    rot_LL(d->esq);
    rot_RR(d);
}

// Balanceamento
void balancear (node aux) {
    if (aux->fb == 2) {
        if (aux->esq->fb == -1) {
            rot_LR(aux);
        } else {
            rot_RR(aux);
        }
    }

    else if (aux->fb == -2) {
        if (aux->dir->fb == 1) {
            rot_RL(aux);
        } else {
            rot_LL(aux);
        }
    }
}

// Atualização do FB
int calcular_altura (node aux) {

    if (aux == NULL) {
        return 0;
    }

    int esq = calcular_altura (aux->esq);
    int dir = calcular_altura (aux->dir);

    if (esq > dir) {
        return esq + 1;
    } else {
        return dir + 1;
    }
}

void atualizar_fb (node aux) {

    if (aux->fb == 2 || aux->fb == -2) {
        balancear(aux);
    }

    else if (aux->esq == NULL || aux->dir == NULL) {
        if (aux->esq == NULL) {
            aux->pai--;
        } else {
            aux->pai++;
        }
        atualizar_fb(aux->pai);
    }

    int esq = calcular_altura(aux->esq);
    int dir = calcular_altura(aux->dir);
    int fb;

    if (aux->esq != NULL && aux->dir != NULL) {
        fb = esq - dir;
    }

    else if (aux->dir == NULL) {
        fb = esq;
    }

    else if (aux->esq == NULL) {
        fb = -dir;
    }

    aux->fb = fb;

    if (fb >= 2 || fb <= -2) {
        balancear (aux);
    }

    printf("fb: %d\n", aux->fb);

    if (aux->pai != aux) {
        atualizar_fb(aux->pai);
    }

}

// Inserção
node inserir (node p, int info) {
    node tmp = alocar();
    tmp->info = info;

    // Se nó criado for o primeiro
    if (p == NULL) {
        p = tmp;
    }

    // Se nó criado não for o primeiro
    else {
        node aux = buscar(p, info);

        // Retorna NULL se o valor já estiver inserido
        if (aux == NULL) {
            printf("Valor já inserido!\n");
        }

        // Retorna a posição ideal para o valor
        else {
            tmp->pai = aux;

            // Verifica em qual dos lados deve ser inserido
            if (aux->info > info) {
                aux->esq = tmp;
                aux->fb++;
            } else {
                aux->dir = tmp;
                aux->fb--;
            }
            atualizar_fb(aux);
        }
    }
    return p;
}

// Remoção
// Antecessor: maior valor entre os menores que o nó
// Sucessor: menor valor entre os maiores que o nó
// Retorna calculando o Fator de Balanceamento

// Impressões
node imprimir (node p) {
    if (p != NULL) {
        printf("%d\n", p->info);
        if (p->esq) imprimir(p->esq);
        if (p->dir) imprimir(p->dir);
    }
}

node imprimir_fb (node p) {
    if (p != NULL) {
        printf("%d\n", p->fb);
        if (p->esq) imprimir(p->esq);
        if (p->dir) imprimir(p->dir);
    }
}

void main (){
    node p = NULL;
    int k, val;

    k = 5;
    while (k--) {
        scanf("%d", &val);
        p = inserir(p, val);
    }

    printf("Lista de Valores: \n");
    imprimir(p);

    printf("Valores de FB: \n");
    imprimir_fb(p);

}
