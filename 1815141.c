#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// =============================================================================
// -- ESTRUTURAS

// Lista encadeada contendo as palavras referente a cada letra capitular
struct linkedList {
    char palavra[30];
    struct linkedList *prox; // Ponteiro ocupa 8 bytes
};

typedef struct linkedList *lista;

// Nós da árvore com sua respectiva letra
struct noARN {
    char letra;
    char cor; // Preto: false; Vermelho: true;
    lista conjunto; // Ponteiro para o conjuto de termos inciando com aquela letra;

    // Ponteiros para Pai e Filhos
    struct noARN *pai;
    struct noARN *esq;
    struct noARN *dir;
};

typedef struct noARN *node;

// =============================================================================
// -- ALOCAÇÕES

// Funçào de alocação para letras
node alocar_letra (char letra) {
    node tmp = calloc(34, sizeof(node));
    tmp->letra = letra;
    tmp->cor = 'v';
    return tmp;
}

// Funçào de alocação para palavras
lista alocar_palavra (char palavra[]) {
    lista tmp = calloc(38, sizeof(node));
    strcpy (tmp->palavra, palavra);
    return tmp;
}

// =============================================================================
// -- PESQUISAS E FUNÇÕES EXTRAS

// Impressão do Menu para o Usuário
void menu () {
    printf("\n*** MENU DE OPÇÕES: ENTRE COM A OPÇÃO DESEJADA ***\n\n");
    printf("1. Pesquisa\n");
    printf("2. Inserção\n");
    printf("3. Remoção\n");
    printf("4. Impressão de um nó\n");
    printf("5. Impressão da árvore\n");
    printf("6. Sair\n\n");
}

// Impressão da cor do nó
void mostra_cor (node aux) {
    if (aux->cor == 'v') printf("vermelho ");
    else printf("preto ");
}

// Impressão do nível do nó
void calcular_nivel (node aux) {
    int nivel = 1;

    // Enquanto nó AUX possuir nó anterior
    while (aux->pai) {
        // Incrementa contágem de nível
        nivel++;
        // E nó AUX recebe nó anterior
        aux = aux->pai;
    }
    // Se nó não possuir anterior, no caso do nó raiz, o valor do nível será 1
    printf("nível %d", nivel);
}

// Retorna o pai do filho que receberá o novo nó, caso letra já esteja inserida, retorna o próprio nó
node buscar_letra (node aux, char letra) {
    node pai = NULL;
    while (aux != NULL) {
        pai = aux;
        // Letra do nó maior que letra lida
        if (aux->letra > letra) {
            aux = aux->esq;
        }
        // Letra do nó menor que letra lida
        else if (aux->letra < letra) {
            aux = aux->dir;
        }
        // Letra do nó igual a letra lida
        else {
            return aux;
        }
    }
    return pai;
}

// Retorna o anterior na lista para inserção da palavra, caso palavra já esteja inserida, retorna o próprio nó
lista buscar_palavra (lista aux, char palavra[]) {
    lista tmp = aux;
    while (aux != NULL) {
        // Retorna NULL se a palavra já estiver contida na lista
        if (strcmp (aux->palavra, palavra) == 0) {
            return aux;
        }
        // Para o loop caso a palavra na lista seja maior do que a palavra que deve ser inserida
        else if (strcmp (aux->palavra, palavra) > 0) {
            break;
        }
        // Atualiza o ponteiro para verificar próxima palavra da lista
        else if (strcmp (aux->palavra, palavra) < 0) {
            tmp = aux;
            aux = aux->prox;
        }
    }
    // Retorna posição anterior ao nó onde nova palavra deve ser inserida
    return tmp;
}

