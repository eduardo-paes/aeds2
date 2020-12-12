#include <stdio.h>
#include <stdlib.h>

// Variável auxiliar utilizada para simular um valor infinito, 
// conforme requisitado pelo algorítmo de Dijkstra.
// O valor 10000 é considerado suficiente para suprir os 
// requisitos dos casos de teste propostos.
int valMaximo = 10000;

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

// Lista auxiliar contendo valores de distância e predecessor a serem utilizados 
// pelo algorítmo de Dijkstra na busca por um caminho mínimo da origem até
// os demais nós do grafo.
struct AuxList {
  int dist;                     // Variável para armazenamento da distância do nó até a origem
  int pred;                     // Variável para armazenamento do predescessor do nó
};
typedef struct AuxList *auxList;

// Estrutura para alocação de peso e chave numa lista de prioridade 
// baseada no algorítmo heap.
struct HeapList {
  int chave;                    // Identificação do vértice na lista
  int peso;                     // Peso referente ao vértice na lista
};
typedef struct HeapList *heapList;

#pragma endregion

#pragma region Alocações do Grafo/Arestas
// Aloca um nó para compor uma lista de adjacências
listAdjNo NovoNoAdjacente(int dest, int peso) { 
  listAdjNo novoNo = (listAdjNo)calloc(16, sizeof(listAdjNo)); 
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
  for (int i = 1; i <= numVertices; ++i) {
    grafo->lista[i].cab = NovoNoAdjacente(0, valMaximo);
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
} 

// Cria uma posição vazia para a lista auxiliar
auxList CriaListaAuxiliar() { 
  auxList tmp = (auxList) calloc(8, sizeof(auxList));
  tmp->pred = -1;
  tmp->dist = valMaximo;
  return tmp; 
}

#pragma endregion

#pragma region Algorítmos para Fila de Prioridade Heap
heapList AlocaListaHeap() {
  heapList tmp = (heapList)calloc(8, sizeof(heapList));
  tmp->chave = 0; 
  tmp->peso = 0;
  return tmp;
}

void TrocaPosicao(heapList vet[], int valA, int valB) {
  heapList tmp = vet[valA];
  vet[valA] = vet[valB];
  vet[valB] = tmp;
}

int RetornaFilhoDireita(heapList vet[], int indice, int num) {
  if((((2*indice)+1) < num) && (indice >= 1))
    return (2*indice) + 1;
  return -1;
}

int RetornaFilhoEsquerda(heapList vet[], int indice, int num) {
  if(((2*indice) < num) && (indice >= 1))
    return 2*indice;
  return -1;
}

int RetornaPai(heapList vet[], int indice, int num) {
  if ((indice > 1) && (indice <= num)) {
    return indice/2;
  }
  return -1;
}

void HeapfyMinimo(heapList vet[], int indice, int num, int heapTam) {
  int fe = RetornaFilhoEsquerda(vet, indice, num);   // Índice do Filho da Esquerda
  int fd = RetornaFilhoDireita(vet, indice, num);    // Índice do Filho da Direita

  int menorVertice = indice;

  if ((fe <= heapTam) && (fe > 0)) {
    if (vet[fe]->peso < vet[menorVertice]->peso) {
      menorVertice = fe;
    }
  }

  if ((fd <= heapTam && (fd > 0))) {
    if (vet[fd]->peso < vet[menorVertice]->peso) {
      menorVertice = fd;
    }
  }

  if (menorVertice != indice) {
    TrocaPosicao(vet, indice, menorVertice);
    HeapfyMinimo(vet, menorVertice, num, heapTam);
  }
}

heapList ExtraiMenor(heapList vet[], int num, int heapTam) {
  heapList menor = vet[1];
  vet[1] = vet[heapTam];
  heapTam--;
  HeapfyMinimo(vet, 1, num, heapTam);
  return menor;
}

void DiminuiPeso(heapList vet[], int chave, int peso, int num, int indice) {
  vet[indice]->peso = peso;
  vet[indice]->chave = chave;

  while((indice > 1) && (vet[RetornaPai(vet, indice, num)]->peso > vet[indice]->peso)) {
    TrocaPosicao(vet, indice, RetornaPai(vet, indice, num));
    indice = RetornaPai(vet, indice, num);
  }
}

void AumentaPeso(heapList vet[], int indice, int peso, int num, int heapTam) {
  vet[indice]->peso = peso;
  HeapfyMinimo(vet, indice, num, heapTam);
}

int InsereFila(heapList vet[], int chave, int peso, int num, int heapTam) {
  heapTam++;
  vet[heapTam] = AlocaListaHeap();
  vet[heapTam]->peso = valMaximo;
  DiminuiPeso(vet, chave, peso, num, heapTam);
  return heapTam;
}
#pragma endregion

#pragma region Algorítmo de Dijkstra
// Inicializa lista auxiliar fazendo todas as distâncias receberem 
// valor máximo e predecessor recebe valor 0.
// Para o nó de início, distância igual a 0.
void Inicializador(graph grafo, int src, auxList listaAuxiliar[], int verificador[]) {
  for (int i = 1; i <= grafo->num; i++) {
    verificador[i] = 0;
    listaAuxiliar[i] = CriaListaAuxiliar();
  }
  listaAuxiliar[src]->dist = 0;
}

// Função para realizar o relaxamento da aresta com origem em "src" e destino em "dest".
// Nesta operação, caso a distância de "dest" seja maior que a distância de "src" mais o peso da aresta que os conecta,
// atualiza distância de "dest" para distância de "src" mais peso da aresta.
void Relaxamento(auxList listaAuxiliar[], int src, int dest, int peso) {
  int srcToDest = listaAuxiliar[src]->dist + peso;
  if (listaAuxiliar[dest]->dist > srcToDest) {
    listaAuxiliar[dest]->dist = srcToDest;
    listaAuxiliar[dest]->pred = src;
  }
}

void AlgoritmoDijkstra (graph grafo, int src) {
  int numVertices = grafo->num, nosVisitados[numVertices];
  int posFila = 0, heapTam = 0;

  // Lista auxiliar contendo distância e predecessor de cada vértice
  auxList listaAuxiliar[numVertices];

  // Lista de prioridade para armazenamento de índice e peso de cada nó
  heapList listaPrioridade[numVertices];

  // Lista para verificação se o nó já foi inserido na lista de prioridade
  int verificador[numVertices];

  // Função de inicializção.
  Inicializador(grafo, src, listaAuxiliar, verificador);

  // Inicializa lista de prioridades inserindo vértice inicial na lista.
  heapTam = InsereFila(listaPrioridade, src, 0, numVertices, heapTam);

  // Marca como nó já inserido na lista de prioridade a fim de evitar duplicidade
  verificador[src] = 1;

  // Variável auxiliar para percorrer lista de adjacências do vértice.
  listAdjNo noAux = grafo->lista[src].cab;

  // Variável auxiliar para índices da lista de adjacência e nó recém extraído da lista de prioridade.
  int idxAux, noAtual = src;

  while (heapTam) {
    // Extrai menor valor da fila de prioridade.
    heapList posAtual = ExtraiMenor(listaPrioridade, numVertices, heapTam);
    heapTam -= 1;

    // Posição do nó recém extraído da lista de prioridade
    noAtual = posAtual->chave;

    // Marca vértice como visitado inserindo na lista de nós visitados.
    nosVisitados[posFila] = noAtual;
    posFila += 1;

    // Atualizar variável auxiliar para a lista de adjacências do nó recém extraído da lista de prioridade.
    noAux = grafo->lista[noAtual].cab;

    printf("%d\n", noAtual);

    while (noAux->dest) {
      idxAux = noAux->dest;

      // Realiza o relaxamento das arestas do nó atual
      Relaxamento(listaAuxiliar, noAtual, idxAux, noAux->peso);

      if (verificador[idxAux]) {
        DiminuiPeso(listaPrioridade, idxAux, listaAuxiliar[idxAux]->dist, numVertices, heapTam);
      } else {
        heapTam = InsereFila(listaPrioridade, idxAux, listaAuxiliar[idxAux]->dist, numVertices, heapTam);
        verificador[idxAux] = 1;
      }

      // Atualiza posição da lista para a próxima
      noAux = noAux->prox;
    }
  }

  for (int i = 1; i <= numVertices; i++) {
    printf("Nó: %d => Dist.: %d | Pred.: %d\n", i, listaAuxiliar[i]->dist, listaAuxiliar[i]->pred);
  }
}
#pragma endregion

#pragma region Função Principal
int main () {
  int numVertices, numArestas, vInicio, valA, valB, peso;
  scanf("%d %d %d%*c", &numVertices, &numArestas, &vInicio);

  // Cria grafo com número de vértices 
  // igual a (numVertices).
  graph grafo = CriaGrafo(numVertices);

  // Lê cada aresta adicionando os pares 
  // na lista dos seus devidos vértices.
  for (int i = 0; i < numArestas; i++) {
    scanf("%d %d %d", &valA, &valB, &peso);
    AdicionaAresta(grafo, valA, valB, peso); 
  }

  AlgoritmoDijkstra(grafo, vInicio);
  return 0;
}
#pragma endregion