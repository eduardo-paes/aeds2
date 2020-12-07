#include <stdio.h> 
#include <stdlib.h> 

// Struct para representar um nó da lista de adjacências
struct AdjListNode { 
    int dest;
    struct AdjListNode* prox;   // Próxima posição na lista
}; 
typedef struct AdjListNode *listAdjNo;
  
// Struct para representar uma lista de adjacências
struct AdjList { 
    int idade;
    struct AdjListNode *cab;    // Nó cabeça da lista
}; 
typedef struct AdjList *listAdj;

// Struct para representar um grafo.
struct Graph { 
    int num;                    // Número de vértices do grafo
    struct AdjList* lista;      // Lista de adjacências do grafo
}; 
typedef struct Graph *sGrafo;

// Struct para representar uma lista de predecessores (armazena os gerentes do empregado)
struct PredList { 
    int ger;
    struct PredList* prox;      // Nó cabeça da lista
}; 
typedef struct PredList *predList;

// Struct contendo listas auxiliares para busca DFS
struct AuxLists {
    struct PredList* pred;
};
typedef struct AuxLists *auxDFS;

// Aloca um nó para compôr uma lista de adjacências
listAdjNo NovaListAdjNo(int dest) {
    listAdjNo novoNo = calloc(16, sizeof(listAdjNo));
    novoNo->dest = dest; 
    novoNo->prox = NULL;
    return novoNo;
} 

// Cria um grafo de tamanho (numVertices)
auxDFS CriaListaGerentes() { 
    auxDFS tmp = (auxDFS) calloc(8, sizeof(auxDFS));
    predList aux = (predList) calloc(16, sizeof(predList));
    aux->ger = -1;
    aux->prox = NULL;
    tmp->pred = aux;
    return tmp;
}
  
// Cria um grafo de tamanho (numVertices)
sGrafo CriaGrafo(int numVertices, auxDFS listaGerentes[numVertices], int guardaIndice[numVertices]) { 
    sGrafo grafo = (sGrafo) malloc(sizeof(sGrafo)); 
    grafo->num = numVertices; 

    // Cria um array de listas de adjacência.
    grafo->lista = (listAdj) calloc(16, numVertices * sizeof(listAdj)); 

    // Inicializa cada lista de adjacência como vazia, fazendo cab receber um nó de aresta vazio.
    for (int i = 1; i <= grafo->num; ++i) { 
        grafo->lista[i].cab = NovaListAdjNo(0);
        listaGerentes[i-1] = CriaListaGerentes();
        guardaIndice[i] = i;
    }
    return grafo;
}

// Adiciona gerente como predecessor na lista de predecessores de empregado
void AdicionaPredecessor(int num, auxDFS listaGerentes[num], int empregado, int gerente ) {
    predList aux = (predList) calloc(16, sizeof(predList));
    aux->ger = gerente;
    aux->prox = listaGerentes[empregado]->pred;
    listaGerentes[empregado]->pred = aux;
}

// Adição de aresta no grafo
void AdicionaAresta(sGrafo grafo, int src, int dest, auxDFS listaGerentes[grafo->num]) { 
    listAdjNo novoNo = NovaListAdjNo(dest);     // Cria novo nó de lista de adj. com destino igual a dest
    novoNo->prox = grafo->lista[src].cab;       // Faz novo nó apontar para lista de adj. de src
    grafo->lista[src].cab = novoNo;             // Faz cab de src apontar para o novo nó (agora cabeça da lista)
    AdicionaPredecessor(grafo->num, listaGerentes, dest-1, src);
} 