// Função de Pesquisa da Main
void pesquisar (node p, char palavra[]) {

    // Encontra o NÓ onde a PALAVRA deve estar contida
    node aux = buscar_letra(p, palavra[0]);

    // Caso haja o NÓ da respectiva PALAVRA
    if (aux->letra == palavra[0]) {
        // Verifica se a palavra está contida
        lista tmp = buscar_palavra(aux->conjunto, palavra);

        // Caso a PALAVRA seja encontrada
        if (strcmp(tmp->palavra, palavra) == 0) {
            // Informa detalhes da PALAVRA
            printf("A Palavra '%s' foi encontrada no NÓ '%c' ", palavra, palavra[0]);

            // Verifica a cor do nó
            mostra_cor(aux);

            // Calcula o nível do NÓ
            calcular_nivel(aux);
            puts("");
        }

        // Caso a PALAVRA não esteja no NÓ
        else {
            printf("Palavra Inexistente!\n");
        }
    }

    // Caso NÃO haja o NÓ da respectiva PALAVRA
    else {
        printf("Palavra Inexistente!\n");
    }
}

// =============================================================================
// -- FUNÇÕES DE CORREÇÃO

// Rotação à Direita
void rot_RR (node p) {
    node filho = p->esq;

    // Se houver pai, faz esse apontar para p->esq
    if (p->pai) {
        node pai = p->pai;
        // Faz o pai apontar para o filho correto
        if (pai->esq == p) {
            pai->esq = filho;
        } else {
            pai->dir = filho;
        }
    }

    // Se houver filho à direita, atualiza o pai para p
    if (filho->dir) {
        filho->dir->pai = p;
    }

    // Torna p filho da esquerda de Filho
    p->esq = filho->dir;
    filho->dir = p;
    filho->pai = p->pai;
    p->pai = filho;
    p = filho;
}

// Rotação à Esquerda
void rot_LL (node p) {
    node filho = p->dir;

    // Se houver pai, faz esse apontar para p->dir
    if (p->pai) {
        node pai = p->pai;
        // Faz o pai apontar para o filho correto
        if (pai->esq == p) {
            pai->esq = filho;
        } else {
            pai->dir = filho;
        }
    }

    // Atualiza, se houver, o pai do filho da esquerda de Filho
    if (filho->esq) {
        filho->esq->pai = p;
    }

    // Torna p filho da direita de Filho
    p->dir = filho->esq;
    filho->esq = p;
    filho->pai = p->pai;
    p->pai = filho;
    p = filho;
}

// Realiza a correção da coloração dos nós e suas posições
node corrigir_insercao (node filho) {

    // Se Pai de Filho é preto, não precisa fazer nada
    // Se for Vermelho, entra no Loop para correção
    while (filho->pai != NULL && filho->pai->cor == 'v') {

        // Ponteiros para o Avô, Pai e Tio
        node avo = filho->pai->pai;
        node pai = filho->pai;
        node tio;

        // Verifica onde está o tio e guarda sua posição.
        if (avo->esq != NULL && pai == avo->esq) {
            tio = avo->dir;
        } else if (avo->dir != NULL && pai == avo->dir) {
            tio = avo->esq;
        }

        // Pai vermelho e Tio vermelho: só trocam de cores
        if (tio != NULL && tio->cor == 'v') {
            pai->cor = 'p';
            tio->cor = 'p';
            avo->cor = 'v';
            filho = avo;
        }

        // Pai vermelho e Tio preto (ou NULL): mudam de cor e faz rotações
        else {

            // Testa Caso LR ou RR
            if (tio == avo->dir) {
                if (filho == pai->dir) {
                    filho = pai;
                    rot_LL(filho);
                }

                filho->pai->cor = 'p';
                filho->pai->pai->cor = 'v';
                rot_RR(filho->pai->pai);
            }

            // Testa Caso RL ou LL
            else if (tio == avo->esq) {
                if (filho == pai->esq) {
                    filho = pai;
                    rot_RR(filho);
                }

                filho->pai->cor = 'p';
                filho->pai->pai->cor = 'v';
                rot_LL(filho->pai->pai);
            }
        }
    }

    // Faz Filho receber o endereço da Raíz e Retorna
    while (filho->pai != NULL) {
        filho = filho->pai;
    }

    // Corrige cor da Raíz para Preto
    filho->cor = 'p';
    return filho;
}

// =============================================================================
// -- INSERÇÕES

