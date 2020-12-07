#include <stdio.h>
#include <stdlib.h>

#pragma region Definição de Structs
// Struct para representar um nó da lista de adjacências
struct ListAdjNo { 
  int dest;                     // Vértice de destino no grafo
  int peso;                     // Peso da aresta para grafos ponderados
  struct ListAdjNo* prox;       // Próxima posição na lista
}; 
typedef struct ListAdjNo *listAdjNo;
  
// Struct para representar uma lista de adjacências
struct ListAdj { 
  struct ListAdjNo *cab;        // Nó cabeça da lista
}; 
typedef struct ListAdj *listAdj;

// Struct para representar um grafo.
// Um grafo possui um array de listas de adjacências.
// O tamanho do array será dado pelo número de vértices do grafo.
struct Graph { 
  int num;                      // Número de vértices do grafo
  struct ListAdj* lista;        // Lista de adjacências do grafo
}; 
typedef struct Graph *graph;

// Struct para representar um nó da árvore de conjuntos.
// Cada nó possui seu dado e um ponteiro para o representante.
struct Set {
  int indice;                   // Índice para identificar posição do conjunto disjunto
  struct Set *rep;              // Ponteiro para o representante do conjunto
};
typedef struct Set *set;

// Struct para guardar uma lista de prioridades contendo
// informações sobre cada aresta, tais como:
// vértice de origem, vértice de destino e peso da aresta.
// Além disso, contém um ponteiro para a próxima posição da lista.
struct WeightList {
  int vOrigem;                  // Vértice de origem
  int vDestino;                 // Vértice de destino
  int pesoAresta;               // Peso da aresta
  struct WeightList *prox;      // Ponteiro para a próxima posição
};
typedef struct WeightList *listaPesos;

#pragma endregion

#pragma region Alocações do Grafo/Arestas
// Aloca um nó para compor uma lista de adjacências
listAdjNo NovoNoAdjacente(int dest, int peso) { 
  listAdjNo novoNo = (listAdjNo) malloc(sizeof(listAdjNo)); 
  novoNo->dest = dest; 
  novoNo->peso = peso; 
  novoNo->prox = NULL; 
  return novoNo;
} 

// Cria um grafo de tamanho (numVertices)
graph CriaGrafo(int numVertices) { 
  graph grafo = (graph) malloc(sizeof(graph)); 
  grafo->num = numVertices; 

  // Cria um array de listas de adjacência.
  // O tamanho da lista de adjacências será dado por (numVertices)
  grafo->lista = (listAdj) malloc(numVertices * sizeof(listAdj)); 

  // Inicializa cada lista de adjacência como vazia,
  // fazendo cab receber um nó de aresta vazio.
  for (int i = 0; i < numVertices; ++i) {
    grafo->lista[i].cab = NovoNoAdjacente(0);
  }
  return grafo; 
}

// Adição de aresta no grafo
void AdicionaAresta(graph grafo, int src, int dest, int peso) { 
  // Adiciona uma aresta de src -> dest;
  // Um novo nó é adicionado à lista de adjacências de src.
  // O nó é adicionado no início da lista.
  listAdjNo novoNo = NovoNoAdjacente(dest, peso);
  novoNo->prox = grafo->lista[src].cab; 
  grafo->lista[src].cab = novoNo; 

  // Como o grafo não é direcionado,
  // adiciona uma aresta de dest -> src também.
  novoNo = NovoNoAdjacente(src, peso); 
  novoNo->prox = grafo->lista[dest].cab; 
  grafo->lista[dest].cab = novoNo; 
} 
#pragma endregion

#pragma region Funções Relacionadas a Conjuntos Disjuntos
// Aloca novo nó para um conjunto disjunto
set MakeSet (int vertice) {
  set tmp;
  tmp = (set)calloc(12, sizeof(set));
  tmp->indice = vertice;
  tmp->rep = tmp;
  return tmp;
}

// Encontra representante do conjunto
set FindSet (set conjunto) {
  while (conjunto->rep != conjunto) {
    conjunto = conjunto->rep;
  }
  return conjunto;
}

// Faz união de conjunto de origem com conjunto de destino
void UnionSet (set conjOrigem, set conjDestino) {
  if (conjOrigem != conjDestino) {
    conjOrigem->rep = conjDestino;
  }
}
#pragma endregion

/* TODO:
  - Criar lista ordenada com informações sobre as arestas e seus pesos;
  - Tal lista será usada para definir a lista de prioridades do algorítmo de Prim;
*/

// Função para inserir aresta na lista de prioridades
void InsereNaListaDePrioridade(listaPesos pLista, int origem, int destino, int peso) {

}

// Função para calcular árvore geradora mínima através do algoritmo de Prim
void AlgoritmoPrim(graph grafo, int origem, int pesos[grafo->num]) {
  // Cria conjunto disjunto com número de posições 
  // igual ao número de vértices do grafo.
  int vPesos[grafo->num];
  set conjunto[grafo->num];
  for (int i = 0; i < grafo->num; i++) {
    conjunto[i] = MakeSet(i);
    vPesos[i] = pesos[i];
  }
}

#pragma region Função Principal
int main () {
  int numVertices, numArestas, vInicio, valA, valB, peso;
  scanf("%d %d %d", &numVertices, &numArestas, &vInicio);

  // Cria grafo com número de vértices 
  // igual a (numVertices).
  graph grafo = CriaGrafo(numVertices);

  // Lê cada aresta adicionando os pares 
  // na lista dos seus devidos vértices.
  for (int i = 0; i < numArestas; i++) {
    scanf("%d %d %d", &valA, &valB, &peso);
    AdicionaAresta(grafo, valA, valB, peso); 
  }


  return 0;
}
#pragma endregion