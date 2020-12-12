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
    int dTemp;
    int fTemp;
    char cor;
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
auxDFS CriaListaDFS() { 
    auxDFS tmp = (auxDFS) calloc(25, sizeof(auxDFS));
    predList aux = (predList) calloc(16, sizeof(predList));
    aux->ger = -1;
    aux->prox = NULL;
    tmp->pred = aux;
    tmp->dTemp = 0;
    tmp->fTemp = 0;
    tmp->cor = 'b';
    return tmp; 
}
  
// Cria um grafo de tamanho (numVertices)
sGrafo CriaGrafo(int numVertices, auxDFS listaDFS[numVertices], int guardaIndice[numVertices]) { 
    sGrafo grafo = (sGrafo) malloc(sizeof(sGrafo)); 
    grafo->num = numVertices; 

    // Cria um array de listas de adjacência.
    grafo->lista = (listAdj) calloc(24, numVertices * sizeof(listAdj)); 

    // Inicializa cada lista de adjacência como vazia, fazendo cab receber um nó de aresta vazio.
    for (int i = 1; i <= grafo->num; ++i) { 
        grafo->lista[i].cab = NovaListAdjNo(0);
        listaDFS[i-1] = CriaListaDFS();
        guardaIndice[i] = i;
    }
    return grafo; 
}

// Adição de aresta no grafo
void AdicionaAresta(sGrafo grafo, int src, int dest) { 
    listAdjNo novoNo = NovaListAdjNo(dest);     // Cria novo nó de lista de adj. com destino igual a dest
    novoNo->prox = grafo->lista[src].cab;       // Faz novo nó apontar para lista de adj. de src
    grafo->lista[src].cab = novoNo;             // Faz cab de src apontar para o novo nó (agora cabeça da lista)
} 

// Adiciona gerente como predecessor na lista de predecessores de empregado
void AdicionaPredecessor(int num, auxDFS listaDFS[num], int empregado, int gerente ) {
    predList aux = (predList) calloc(16, sizeof(predList));
    aux->ger = gerente;
    aux->prox = listaDFS[empregado]->pred;
    listaDFS[empregado]->pred = aux;
}

// Função recursiva para visitação das posições da lista
int VisitaDFS(sGrafo grafo, int src, int tempo, auxDFS listaDFS[grafo->num]) {
    listaDFS[src-1]->cor = 'c';                 // Marca como visitado
    tempo += 1;                                 // Incrementa tempo
    listaDFS[src-1]->dTemp = tempo;             // Atualiza tempo de visitação
    listAdjNo noAux = grafo->lista[src].cab;    // Lista do nó atual em visitação
    
    while (noAux->dest) {
        // Adiciona nó de origem como predecessor na lista de nós do nó a ser visitado
        AdicionaPredecessor(grafo->num, listaDFS, noAux->dest-1, src);
        
        // Verifica se nó a ser visitado possui a cor branca.
        if (listaDFS[noAux->dest-1]->cor == 'b' ) {
            tempo = VisitaDFS(grafo, 
                noAux->dest, tempo, listaDFS);  // Chama visita recursiva caso o nó ainda não tenha sido visitado.
        }
        noAux = noAux->prox;                    // Atualiza posição da lista para a próxima.
    }
    
    listaDFS[src-1]->cor = 'p';                 // Marca nó como completamente visitado.
    tempo += 1;                                 // Incrementa tempo de visitação.
    listaDFS[src-1]->fTemp = tempo;             // Guarda tempo de término da visitação.
    return tempo;                               // Retorna tempo geral, a fim de manter variável atualizada entre iterações.
}

// Visita recursivamente cada posição na lista de predecessores do nó
int VisitaGerente(sGrafo grafo, auxDFS listaDFS[grafo->num], int gerente, int idadeGer) {
    predList aux = listaDFS[gerente]->pred;
    // A cada iteração, testa se a idade do gerente é menor do que a idade salva em idadeGer.
    // Caso seja a primeira iteração, idadeGer recebe idade do gerente.
    while(aux->ger > -1) {
        idadeGer = VisitaGerente(grafo, listaDFS, aux->ger-1, idadeGer);
        if (!idadeGer || idadeGer > grafo->lista[aux->ger].idade) {
            idadeGer = grafo->lista[aux->ger].idade;
        }
        aux = aux->prox;
    }
    // Retorna idadeGer a fim de manter o valor atualizado entre as iterações.
    return idadeGer;
}

// Trata a solicitação de pergunta do usuário retornando a idade do gerente mais novo de determinado empregado.
// Caso não haja gerente, retorna (*).
void TrataPergunta(sGrafo grafo, auxDFS listaDFS[grafo->num], int emp) {
    int idadeGer = 0;

    // Após achar posição do empregado, declara vetor auxiliar contedo essa posição para iniciar a busca
    predList aux = listaDFS[emp-1]->pred;

    // Busca recursiva para identificar qual o gerente mais novo do empregado.
    // Todos os gerente do empregado serão visitados e a idade do mais novo erá salva
    // na variável idadeGer.
    while (aux->ger > -1) {
        idadeGer = VisitaGerente(grafo, listaDFS, aux->ger-1, idadeGer);
        if (!idadeGer || idadeGer > grafo->lista[aux->ger].idade) {
            idadeGer = grafo->lista[aux->ger].idade;
        }
        aux = aux->prox;
    }

    // Cado idadeGer seja diferente de 0, imprime na tela.
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

// Função de inicialização da busca em profundidade
void BuscaEmProfundidade(sGrafo grafo, auxDFS listaDFS[grafo->num]) {   
    int tempo = 0;                  // Variável para marcação do tempo
    for (int i = 1; i <= grafo->num; i++) {
        if (listaDFS[i-1]->cor == 'b') {
            tempo = VisitaDFS(grafo, i, tempo, listaDFS);
        }
    }
}

int main() { 
    int numEmpregados, numArestas, numInstrucoes, valA, valB;

    while(scanf("%d %d %d", &numEmpregados, &numArestas, &numInstrucoes) != EOF) {
        // Cria grafo com número de vértices igual a (numEmpregados).
        auxDFS listaDFS[numEmpregados];
        int guardaIndice[numEmpregados];
        sGrafo grafo = CriaGrafo(numEmpregados, listaDFS, guardaIndice);


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
            AdicionaAresta(grafo, valA, valB);
        }

        // Realiza busca para preparação da ListaDFS
        BuscaEmProfundidade(grafo, listaDFS);
        
        // Lê e trata cada instrução do usuário
        char instrucao;
        for (int i = 0; i < numInstrucoes; i++) {   // Mostrou-se mais eficiente em termos de [time / memory]
            scanf(" %c", &instrucao);
            if (instrucao == 'P') {
                scanf(" %d", &valA);
                TrataPergunta(grafo, listaDFS, guardaIndice[valA]);
            }
            if (instrucao == 'T') {
                scanf(" %d %d", &valA, &valB);
                TrataTroca(grafo, valA, valB, guardaIndice);
            }
        }
    }

    return 0;
}