// Inserção de Letra
node inserir_letra (node p, char letra) {

    // Atribuições iniciais
    node aux = p;
    node tmp = alocar_letra(letra);

    // Se nó criado for o primeiro
    if (p == NULL) {
        tmp->cor = 'p';
        return tmp;
    }

    else {
        // Retorna o pai da posição ideal para o valor
        node pai = buscar_letra (aux, letra);

        // Retorna a posição do nó, se o valor já estiver inserido
        if (pai->letra == letra) {
            // printf("Letra %c já inserida!\n", pai->letra);
        }

        // Faz o filho apontar para o novo pai
        else {
            tmp->pai = pai;

            // Verifica em qual dos lados do pai deve ser inserido
            if (pai->letra > letra) pai->esq = tmp; // Inserindo à ESQUERDA
            else pai->dir = tmp; // Inserindo à DIREITA

            // Corrige violação das Propriedades
            aux = corrigir_insercao (tmp);
        }
    }
    return aux;
}

// Inserção de Palavra
int inserir_palavra (node p, char palavra[], int opt) {

    // Atribuições iniciais
    node aux = p;
    lista tmp = alocar_palavra(palavra);

    // Se não houver ainda nenhuma palavra inserida no nó
    if (p->conjunto == NULL) p->conjunto = tmp;

    else {
        // Retorna o anterior da posição ideal para o valor
        lista ant = buscar_palavra (aux->conjunto, palavra);

        // Palavra já consta no dicionáro
        if (strcmp(ant->palavra, palavra) == 0) {
            // Teste para inserções pós-início
            if (opt) {
                printf("A Palavra '%s' já consta no dicionário\n", palavra);
                opt = 0;
            }
            return 1;
        }

        // Palavra ainda não consta no dicionário
        else {
            // Se a palavra a ser inserida deve ocupar a primeira posição
            if (strcmp (ant->palavra, palavra) > 0) {
                p->conjunto = tmp;
                tmp->prox = ant;
            }
            // Do contrário, insere na posição correspondente
            else {
                lista pos = ant->prox;
                ant->prox = tmp;
                tmp->prox = pos;
            }
        }
    }

    // Caso a inserção tenha ocorrido normalmente
    if (opt) {
        // Informa inserção
        printf("A palavra '%s' foi Inserida com sucesso no NÓ '%c' ", palavra, palavra[0]);

        // Verifica a cor do nó
        mostra_cor(aux);

        // Calcula o nível do NÓ
        calcular_nivel(aux);
        puts("");
    }
    return 0;
}

// Função de Inserção da Main
node inserir (node p, char palavra[], int opt, int *count) {
    node aux;

    p = inserir_letra(p, palavra[0]);

    // Encontra o nó onde a palavra deverá ser inserida
    aux = buscar_letra (p, palavra[0]);

    // Lê e insere a palavra no nó referente (Se já estiver inserida retorna 1)
    *count = inserir_palavra(aux, palavra, opt);

    return p;
}

// =============================================================================
// -- REMOÇÕES

// Encontra o nó sucessor de aux
node sucessor (node aux) {
    // Encontra o nó sucessor de AUX
    while (aux->esq != NULL) {
		aux = aux->esq;
	}
    // Retorna a posição do nó encontrado
    return aux;
}

// Faz o nó V ser transplantado para o lugar de U
node transplante_RN (node raiz, node u, node v) {
    // U nó que será movido
    // V nó que ocupará o lugar de U

    // Se U for o nó RAIZ
    if (u == raiz) {
        // Transforma V na nova raíz
        raiz = v;
    }

    // Atualização do Ponteiro-Filho do Pai de U para V
    // Se U estiver à ESQUERDA de seu Pai
    else if (u->pai->esq != NULL && u == u->pai->esq) {
        // Faz seu pai apontar para V
        u->pai->esq = v;
    }

    // Se U estiver à DIREITA de seu Pai
    else if (u->pai->dir != NULL && u == u->pai->dir) {
        // Faz seu pai apontar para V
        u->pai->dir = v;
    }

    // Atualiza Pai de V recebe Pai de U
    if (v != NULL && u != NULL) {
        v->pai = u->pai;
    }

    return raiz;
    // No final, V estará no lugar de U, porém seus filhos ainda serão os mesmos
}