// Trata a solicitação de pergunta do usuário retornando a idade do gerente mais novo de determinado empregado.
// Caso não haja gerente, retorna (*).
// O algoritimo de busca BFS foi utilizada para esta finalidade.
void TrataPergunta(sGrafo grafo, auxDFS listaGerentes[grafo->num], int emp) {
    int idadeGer = 0;
    int qVert[2*grafo->num];    // Fila para guardar próxima investigação no grafo (tamanho dobrado para impedir acesso de memória inválido)
    char aCores[grafo->num];    // Array com a cor de cada vértice
    int iFila = 0, fFila = 1;   // Variáveis para guardar início e fim da fila

    // Prepara vetores para iterações de busca a seguir
    for (int i = 0; i < 2*grafo->num; i++) {
        if (i >= grafo->num) {
            qVert[i] = 0;
        } else {
            aCores[i] = 'b';
            qVert[i] = 0;
        }
    }

    // Após achar posição do empregado, declara vetor auxiliar contendo essa posição para iniciar a busca
    predList noAux = listaGerentes[emp-1]->pred;

    int noAtual = emp;
    int idxAux;

    aCores[noAtual] = 'c';          // Marca nó inicial como conhecido (Cinza)
    qVert[iFila] = emp;             // Enfila nó inicial

     while (qVert[iFila]) {
        noAtual = qVert[iFila];                         // Desenfila nó que será verificado
        iFila += 1;                                     // Incrementa índice inicial da fila
        noAux = listaGerentes[noAtual-1]->pred;

        while (noAux->ger > -1) {
            if (!idadeGer || idadeGer > grafo->lista[noAux->ger].idade) {
                idadeGer = grafo->lista[noAux->ger].idade;
            }
            
            idxAux = noAux->ger-1;                      // Índice auxiliar
            if (aCores[idxAux] == 'b') {
                aCores[idxAux] = 'c';                   // Marca nó como conhecido (Cinza)
                qVert[fFila] = noAux->ger;              // Enfila próximo vértice que deve ser visitado
                fFila += 1;                             // Incrementa índice final da fila
            }
            noAux = noAux->prox;                        // Atualiza posição da lista para a próxima
        }
        aCores[noAtual] = 'p';                          // Verificação concluída para este nó, marca de Preto      
    }

    // Caso idadeGer seja diferente de 0, imprime na tela.
    // Do contrário, imprime (*).
    if (idadeGer) {
        printf("%d\n", idadeGer);
    } else {
        printf("*\n");
    }
}

// Realiza a troca de posições entre src e dest.
// Para tanto é trocada os valores de key e idade dos vértices.
void TrataTroca(sGrafo grafo, int src, int dest, int indices[grafo->num]) {
    int posSRC = indices[src];
    int posDEST = indices[dest];

    int idadeSRC = grafo->lista[posSRC].idade;
    int idadeDEST = grafo->lista[posDEST].idade;

    grafo->lista[posSRC].idade = idadeDEST;
    grafo->lista[posDEST].idade = idadeSRC;

    indices[src] = posDEST;
    indices[dest] = posSRC;
}

int main() { 
    int numEmpregados, numArestas, numInstrucoes, valA, valB;

    while(scanf("%d %d %d", &numEmpregados, &numArestas, &numInstrucoes) != EOF) {
        // Cria grafo com número de vértices igual a (numEmpregados).
        auxDFS listaGerentes[numEmpregados];
        int guardaIndice[numEmpregados];
        sGrafo grafo = CriaGrafo(numEmpregados, listaGerentes, guardaIndice);

        // Lê idade dos empregados e salva num vetor auxiliar
        int idade;
        for (int i = 1; i <= numEmpregados; i++) {
            if (i == numEmpregados-1) {
                scanf("%d", &idade);
            } else {
                scanf("%d ", &idade);
            }
            grafo->lista[i].idade = idade;
        }

        // Lê cada aresta adicionando os pares na lista dos seus devidos vértices.
        for (int i = 0; i < numArestas; i++) {
            scanf("%d %d%*c", &valA, &valB);
            AdicionaAresta(grafo, valA, valB, listaGerentes);
        }
        
        // Lê e trata cada instrução do usuário
        char instrucao;
        for (int i = 0; i < numInstrucoes; i++) {   // Mostrou-se mais eficiente em termos de [time / memory]
            scanf(" %c", &instrucao);
            if (instrucao == 'P') {
                scanf(" %d", &valA);
                TrataPergunta(grafo, listaGerentes, guardaIndice[valA]);
            }
            if (instrucao == 'T') {
                scanf(" %d %d", &valA, &valB);
                TrataTroca(grafo, valA, valB, guardaIndice);
            }
        }
    }
    return 0;
}