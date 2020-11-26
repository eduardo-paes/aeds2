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
  struct AdjListNode *cab;    // Nó cabeça da lista
}; 
typedef struct AdjList *listAdj;

// Struct para representar um grafo.
// Um grafo possui um array de listas de adjacências.
// O tamanho do array será dado pelo número de vértices do grafo.
struct Graph { 
  int num;                    // Número de vértices do grafo
  struct AdjList* lista;      // Lista de adjacências do grafo
}; 
typedef struct Graph *sGrafo;

// Aloca um nó para compor uma lista de adjacências
listAdjNo NovaListAdjNo(int dest) { 
  listAdjNo novoNo = (listAdjNo) malloc(sizeof(listAdjNo)); 
  novoNo->dest = dest; 
  novoNo->prox = NULL; 
  return novoNo;
} 
  
// Cria um grafo de tamanho (numVertices)
sGrafo CriaGrafo(int numVertices) { 
  sGrafo grafo = (sGrafo) malloc(sizeof(sGrafo)); 
  grafo->num = numVertices; 

  // Cria um array de listas de adjacência.
  // O tamanho da lista de adjacências será dado por (numVertices)
  grafo->lista = (listAdj) malloc(numVertices * sizeof(listAdj)); 

  // Inicializa cada lista de adjacência como vazia,
  // fazendo cab receber um nó de aresta vazio.
  listAdjNo noVazio = NovaListAdjNo(0);
  for (int i = 1; i <= numVertices; ++i) {
    grafo->lista[i].cab = noVazio;
    // printf("%d\n", grafo->lista[i].cab->dest);
  }
  return grafo; 
}
  
// Adição de aresta no grafo
void AdicionaAresta(sGrafo grafo, int src, int dest) { 
  // Adiciona uma aresta de src -> dest;
  // Um novo nó é adicionado à lista de adjacências de src.
  // O nó é adicionado no início da lista.
  listAdjNo novoNo = NovaListAdjNo(dest);
  novoNo->prox = grafo->lista[src].cab; 
  grafo->lista[src].cab = novoNo; 

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

    if (listaVertice->dest == 0) {
      printf("-> vazio\n"); 
    }
    else {
      while (listaVertice->dest > 0) { 
        printf("-> %d", listaVertice->dest); 
        listaVertice = listaVertice->prox; 
      }
      printf("\n"); 
    }
  } 
} 
  
void main() { 
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

  ImprimeGrafo(grafo); 
}