// Função de Correção após remoção
void corrigir_remocao (node raiz, node x, node w) {

    while (x != raiz && (x == NULL || x->cor == 'p')) {

        // Se X está à Esquerda de seu Pai
        if (w == w->pai->dir) {

            // Caso 1: Se o irmão W de X é Vermelho
            if (w->cor == 'v') {
                w->cor = 'p';
                w->pai->cor = 'v';
                rot_LL(w->pai);
                w = w->esq->dir;
            }

            // Caso 2: Sendo ambos os filhos de W de cor Preta
            if ((w->esq == NULL || w->esq->cor == 'p') && (w->dir == NULL || w->dir->cor == 'p')) {
                // Remove um Preto de W
                w->cor = 'v';
                // Atualiza X
                x = w->pai;
                // Se X->cor é Preto, então foi balanceada
                if (x->cor == 'p') break;
            }

            // Casos 3 e 4
            else {
                // Caso 3: Sendo apenas o filho da direita Preto
                if (w->dir != NULL && w->dir->cor == 'p') {
                    w->esq->cor = 'p';
                    w->cor = 'v';
                    rot_RR(w);
                    w = w->pai;
                }

                // Caso 4: Sendo W preto e W->dir Vermelho
                w->cor = w->pai->cor;
                w->pai->cor = 'p';
                w->dir->cor = 'p';
                rot_LL(w->pai);
                x = raiz;
            }
        }

        // Se X está à Direita de seu Pai
        else {
            // Caso 1: Se o irmão W de X é Vermelho
            if (w->cor == 'v') {
                w->cor = 'p';
                w->pai->cor = 'v';
                rot_RR(w->pai);
                w = w->dir->esq;
            }

            // Caso 2: Sendo ambos os filhos de W de cor Preta
            if ((w->esq == NULL || w->esq->cor == 'p') && (w->dir == NULL || w->dir->cor == 'p')) {
                // Remove um Preto de W
                w->cor = 'v';
                // Atualiza X
                x = w->pai;
                // Se X->cor é Preto, então foi balanceada
                if (x->cor == 'p') break;
            }

            // Casos 3 e 4
            else {
                // Caso 3: Sendo apenas o filho da direita Preto
                if (w->esq != NULL && w->esq->cor == 'p') {
                    w->dir->cor = 'p';
                    w->cor = 'v';
                    rot_LL(w);
                    w = w->pai;
                }

                // Caso 4: Sendo W preto e W->dir Vermelho
                w->cor = w->pai->cor;
                w->pai->cor = 'p';
                w->esq->cor = 'p';
                rot_RR(w->pai);
                x = raiz;
            }
        }
    }

    // Colore X de Preto
    x->cor = 'p';
}

// Retorna o irmão W de Y
node achar_w (node y) {

    // Verifica se Y é o nó Raiz
    if (y->pai != NULL) {
        // Verifica se Y está à Esquerda
        if (y->pai->esq == y) {
            return y->pai->dir;
        }

        // Verifica se Y está à Direita
        else {
            return y->pai->esq;
        }
    }

    // Retorna NULL se for Raiz
    return NULL;
}

