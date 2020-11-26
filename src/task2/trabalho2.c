#include <stdio.h> 
#include <stdlib.h> 

#pragma region Structs
// Struct para representar um nó da lista de adjacências
struct AdjListNode { 
    int dest;
    struct AdjListNode* prox;   // Próxima posição na lista
}; 
typedef struct AdjListNode *listAdjNo;
  
// Struct para representar uma lista de adjacências
struct AdjList { 
    struct AdjListNode *cab;    // Nó cabeça da lista
}; 
typedef struct AdjList *listAdj;

// Struct para representar um grafo.
struct Graph { 
    int num;                    // Número de vértices do grafo
    struct AdjList* lista;      // Lista de adjacências do grafo
}; 
typedef struct Graph *sGrafo;

// Struct contendo listas auxiliares para busca DFS
struct AuxLists {
    int dTemp;
    int fTemp;
    int pred;
    char cor;
};
typedef struct AuxLists *auxDFS;

#pragma endregion

#pragma region Alocações: Grafo/Lista
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
    grafo->lista = (listAdj) malloc(numVertices * sizeof(listAdj)); 

    // Inicializa cada lista de adjacência como vazia, fazendo cab receber um nó de aresta vazio.
    listAdjNo noVazio = NovaListAdjNo(0);
    for (int i = 1; i <= grafo->num; ++i) { 
        grafo->lista[i].cab = noVazio;
    }
    return grafo; 
}

// Adição de aresta no grafo
void AdicionaAresta(sGrafo grafo, int src, int dest) { 
    listAdjNo novoNo = NovaListAdjNo(dest);     // Cria novo nó de lista de adj. com destino igual a dest
    novoNo->prox = grafo->lista[src].cab;       // Faz novo nó apontar para lista de adj. de src
    grafo->lista[src].cab = novoNo;             // Faz cab de src apontar para o novo nó (agora cabeça da lista)

    // Como o grafo não é direcionado, 
    // adiciona uma aresta de dest -> src também.
    novoNo = NovaListAdjNo(src); 
    novoNo->prox = grafo->lista[dest].cab; 
    grafo->lista[dest].cab = novoNo;
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
#pragma endregion  

#pragma region Algorítmo: BFS
// Função de busca em largura
void BuscaEmLargura(sGrafo grafo, int src) {
    int aDist[grafo->num];      // Array de distância
    int aPred[grafo->num];      // Array de predecessores
    int qVert[grafo->num];      // Fila para guardar próxima investigação no grafo 
    char aCores[grafo->num];    // Array com a cor de cada vértice
    int iFila = 0, fFila = 1;   // Variáveis para guardar início e fim da fila

    // Prepara vetores para iterações de busca a seguir
    for (int i = 0; i < grafo->num; i++) {
        aCores[i] = 'b';
        qVert[i] = 0;
        aPred[i] = -1;
        aDist[i] = -1;
    }

    listAdjNo noAux = grafo->lista[src].cab;

    int noAtual = src-1;
    int idxAux;

    aCores[noAtual] = 'c';          // Marca nó inicial como conhecido (Cinza)
    aDist[noAtual] = 0;             // Distância do nó inicial = 0
    aPred[noAtual] = 0;             // Nó inicial não possui predecessor
    qVert[iFila] = src;             // Enfila nó inicial

    while (qVert[iFila]) {
        noAtual = qVert[iFila];                         // Desenfila nó que será verificado
        iFila += 1;                                     // Incrementa índice inicial da fila
        noAux = grafo->lista[noAtual].cab;              // Pega lista do nó que será verificado

        // Impressão da fila
        printf("Fila: [");
        for (int i = iFila-1; i < fFila; i++) {
            printf(" %d ", qVert[i]);
        }
        printf("]\n");

        while (noAux->dest) {
            idxAux = noAux->dest-1;                     // Índice auxiliar
            if (aCores[idxAux] == 'b') {
                aCores[idxAux] = 'c';                   // Marca nó como conhecido (Cinza)
                aDist[idxAux] = aDist[noAtual-1] + 1;   // Define sua distância como dist. do nó atual + 1
                aPred[idxAux] = noAtual;                // Define seu predecessor com nó atual
                qVert[fFila] = noAux->dest;             // Enfila próximo vértice que deve ser visitado
                fFila += 1;                             // Incrementa índice final da fila
            }
            
            // Impressão de cada iteração
            printf("Para o nó %d-%d: ", noAtual, noAux->dest);
            printf("Dist. = %d | Cor = %c | Pi = %d\n", aDist[idxAux], aCores[idxAux], aPred[idxAux]);

            noAux = noAux->prox;                        // Atualiza posição da lista para a próxima
        }

        aCores[noAtual-1] = 'p';                        // Verificação concluída para este nó, marca de Preto      
    }
}
#pragma endregion

#pragma region Algorítmo: DFS
// Cria um grafo de tamanho (numVertices)
auxDFS CriaListaDFS() { 
    auxDFS tmp = (auxDFS) calloc(25, sizeof(auxDFS));
    tmp->pred = -1;
    tmp->dTemp = 0;
    tmp->fTemp = 0;
    tmp->cor = 'b';
    return tmp; 
}

// Função recursiva para visitação das posições da lista
int VisitaDFS(sGrafo grafo, int src, int tempo, auxDFS listaDFS[grafo->num]) {
    listaDFS[src-1]->cor = 'c';                 // Marca como visitado
    tempo += 1;                                 // Incrementa tempo
    listaDFS[src-1]->dTemp = tempo;             // Atualiza tempo de visitação

    int idxAux;                                 // Índice auxiliar
    listAdjNo noAux = grafo->lista[src].cab;    // Lista do nó atual em visitação
    
    while (noAux->dest) {                       
        idxAux = noAux->dest-1;                 // Índice auxiliar
        if (listaDFS[idxAux]->cor == 'b' ) {
            listaDFS[idxAux]->pred = src;       // Define seu predecessor como o nó atual
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

// Função de inicialização da busca em profundidade
void BuscaEmProfundidade(sGrafo grafo) {   
    int tempo = 0;                  // Variável para marcação do tempo
    auxDFS listaDFS[grafo->num];

    // Prepara vetores para iterações de busca a seguir
    for (int i = 0; i < grafo->num; i++) {
        listaDFS[i] = CriaListaDFS();
    }

    int idxAux;

    for (int i = 1; i <= grafo->num; i++) {
        idxAux = i-1;

        if (listaDFS[idxAux]->cor == 'b') {
            tempo = VisitaDFS(grafo, i, tempo, listaDFS);
        }
    }

    for (int i = 0; i < grafo->num; i++) {
        printf("Para o nó %d: ", i+1);
        printf("dTemp. = %d | fTemp. = %d | Cor = %c | Pi = %d\n", listaDFS[i]->dTemp, listaDFS[i]->fTemp, listaDFS[i]->cor, listaDFS[i]->pred);
    }
}
#pragma endregion

int main() { 
    int numVertices, numArestas, vInicio, valA, valB;
    scanf("%d %d %d", &numVertices, &numArestas, &vInicio);

    // Cria grafo com número de vértices 
    // igual a (numVertices).
    sGrafo grafo = CriaGrafo(numVertices); 

    // Lê cada aresta adicionando os pares 
    // na lista dos seus devidos vértices.
    for (int i = 0; i < numArestas; i++) {
        scanf("%d %d", &valA, &valB);
        AdicionaAresta(grafo, valA, valB);
    }

    // ImprimeGrafo(grafo);
    BuscaEmLargura(grafo, 1);
    // BuscaEmProfundidade(grafo);
    return 0;
}