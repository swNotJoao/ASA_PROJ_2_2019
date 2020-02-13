/*
  João Lopes
  90732
  Tomás Gomes
  90782
  ASA - 2º Projecto
*/
#include <stdlib.h>
#include <stdio.h>
#define MIN(X,Y) X < Y ? X : Y
#define INF 10000000
/* - - - - - - - ESTRUTURAS - - - - - - - */
typedef struct aresta aresta_t;

typedef struct vertice {
  int id; /* não sei ainda se é necessário */
  int excesso;
  int altura;
  int visitado;
  aresta_t *ligacoes;
} vertice_t;

typedef struct aresta{
  int id; /* não sei ainda se é necessário */
  int capacidade;
  int fluxo;
  vertice_t *origem;
  vertice_t *destino;
  struct aresta *next;
} aresta_t;

/* - - - - - -VARIÁVEIS GLOBAIS - - - - - */
int num_fornecedores;
int num_estacoes;
int num_lig;
vertice_t **vertices;

/* - - - - - - - -FUNÇÕES - - - - - - - - */
void lerInput();
void addHiper();
void addFornecedor();
void addEstacao();
void criaLigacao();
void addSource();
void printRede();
aresta_t *getAresta(int u, int v);
void push(int u, int v);
void relabel(int u);
void discharge(int u);
void moveToFront(int i, int *qeue);
int pushRelabel(int source, int sink);

/* - - - - - - -MAIN E CÓDIGO - - - - - - */
int main() {
  int l[] = {1,2,3,4,5};

  moveToFront(4, l);
  printf("\n\n\n\n\n[%d,%d,%d,%d,%d]\n\n\n\n", l[0], l[1], l[2], l[3], l[4]);

  int maxF;
    lerInput();
    maxF = pushRelabel(0,1);
    printf("\n\nFLUXO === %d\n\n", maxF);
    printRede();
    return 0;
}
void lerInput() {
  int i, o, d, c;
  scanf("%d %d %d", &num_fornecedores, &num_estacoes, &num_lig);
  vertices = malloc(sizeof(vertice_t) * (2 + num_fornecedores + num_estacoes * 2));
  addSource();
  addHiper();
  for(i = 1; i <= num_fornecedores; i++) {
    //printf("f %d\n", i);
    addFornecedor(i+1);
  }
  for(; i <= num_fornecedores + num_estacoes; i++) {
    //printf("e %d\n", i);
    addEstacao(i+1);
  }
  for(i = 0; i < num_lig; i++) {
    scanf("%d %d %d", &o, &d, &c);
    criaLigacao(o, d, c);
  }
}
void addSource() {
  vertice_t *s = (vertice_t*)malloc(sizeof(vertice_t));
  s->id = 0;
  s->excesso = 0;
  s->altura = num_fornecedores + num_estacoes + 2;
  s->ligacoes = NULL;
  s->visitado = 0;
  vertices[0] = s;
}
void addHiper() {
  vertice_t *h = (vertice_t*)malloc(sizeof(vertice_t));
  h->id = 1;
  h->excesso = -1;
  h->altura = 0;
  h->ligacoes = NULL;
  h->visitado = 0;
  vertices[1] = h;
}
void addFornecedor(int id) {
  int producao;
  scanf("%d", &producao);
  vertice_t *f = (vertice_t*)malloc(sizeof(vertice_t));
  f->id = id;
  f->excesso = 0;
  f->altura = 0;
  f->ligacoes = NULL;
  f->visitado = 0;
  vertices[id] = f;
  criaLigacao(0, id, producao);
}
void addEstacao(int id) {
  int capacidade;
  scanf("%d", &capacidade);
  vertice_t *e1 = (vertice_t*)malloc(sizeof(vertice_t));
  vertice_t *e2 = (vertice_t*)malloc(sizeof(vertice_t));
  e1->id = id;
  e1->excesso = 0;
  e1->altura = 0;
  e1->ligacoes = NULL;
  e1->visitado = 0;
  e2->id = id + num_estacoes;
  e2->excesso = 0;
  e2->altura = 0;
  e2->ligacoes = NULL;
  e2->visitado = 0;
  vertices[id] = e1;
  vertices[e2->id] = e2;
  criaLigacao(e1->id, e2->id, capacidade);
}
void criaLigacao(int id1, int id2, int capacidade) {
  aresta_t *a1 = (aresta_t*)malloc(sizeof(aresta_t));
  aresta_t *a2 = (aresta_t*)malloc(sizeof(aresta_t));
  aresta_t *e1;
  aresta_t *e2;

  if(id1 > 1 + num_fornecedores) id1 += num_estacoes;

  a1->id = id1;
  a1->capacidade = capacidade;
  a1->fluxo = 0;
  a1->origem = vertices[id1];
  a1->destino = vertices[id2];
  a1->next = NULL;

  a2->id = id2;
  a2->capacidade = 0;
  a2->fluxo = 0;
  a2->origem = vertices[id2];
  a2->destino = vertices[id1];
  a2->next = NULL;

  e1 = vertices[id1]->ligacoes;
  e2 = vertices[id2]->ligacoes;

  if(e1 == NULL) vertices[id1]->ligacoes = a1;
  else {
    while(e1->next != NULL) e1 = e1->next;
    e1->next = a1;
  }
  if(e2 == NULL) vertices[id2]->ligacoes = a2;
  else {
    while(e2->next != NULL) e2 = e2->next;
    e2->next = a2;
  }
}