// Remove o nó Z da árvore P
node remocao_letra (node p, node z) {
    // Z: Nó que será removido
    // Y: Nó que substituirá nó a ser removido
    // X: Rastreia nó Y após remoção a fim de corrigir violações

    // Atribuições iniciais
    node x, y, w;
    char y_cor_original;

    // Guarda posição de Z e cor de Y
    y = z;
    y_cor_original = y->cor;

    // Caso não haja filho da esquerda no nó que será removido
    // Então substitui o nó pelo seu filho à Direita
    if (z->esq == NULL) {
        // Rastreia X
        x = z->dir;
        // Encontra W
        w = achar_w(y);
        p = transplante_RN (p, z, z->dir);
    }

    // Da mesma forma, havendo apenas o filho à esquerda
    // Substitui o nó pelo seu filho à Esquerda
    else if (z->dir == NULL) {
        // Rastreia X
        x = z->esq;
        // Encontra W
        w = achar_w(y);
        p = transplante_RN (p, z, z->esq);
    }

    // Caso Z tenha os 2 filhos
    else {
        // Y recebe o sucessor de Z
        y = sucessor (z->dir);
        // Guarda a cor de Y
        y_cor_original = y->cor;
        // Rastreia X - X pode ser NULL
        x = y->dir;
        // Encontra W
        w = achar_w(y);

        // Caso Y seja filho imediato de Z
        if (y->pai == z) {
            // Nesse caso, X->pai aponta para Y, pois Y ficará no lugar de Z
            // Mas antes verifica se X não é NULL
            if (x) x->pai = y;
        }

        // Se Y não for filho imediato de Z
        else {
            // Troca posição de Y com seu filho X (y->dir)
            p = transplante_RN (p, y, y->dir);
            // Atualiza os ponteiros de Y para assumir a posição de Z com o filho da Direita correto
            y->dir = z->dir;
            y->dir->pai = y;
            // Note que Z e Y passam a ocupar o mesmo lugar - Duplicidade
        }

        // Troca de posição Z com Y
        p = transplante_RN (p, z, y);
        // Atualiza as informações de Y para assumir a posição de Z com o filho da Esquerda correto
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }

    // Libera o nó Z após todas as substituição
    z = NULL;
    free(z);

    // Se nó Y era preto, então será necessário corrigir árvore
    if (y_cor_original == 'p') {
        // Corrige nó X após remoção realizada
        corrigir_remocao (p, x, w);
    }

    return p;
}

// Remove palavra da lista P
node remover_palavra (node p, char palavra[]) {

    // Encontra o nó onde a palavra deve estar contida
    node aux = buscar_letra(p, palavra[0]);

    // Caso haja o NÓ no Dicionário
    if (aux->letra == palavra[0]) {
        // TMP recebe o endereço da lista referente a respectiva palavra
        lista tmp = aux->conjunto;
        // Salva a posição do anterior caso a palavra seja a primeira da lista
        lista ant = tmp;

        // Procura a posição da PALAVRA na lista salvando a posição de seu anterior
        while (tmp != NULL && strcmp(tmp->palavra, palavra) < 0) {
            ant = tmp;
            tmp = tmp->prox;
        }

        // Caso a PALAVRA não esteja no NÓ
        if (tmp == NULL || strcmp(tmp->palavra, palavra) != 0) {
            printf("A Palavra '%s' não consta no Dicionário\n", palavra);
            tmp = NULL;
        }

        // Caso a PALAVRA esteja no NÓ
        if (tmp) {
            // Caso a PALAVRA a ser removida seja a única da Lista
            // Então remove também o nó com a Letra
            if (tmp == ant && tmp->prox == NULL) {
                p = remocao_letra(p, aux);
            }

            // Caso a PALAVRA seja encontrada
            else if (strcmp(tmp->palavra, palavra) == 0) {
                if (tmp == ant) {
                    aux->conjunto = tmp->prox;
                } else {
                    ant->prox = tmp->prox;
                    tmp = NULL;
                }
            }

            // Informa que PALAVRA foi excluída
            printf("A palavra '%s' foi Excluída com sucesso do NÓ '%c' ", palavra, palavra[0]);

            // Verifica a cor do nó
            mostra_cor(aux);

            // Calcula o nível do NÓ
            calcular_nivel(aux);
            puts("");
        }
    }

    // Caso não haja o NÓ no Dicionário
    else {
        printf("A Palavra '%s' não consta no Dicionário\n", palavra);
    }

    return p;
}

// =============================================================================
// -- IMPRESSÕES

// Impressão de Palavra de um nó
void imprimir_no (node p, char letra) {
    // Encontra a posição do nó, referente a letra, na árvore
    node aux = buscar_letra (p, letra);

    // Caso nó esteja contido na árvore
    if (aux->letra == letra) {
        lista tmp = aux->conjunto;
        // Imprime as palavras contidas naquele nó
        printf("\nA(s) Palavra(s) contida(s) no NÓ '%c' ", letra);

        // Verifica a cor do nó e imprime
        mostra_cor(aux);

        // Calcula e imprime o nível
        calcular_nivel(aux);
        printf(" são:\n\n");

        // Imprime palavras contidas na Lista
        while (tmp != NULL) {
            printf("%s\n", tmp->palavra);
            tmp = tmp->prox;
        }
    }

    // Caso nó não esteja contido na árvore
    else {
        printf("Nó Inexistente...\n");
    }
}

