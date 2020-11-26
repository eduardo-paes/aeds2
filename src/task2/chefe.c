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
    int key;
    struct AdjListNode *cab;    // Nó cabeça da lista
}; 
typedef struct AdjList *listAdj;

// Struct para representar um grafo.
struct Graph { 
    int num;                    // Número de vértices do grafo
    struct AdjList* lista;      // Lista de adjacências do grafo
}; 
typedef struct Graph *sGrafo;

// Struct para representar uma lista de predecessores
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
sGrafo CriaGrafo(int numVertices) { 
    sGrafo grafo = (sGrafo) malloc(sizeof(sGrafo)); 
    grafo->num = numVertices; 

    // Cria um array de listas de adjacência.
    grafo->lista = (listAdj) calloc(24, numVertices * sizeof(listAdj)); 

    // Inicializa cada lista de adjacência como vazia, fazendo cab receber um nó de aresta vazio.
    for (int i = 1; i <= grafo->num; ++i) { 
        grafo->lista[i].cab = NovaListAdjNo(0);
    }
    return grafo; 
}

// Adição de aresta no grafo
void AdicionaAresta(sGrafo grafo, int src, int dest) { 
    listAdjNo novoNo = NovaListAdjNo(dest);     // Cria novo nó de lista de adj. com destino igual a dest
    novoNo->prox = grafo->lista[src].cab;       // Faz novo nó apontar para lista de adj. de src
    grafo->lista[src].cab = novoNo;             // Faz cab de src apontar para o novo nó (agora cabeça da lista)
} 

// Função para impressão de uma lista de adjacências
void ImprimeGrafo(sGrafo grafo) { 
  for (int i = 1; i <= grafo->num; ++i) { 
    listAdjNo listaVertice = grafo->lista[i].cab; 
    printf("Adjacencias do vertice [%d] ", i);

    if (!listaVertice->dest) {
        // Para vértices sem arestas
        printf("-> vazio\n"); 
    } else {
        while (listaVertice->dest) { 
            printf("-> %d", listaVertice->dest); 
            listaVertice = listaVertice->prox; 
        }
        puts(""); 
    }
  } 
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

    int idxAux;                                 // Índice auxiliar
    listAdjNo noAux = grafo->lista[src].cab;    // Lista do nó atual em visitação
    
    while (noAux->dest && noAux != NULL) {                       
        idxAux = noAux->dest-1;                 // Índice auxiliar
        AdicionaPredecessor(grafo->num, listaDFS, idxAux, src);

        if (listaDFS[idxAux]->cor == 'b' ) {
            // listaDFS[idxAux]->pred = src;       // Define seu predecessor como o nó atual
            tempo = VisitaDFS(grafo, noAux->dest,       // Chama visita recursiva caso o nó ainda não tenha sido visitado
                tempo, listaDFS);
        }

        noAux = noAux->prox;                    // Atualiza posição da lista para a próxima
    }
    listaDFS[src-1]->cor = 'p';                 // Marca nó como completamente visitado
    tempo += 1;                                 // Incrementa tempo de visitação
    listaDFS[src-1]->fTemp = tempo;             // Guarda tempo de término da visitação
    return tempo;
}

void ImprimeListaDFS(int num, auxDFS listaDFS[num]) {
    predList aux;
    for (int i = 0; i < num; i++) {
        printf("Para o nó %d: ", i+1);
        printf("dTemp. = %d | fTemp. = %d   | Cor = %c  | Pi = ", listaDFS[i]->dTemp, listaDFS[i]->fTemp, listaDFS[i]->cor);

        aux = listaDFS[i]->pred;
        while (aux != NULL && aux->ger != -1) {
            printf("%d ", aux->ger);
            aux = aux->prox;
        }
        puts("");
    }
}

int VisitaGerente(sGrafo grafo, auxDFS listaDFS[grafo->num], int gerente, int idadeGer) {
    predList aux = listaDFS[gerente]->pred;

    while(aux != NULL && aux->ger != -1) {

        idadeGer = VisitaGerente(grafo, listaDFS, aux->ger-1, idadeGer);

        if (!idadeGer || idadeGer > grafo->lista[aux->ger].idade) {
            idadeGer = grafo->lista[aux->ger].idade;
        }

        aux = aux->prox;
    }

    return idadeGer;
}

