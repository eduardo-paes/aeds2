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

#pragma region Balanceamento
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

// Corrige o FB após cálculo de altura dos ramos
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
#pragma endregion

#pragma region Inserção
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
#pragma endregion

#pragma region Remoção
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

void remover (node p, int info) {
    // Encontra a posição que será removida
    node tmp, aux = buscar(p, info);
    if (aux->info != info) {
        printf("Valor não contido na árvore!\n");
    } else {
        // Folha
        if (aux->esq == NULL && aux->dir == NULL) {
            tmp = aux->pai;
            if (tmp->esq == aux) {
                tmp->esq = NULL;
                tmp->fb--;
            } else {
                tmp->dir = NULL;
                tmp->fb++;
            }
            aux = NULL;
            free(aux);
        }

        // // Existindo nó à direita
        // else if (aux->esq == NULL) {
        //     tmp = aux->pai;
        //     // Substitui ponteiro do pai para o filho existente
        //     if (tmp->dir == aux) {
        //         tmp->dir = aux->dir;
        //     } else {
        //         tmp->esq = aux->dir;
        //     }
        //     // Remove aux e atualiza o FB de tmp
        //     free(aux);
        //     tmp->fb = atualizar_fb(tmp);
        //     balancear(tmp);
        // }
        //
        // // Existindo nó à esquerda
        // else if (aux->dir == NULL) {
        //     tmp = aux->pai;
        //     // Substitui ponteiro do pai para o filho existente
        //     if (tmp->dir == aux) {
        //         tmp->dir = aux->esq;
        //     } else {
        //         tmp->esq = aux->esq;
        //     }
        //     // Remove aux e atualiza o FB de tmp
        //     free(aux);
        //     tmp->fb = atualizar_fb(tmp);
        //     balancear(tmp);
        // }

        // Caso não seja um nó folha
        else {
            // TMP recebe o antecessor ou sucessor do nó a ser removido
            if (aux->esq != NULL) {
                tmp = antecessor(aux->esq);
            } else {
                tmp = sucessor(aux->dir);
            }


            // Troca as informações entre o nó que será removido e seu sucessor/antecessor
            aux->info = tmp->info;

            // Guarda posição do nó que será removido para fins de balanceamento
            node meio = aux;

            // Caso um dos filhos do nó que será removido exista
            if (tmp->esq != NULL || tmp->dir != NULL) {
                node filho;

                if (tmp->esq != NULL) {
                    filho = tmp->esq;
                } else {
                    filho = tmp->dir;
                }

                // AUX = PAI
                // TMP = Nó que será removido
                // FILHO = FILHO

                if (aux->esq == tmp) {
                    aux->esq = filho;
                } else {
                    aux->dir = filho;
                }
                filho->pai = aux;
                aux = tmp;
            } else {
                // AUX recebe o TMP e TMP recebe seu pai
                aux = tmp;
                tmp = tmp->pai;

                // Verifica qual filho é o que será removido, fax TMP apontar para NULL e remove
                if (tmp->esq == aux) {
                    tmp->esq = NULL;
                } else {
                    tmp->dir = NULL;
                }
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
#pragma endregion

// Impressões
node imprimir (node p) {
    if (p != NULL) {
        printf("Nó: %d | FB: %d\n", p->info, p->fb);
        if (p->esq) imprimir(p->esq);
        if (p->dir) imprimir(p->dir);
    }
}