void debug(){
  printf("SW DEBUGGER >>> ");
}

void printRede(){
  int i;
  aresta_t *tmp;

  debug();
  printf("Source %d ---> id = %d; id = %d\n", 0, vertices[0]->id, vertices[0]->id);
  debug();
  printf("Hiper %d ---> id = %d; id = %d\n", 1, vertices[1]->id, vertices[1]->id);
  for(i = 2; i<=num_fornecedores + 1;i++){
    debug();
    printf("Suplier %d ---> id = %d; id = %d\n", i, vertices[i]->id, vertices[i]->id);
  }
  for(i = num_fornecedores + 2; i<num_fornecedores + num_estacoes*2 + 2;i++){
    debug();
    printf("Station %d ---> id = %d; id = %d\n", i, vertices[i]->id, vertices[i]->id);
  }

  for(i = 0; i<num_fornecedores+num_estacoes*2 + 2;i++){
    if(vertices[i]->ligacoes == NULL){
      debug();
      printf("Vertex with id <%d> has no links!\n", vertices[i]->id);
      continue;
    }

    tmp = vertices[i]->ligacoes;
    while (tmp->next != NULL) {
      debug();
      printf("Edge from %d to %d\n", tmp->origem->id, tmp->destino->id);
      tmp = tmp->next;
    }
    debug();
    printf("Edge from %d to %d\n", tmp->origem->id, tmp->destino->id);
  }
}

/*Retorna a aresta (u,v)*/
aresta_t *getAresta(int u, int v){
  vertice_t *aux = vertices[u];
  aresta_t *res = NULL, *tmp;

  tmp = aux->ligacoes;
  while(tmp!=NULL){
    if(tmp->origem->id == u && tmp->destino->id == v){
      res = tmp;
      break;
    }
    tmp = tmp->next;
  }
  return res;
}

void push(int u, int v) {
  int send;
  aresta_t *aresta = getAresta(u, v);
  aresta_t *arestaRev = getAresta(v, u);

  if(aresta != NULL && arestaRev != NULL){
    send = MIN(vertices[u]->excesso, aresta->capacidade - aresta->fluxo);
    aresta->fluxo += send;
    arestaRev->fluxo -= send;
    vertices[u]->excesso -= send;
    vertices[v]->excesso += send;
  }
  if(aresta != NULL)
  printf("ARESTA %d-->%d ----  %d/%d\n", aresta->origem->id, aresta->destino->id, aresta->fluxo, aresta->capacidade);
}

void relabel(int u) {
  int v, min_height = INF;
  aresta_t *aresta;
  int NODES = 2 + num_estacoes*2 + num_fornecedores;

  for (v = 0; v < NODES; v++) {
    aresta = getAresta(u, v);
    if(aresta != NULL){
      if (aresta->capacidade - aresta->fluxo > 0) {
        min_height = MIN(min_height, vertices[v]->altura);
        vertices[u]->altura = min_height + 1;
      }
    }
  }
}

void discharge(int u) {
  int v;
  aresta_t *aresta;
  int NODES = 2 + (num_estacoes*2) + num_fornecedores;

  while (vertices[u]->excesso > 0) {
    if (vertices[u]->visitado < NODES) {
      v = vertices[u]->visitado;
      aresta = getAresta(u, v);
      if ((aresta != NULL) && (aresta->capacidade - aresta->fluxo > 0) && (vertices[u]->altura > vertices[v]->altura)){
        push(u, v);
      }
      else{
        vertices[u]->visitado += 1;
      }
    }
    else {
    relabel(u);
    vertices[u]->visitado = 0;
    }
  }
}

void moveToFront(int i, int *qeue) {
  int temp = qeue[i], n;

  for (n = i; n > 0; n--){
    qeue[n] = qeue[n-1];
  }
  qeue[0] = temp;
}

int pushRelabel(int source, int sink) {
  int i, p;
  int NODES = 2 + num_estacoes*2 + num_fornecedores;

  int list[NODES-2];

  for (i = 0, p = 0; i < NODES; i++){
    if((i != source) && (i != sink)) {
      list[p] = i;
      printf("Lista %d\n", list[p]);
      p++;
    }
  }

  vertices[source]->altura = NODES;
  vertices[source]->excesso = INF;
  /*Pre-flow*/
  for (i = 0; i < NODES; i++){
    push(source, i);
    if(getAresta(source,i) != NULL)
    printf("Fluxoasdasdasda : %d \n", getAresta(source,i)->fluxo);
  }


  p = 0;
  while (p < NODES - 2) {
    int u = list[p];
    int old_height = vertices[u]->altura;
    discharge(u);
    if (vertices[u]->altura > old_height) {
      moveToFront(p,list);
      printf("ListaORDEM %d ======== %d\n", list[0], u);
      p=0;
    }
    else
      p += 1;
  }
  int maxflow = 0;
  for (i = 0; i < NODES; i++){
    aresta_t *aresta = getAresta(source, i);
    if(aresta !=NULL){
    maxflow += aresta->fluxo;
    debug();
    printf("Fluxo : %d \n", aresta->fluxo);
    }
  }

  printf("Fluxo : %d \n", getAresta(4,1)->fluxo);
  printf("Fluxo : %d \n", getAresta(6,1)->fluxo);

  return maxflow;
}
