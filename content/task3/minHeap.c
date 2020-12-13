#include <stdio.h>
#include <stdlib.h>

int valMaximo = 10000;
int heapTam = 0;

struct HeapList {
  int chave;
  int peso;
};
typedef struct HeapList *heapList;

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

int RetornaFilhoDireita(int indice, int num) {
  if((((2*indice)+1) < num) && (indice >= 1))
    return (2*indice) + 1;
  return -1;
}

int RetornaFilhoEsquerda(int indice, int num) {
  if(((2*indice) < num) && (indice >= 1))
    return 2*indice;
  return -1;
}

int RetornaPai(int indice, int num) {
  if ((indice > 1) && (indice <= num)) {
    return indice/2;
  }
  return -1;
}

void HeapfyMinimo(heapList vet[], int indice, int num) {
  int fe = RetornaFilhoEsquerda(indice, num);   // Índice do Filho da Esquerda
  int fd = RetornaFilhoDireita(indice, num);    // Índice do Filho da Direita

  int minVertice = indice;

  // Testa filho da esquerda
  if (fe <= heapTam && fe > 0) {
    if (vet[fe]->peso < vet[minVertice]->peso) {
      minVertice = fe;
    }
  }

  // Testa filho da direita
  if (fd <= heapTam && fd > 0) {
    if (vet[fd]->peso < vet[minVertice]->peso) {
      minVertice = fd;
    }
  }

  if (minVertice != indice) {
    TrocaPosicao(vet, indice, minVertice);
    HeapfyMinimo(vet, minVertice, num);
  }
}

heapList ExtraiMenor(heapList vet[], int num) {
  heapList menor = vet[1];
  vet[1] = vet[heapTam];
  heapTam--;
  HeapfyMinimo(vet, 1, num);
  return menor;
}

void DiminuiPeso(heapList vet[], int chave, int peso, int num, int indice) {
  int pai = RetornaPai(indice, num);

  vet[indice]->peso = peso;
  vet[indice]->chave = chave;

  while((indice > 1) && (vet[pai]->peso > vet[indice]->peso)) {
    TrocaPosicao(vet, indice, pai);
    indice = RetornaPai(indice, num);
  }
}

void InsereFila(heapList vet[], int chave, int peso, int num) {
  heapTam++;
  vet[heapTam] = AlocaListaHeap();
  vet[heapTam]->peso = valMaximo;
  DiminuiPeso(vet, chave, peso, num, heapTam);
}

void ImprimeFila(heapList vet[]) {
  for(int i = 1; i <= heapTam; i++) {
    printf("C: %d | P: %d\n", vet[i]->chave, vet[i]->peso);
  }
  printf("\n");
}

int main() {
  int num = 9;

  heapList vet[num];
  InsereFila(vet, 3, 6, num);
  InsereFila(vet, 2, 4, num);
  InsereFila(vet, 1, 0, num);
  InsereFila(vet, 5, 13, num);
  InsereFila(vet, 7, 12, num);
  InsereFila(vet, 4, 13, num);
  InsereFila(vet, 9, 11, num);
  InsereFila(vet, 6, 8, num);
  InsereFila(vet, 8, 9, num);
  // InsereFila(vet, 10, 139, num);

  printf("HeapTam: %d\n", heapTam);
  ImprimeFila(vet);

  heapList val;

  for (int i = 0; i < num; i++) {
    val = ExtraiMenor(vet, num);
    printf("Menor: Chave = %d | Peso = %d\n", val->chave, val->peso);
  }

  printf("HeapTam: %d\n", heapTam);

  return 0;
}