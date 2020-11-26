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
    return tmp;
}

// Retorna o pai do filho que receberá o novo nó
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
                // Pai de esquerda NULL
                return p;
            }
        }
        // Verifica Direita
        else {
            if (p->dir != NULL) {
                buscar(p->dir, info);
            } else {
                // Pai de direita NULL
                return p;
            }
        }
    }
}

// Retorna a altura de aux
int calcular_altura (node aux) {

    // Verifica se aux é folha
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

// Rotações
void rot_RR (node d) {
    node aux, tmp;

    aux = d->esq;
    tmp = d->pai;

    // Troca o endereço do pai para o filho de d para d->esq
    if (tmp->esq->info == d->info) {
        tmp->esq = aux;
    } else {
        tmp->dir == aux;
    }

    // Atualiza, se houver, o pai do filho da direita de aux
    if (aux->dir != NULL) {
        aux->dir->pai = d;
    }

    // Torna d filho da esquerda de aux
    d->esq = aux->dir;
    aux->dir = d;
    aux->pai = d->pai;
    d->pai = aux;
    d = aux;

    d->fb = calcular_altura(aux->esq) - calcular_altura(aux->dir);
}

void rot_LL (node d) {
    node aux, tmp;

    aux = d->dir;
    tmp = d->pai;

    // Troca o endereço do pai para o filho de d para d->dir
    if (tmp->esq->info == d->info) {
        tmp->esq = aux;
    } else {
        tmp->dir == aux;
    }

    // Atualiza, se houver, o pai do filho da esquerda de aux
    if (aux->esq != NULL) {
        aux->esq->pai = d;
    }

    // Torna d filho da direita de aux
    d->dir = aux->esq;
    aux->esq = d;
    aux->pai = d->pai;
    d->pai = aux;
    d = aux;

    d->fb = calcular_altura(aux->esq) - calcular_altura(aux->dir);
}

// Balanceamento
void balancear (node aux) {
    if (aux->fb == 2) {
        if (aux->esq->fb == -1) {
            rot_LL(aux->esq);
            rot_RR(aux);
        } else {
            rot_RR(aux);
        }
    } else if (aux->fb == -2) {
        if (aux->dir->fb == 1) {
            rot_RR(aux->dir);
            rot_LL(aux);
        } else {
            rot_LL(aux);
        }
    }

    // Atualiza o FB de aux após rotações
    aux->fb = calcular_altura(aux->esq) - calcular_altura(aux->dir);
}

// Atualização do FB
void atualizar_fb (node aux) {
    if (aux->fb == 2 || aux->fb == -2) {
        balancear(aux);
    }

    // Se um dos filhos for nulo, a árvore aumentou de altura
    // Portanto, será preciso atualizar fb do avô
    else if (aux->esq == NULL || aux->dir == NULL) {
        // Verifica se nó corrente é a raiz da árvore
        if (aux->pai != NULL) {

            // Verifica se a árvore aumentou à esquerda
            if (aux == aux->pai->esq) {
                // Incrementa FB do avô
                aux->pai->fb++;
            }
            // Verifica se a árvore aumentou à direita
            else {
                // Decrementa FB do avô
                aux->pai->fb--;
            }

            // Chama função para verificar se o avô necessita de balanceamento
            atualizar_fb(aux->pai);
        }
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
                // Incrementa FB do pai
                aux->fb++;
            } else {
                aux->dir = tmp;
                // Decrementa FB do pai
                aux->fb--;
            }
            // printf("Filho: %d Pai: %d FB %d\n", tmp->info, aux->info, aux->fb);
            atualizar_fb(aux);
        }
    }
    return p;
}

// Procura última posição para remoção
node antecessor (node p) {
    if (p->dir != NULL) {
        antecessor(p->dir);
    } else {
        return p;
    }
}

node sucessor (node p) {
    if (p->esq != NULL) {
        antecessor(p->esq);
    } else {
        return p;
    }
}

// Remoção
node remover (node p, int info) {
    node aux, tmp;

    // Encontra a posição que será removida
    tmp = buscar(p, info);
    if (tmp == NULL) {
        printf("Valor não contido na árvore!\n");
    } else {
        // Se a posição estiver contida na árvore, identifica a qual filho pertence
        if (tmp->esq->info == info) {
            aux = tmp->esq;
        } else {
            aux = tmp->dir;
        }

        // Verifica se a posição que será removida é uma folha
        if (aux->esq == NULL && aux->dir == NULL) {
            // Se é uma folha, remove normalmente atualizando FB
            tmp = aux->pai;
            if (tmp->esq == aux) {
                tmp->esq == NULL;
                tmp->fb--;
            } else {
                tmp->dir == NULL;
                tmp->fb++;
            }
            free(aux);
        } else {
            // Encontra o antecessor ou sucessor do nó que será removido
            tmp = antecessor(aux->esq);
            if (tmp == NULL) {
                tmp = sucessor(p->dir);
            }

            // Faz a troca de aux (valor que será removido) por tmp (antecessor ou sucessor)
            aux->info = tmp->info;
            info = tmp->info;

            node meio = aux;

            // Encontra o pai do valor movido (tmp), faz apontar para NULL e remove o nó
            aux = buscar(tmp, info);
            if (aux->esq == tmp) {
                aux->esq == NULL;
            } else {
                aux->dir == NULL;
            }
            free(tmp);

            // Revisa os valores de FB
            aux->fb = calcular_altura(aux->esq) - calcular_altura(aux->dir);
            atualizar_fb(aux);
            meio->fb = calcular_altura(meio->esq) - calcular_altura(meio->dir);
            atualizar_fb(meio);
        }
    }
    return p;
}

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
        if (p->esq) imprimir_fb(p->esq);
        if (p->dir) imprimir_fb(p->dir);
    }
}

void main (){
    node p = NULL;
    int opt = 3, k, val;

    while (opt != 0) {
        printf("Comandos:\n 0- Sair;\n 1- Inserção;\n 2- Remoção;\n =====================\n O que deseja fazer?\n");
        scanf("%d", &opt);

        #pragma region => Comando de Inserção
        if (opt == 1) {
            printf("\nQuantos valores serão inseridos? ");
            scanf("%d", &k);
            while (k--) {
                scanf("%d", &val);
                p = inserir(p, val);
                
                printf("Lista de Valores:\n");
                imprimir(p);

                printf("\nValores de FB:\n");
                imprimir_fb(p);
            }
        }
        #pragma endregion
        
        #pragma region => Comando de Remoção
        if (opt == 2) {
            printf("\nQual valor deseja remover? ");
            scanf("%d", &val);
            p = remover(p, val);
            // Impressão
            printf("Lista de Valores:\n");
            imprimir(p);
        }
        #pragma endregion

        #pragma region => Comandos de Impressão 
        printf("Lista de Valores:\n");
        imprimir(p);

        printf("\nValores de FB:\n");
        imprimir_fb(p);
        #pragma endregion
    }
}
