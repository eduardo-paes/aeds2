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
        // Verifica se está contido na árvore
        if (p->info == info) {
            return p;
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

// Corrige o FB após alguma operação
int atualizar_fb (node aux) {
    return calcular_altura(aux->esq) - calcular_altura(aux->dir);
}

// Rotações
void rot_RR (node p) {
    node filho = p->esq;

    // Se houver pai, faz esse apontar para d->esq
    if (p->pai) {
        node pai = p->pai;
        // Faz o pai apontar para o filho correto
        if (pai->esq == p) {
            pai->esq = filho;
        } else {
            pai->dir = filho;
        }
    }

    // Se houver filho à direita, atualiza o pai para d
    if (filho->dir) {
        filho->dir->pai = p;
    }

    // Torna d filho da esquerda de aux
    p->esq = filho->dir;
    filho->dir = p;
    filho->pai = p->pai;
    p->pai = filho;
    p = filho;

    // Atualiza o FB de p após rotação
    p->fb = atualizar_fb(p);
    p->dir->fb = atualizar_fb(p->dir);
}

void rot_LL (node p) {
    node filho = p->dir;

    // Se houver pai, faz esse apontar para d->dir
    if (p->pai) {
        node pai = p->pai;
        // Faz o pai apontar para o filho correto
        if (pai->esq == p) {
            pai->esq = filho;
        } else {
            pai->dir = filho;
        }
    }

    // Atualiza, se houver, o pai do filho da esquerda de aux
    if (filho->esq) {
        filho->esq->pai = p;
    }

    // Torna d filho da direita de aux
    p->dir = filho->esq;
    filho->esq = p;
    filho->pai = p->pai;
    p->pai = filho;
    p = filho;

    // Atualiza o FB de p após rotação
    p->fb = atualizar_fb(p);
    p->esq->fb = atualizar_fb(p->esq);
}

// Atualização do FB e Balanceamento
void balancear (node aux) {

    // Verifica e realiza o balanceamento
    if (aux->fb == 2 || aux->fb == -2) {
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
        aux->fb = atualizar_fb(aux);
    }

    // Verifica se nó corrente é a raiz da árvore
    if (aux->pai) {
        // Atualiza FB do pai e verifica o balanceamento
        aux->pai->fb = atualizar_fb(aux->pai);
        balancear(aux->pai);
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
        // Retorna o pai da posição ideal para o valor
        node pai = buscar(p, info);

        // Retorna a posição do nó, se o valor já estiver inserido
        if (pai->info == info) {
            printf("Valor já inserido!\n");
        }

        // Faz o filho apontar para o novo pai
        else {
            tmp->pai = pai;

            // Verifica em qual dos lados do pai deve ser inserido
            if (pai->info > info) {
                pai->esq = tmp;
                // Incrementa FB do pai
                pai->fb++;
            } else {
                pai->dir = tmp;
                // Decrementa FB do pai
                pai->fb--;
            }
            balancear(pai);
        }
    }
    while (p->pai) {
        p = p->pai;
    }
    return p;
}

// ================================================

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
void remover (node p, int info) {
    // Encontra a posição que será removida
    node tmp = buscar(p, info);
    if (tmp->info != info) {
        printf("Valor não contido na árvore!\n");
    }

    // Se a posição estiver contida na árvore, identifica a qual filho pertence
    else {
        // AUX é o nó que que deve ser removido
        // TMP é o sucessor ou antecessor de AUX
        node aux = tmp;

        // Verifica se a posição que será removida é uma folha
        if (tmp->esq == NULL && tmp->dir == NULL) {

            // Se é uma folha, remove normalmente atualizando FB
            tmp = aux->pai;
            if (tmp->esq == aux) {
                tmp->fb--;
            } else {
                tmp->fb++;
            }
            aux = NULL;
            free(aux);
        }

        // Caso não seja um nó folha
        else {
            // Encontra o antecessor ou sucessor do nó que será removido
            if (tmp->dir != NULL) {
                tmp = antecessor(tmp->esq);
            } else {
                tmp = sucessor(tmp->dir);
            }

            // Faz a troca de aux (valor que será removido) por tmp (antecessor ou sucessor)
            aux->info = tmp->info;

            // Guarda posição do no que será removido para fins de balanceamento
            node meio = aux;

            // AUX recebe o TMP e TMP recebe seu pai
            aux = tmp;
            tmp = tmp->pai;

            // Verifica qual filho é o que será removido, fax TMP apontar para NULL e remove
            if (tmp->esq == aux) {
                tmp->esq = NULL;
            } else {
                tmp->dir = NULL;
            }
            aux = NULL;
            free(aux);

            // Revisa os valores de FB
            tmp->fb = atualizar_fb(tmp);
            meio->fb = atualizar_fb(meio);
            balancear(meio);
        }
    }
}

// Impressões
node imprimir (node p) {
    if (p != NULL) {
        printf("Nó: %d | FB: %d\n", p->info, p->fb);
        if (p->esq) imprimir(p->esq);
        if (p->dir) imprimir(p->dir);
    }
}