// Impressão da Árvore completa
void imprimir_arvore (node p) {
    lista tmp;

    // Verifica se Árvore não está vazia
    if (p != NULL) {
        // Recursão à Esquerda
        if (p->esq) imprimir_arvore(p->esq);

        // Imprime a Letra do respectivo nó
        printf("\n- Nó %c ", p->letra);

        // Verifica a cor do nó e a imprime
        mostra_cor(p);

        // Calcula e imprime o nível
        calcular_nivel(p);
        printf(" -\n");

        // Imprime as palavras contidas na Lista
        tmp = p->conjunto;
        while (tmp != NULL) {
            printf("%s\n", tmp->palavra);
            tmp = tmp->prox;
        }

        // Recursão à Direita
        if (p->dir) imprimir_arvore(p->dir);
    }
}

// Função introdutório de leitura dos dados iniciais
node introitus (node p, char palavra[]) {
    // Contador para palavras repetidas
    int k = 0, countR = 0;
    // Contador para palavras inseridas
    int countI = 0;

    while (scanf("%s%*c", palavra) != EOF) {
        // Verifica se palavra lida é igual a 0
        if (palavra[0] == '0') break;

        // Se não, insere normalmente
        else {
            // Chama função de inserção
            p = inserir(p, palavra, 0, &k);

            // Contador de palavras repetidas
            if (k) countR++;

            // Incrementa contador de palavras
            countI++;
        }
    }

    if (countI) {
        printf("Todos os dados foram carregados com sucesso!!\n");
        if (countR) printf("Existiram %d palavras repetidas\n", countR);
        printf("Total de %d palavras inseridas no dicionário\n", countI);
        countI = 0;
    } else {
        printf("Erro de inserção!\n");
    }

    return p;
}

// =============================================================================
// -- FUNÇÃO PRINCIPAL

int main (){
    int opt = 0;
    node p = NULL;
    char palavra[30];

    p = introitus(p, palavra);

    while (opt != 6) {
        menu();
        scanf("%d", &opt);
        printf("%d\n", opt);

        // PESQUISA
        if (opt == 1) {
            // Recebe a palavra que vai ser buscada
            printf("Informe a palavra a ser Pesquisada:\n\n");
            scanf("%s%*c", palavra);
            printf("%s\n", palavra);

            // Chama função de pesquisa
            pesquisar(p, palavra);
        }

        // INSERÇÃO
        if (opt == 2) {
            // Verifica qual a palavra a ser inserida
            printf("Informe a palavra a ser Inserida:\n");
            scanf("%s%*c", palavra);
            printf("\n%s\n", palavra);

            // Chama função de inserção
            p = inserir(p, palavra, 1, &opt);
        }

        // REMOÇÃO
        if (opt == 3) {
            // Verifica qual a palavra a ser removida
            printf("Informe a palavra a ser Excluída:\n");
            scanf("%s%*c", palavra);
            printf("\n%s\n", palavra);

            // Chama a função de remoção do nó
            p = remover_palavra(p, palavra);
        }

        // IMPRESSÃO DE UM NÓ
        if (opt == 4) {

            // Recebe a letra que será impressa
            printf("Informe o nó que deseja Imprimir(Ou seja, primeira letra.)\n\n");
            scanf("%s%*c", palavra);
            printf("%s\n", palavra);

            // Chama função de impressão do nó
            imprimir_no(p, palavra[0]);
        }

        // IMPRESSÃO DA ÁRVORE
        if (opt == 5) {
            printf("Imprimindo Árvore...\n");

            // Chama função de impressão da árvore
            imprimir_arvore(p);
        }
    }

    printf("Programa Encerrado!!\n");

    return 0;
}
