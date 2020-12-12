#include <stdio.h>
#include <stdlib.h>

struct HeapList {
  int chave;
  int peso;
};
typedef struct HeapList *heapList;

heapList AlocaHeapLista() {
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

void ConstroiFilaPrioridade(heapList vet[], int num, int heapTam) {
  for(int i = heapTam/2; i>=1; i--) {
    HeapfyMinimo(vet, i, num, heapTam);
  }
}

int Minimo(int vet[]) {
  return vet[1];
}

heapList ExtraiMenor(heapList vet[], int num, int heapTam) {
  heapList menor = vet[1];
  vet[1] = vet[heapTam];
  heapTam--;
  HeapfyMinimo(vet, 1, num, heapTam);
  return menor;
}

void DiminuiPeso(heapList vet[], int indice, int peso, int num, int chave) {
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
  vet[heapTam] = AlocaHeapLista();
  vet[heapTam]->peso = 100000;
  DiminuiPeso(vet, heapTam, peso, num, chave);
  return heapTam;
}

void ImprimeFila(heapList vet[], int heapTam) {
  for(int i = 1; i <= heapTam; i++) {
    printf("%d\n",vet[i]->peso);
  }
  printf("\n");
}

int main() {
  int num = 10;
  int heapTam = 0;

  heapList vet[num];
  heapTam = InsereFila(vet, 1, 150, num, heapTam);
  heapTam = InsereFila(vet, 2, 200, num, heapTam);
  heapTam = InsereFila(vet, 3, 100, num, heapTam);
  heapTam = InsereFila(vet, 3, 215, num, heapTam);
  heapTam = InsereFila(vet, 5, 310, num, heapTam);
  heapTam = InsereFila(vet, 6, 117, num, heapTam);
  heapTam = InsereFila(vet, 7, 224, num, heapTam);
  heapTam = InsereFila(vet, 8, 392, num, heapTam);
  heapTam = InsereFila(vet, 9, 142, num, heapTam);
  heapTam = InsereFila(vet, 10, 139, num, heapTam);

  printf("HeapTam: %d\n", heapTam);
  ImprimeFila(vet, heapTam);

  heapList val;

  for (int i = 0; i < num; i++) {
    val = ExtraiMenor(vet, num, heapTam);
    heapTam -= 1;
    printf("Menor: Chave = %d | Peso = %d\n", val->chave, val->peso);
  }

  printf("HeapTam: %d\n", heapTam);

  return 0;
}