void TrataPergunta(sGrafo grafo, auxDFS listaDFS[grafo->num], int emp) {
    int idadeGer = 0;

    int id = 0;
    for (int i = 1; i <= grafo->num; i++) {
        if (!id && grafo->lista[i].key == emp){
            id = i;
        }
    }
    emp = id;

    predList aux = listaDFS[emp-1]->pred;

    if (aux->ger == -1) {
        printf("*\n");
    } else {
        while (aux->ger != -1) {

            idadeGer = VisitaGerente(grafo, listaDFS, aux->ger-1, idadeGer);

            if (!idadeGer || idadeGer > grafo->lista[aux->ger].idade) {
                idadeGer = grafo->lista[aux->ger].idade;
            }

            aux = aux->prox;
        }
        printf("%d\n", idadeGer);
    }
}

void TrataTroca(sGrafo grafo, int src, int dest) {
    int idSRC = 0, idDEST = 0; 
    int idadeSRC, idadeDEST;

    for (int i = 1; i <= grafo->num; i++) {
        if (!idSRC && grafo->lista[i].key == src){
            idSRC = i;
            idadeSRC = grafo->lista[i].idade;
        }

        if (!idDEST && grafo->lista[i].key == dest){
            idDEST = i;
            idadeDEST = grafo->lista[i].idade;
        }
    }

    grafo->lista[idSRC].key = dest;
    grafo->lista[idSRC].idade = idadeDEST;
    grafo->lista[idDEST].key = src;
    grafo->lista[idDEST].idade = idadeSRC;
}

// Função de inicialização da busca em profundidade
void BuscaEmProfundidade(sGrafo grafo, auxDFS listaDFS[grafo->num]) {   
    int tempo = 0;                  // Variável para marcação do tempo

    int idxAux;

    for (int i = 1; i <= grafo->num; i++) {
        idxAux = i-1;

        if (listaDFS[idxAux]->cor == 'b') {
            tempo = VisitaDFS(grafo, i, tempo, listaDFS);
        }
    }
    
    // ImprimeListaDFS(grafo->num, listaDFS);
}

int main() { 
    int numEmpregados, numArestas, numInstrucoes, valA, valB;
    scanf("%d %d %d", &numEmpregados, &numArestas, &numInstrucoes);

    // Cria grafo com número de vértices igual a (numEmpregados).
    sGrafo grafo = CriaGrafo(numEmpregados);
    auxDFS listaDFS[numEmpregados];

    // Prepara vetores para iterações de busca a seguir
    for (int i = 0; i < grafo->num; i++) {
        listaDFS[i] = CriaListaDFS();
    }

    // Lê idade dos empregados e salva num vetor auxiliar
    int idade;
    for (int i = 1; i <= numEmpregados; i++) {
        if (i == numEmpregados-1) {
            scanf("%d", &idade);
        } else {
            scanf("%d ", &idade);
        }

        grafo->lista[i].idade = idade;
        grafo->lista[i].key = i;
    }

    // Lê cada aresta adicionando os pares na lista dos seus devidos vértices.
    for (int i = 0; i < numArestas; i++) {
        scanf("%d %d%*c", &valA, &valB);
        AdicionaAresta(grafo, valA, valB);
    }

    BuscaEmProfundidade(grafo, listaDFS);
    
    char instrucao;
    scanf("%*c");

    // Lê e trata cada instrução do usuário
    for (int i = 0; i < numInstrucoes; i++) {
        scanf(" %c", &instrucao);
        
        if (instrucao == 'P') {
            scanf(" %d", &valA);
            TrataPergunta(grafo, listaDFS, valA);
        }

        if (instrucao == 'T') {
            scanf(" %d %d", &valA, &valB);
            TrataTroca(grafo, valA, valB);
        }
    }
    return 0;
}