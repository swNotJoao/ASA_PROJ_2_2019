/*
  João Lopes
  90732
  Tomás Gomes
  90782
  ASA - 2º Projecto
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define MIN(X,Y) X < Y ? X : Y
#define INF 10000000
/* - - - - - - - ESTRUTURAS - - - - - - - */
typedef struct aresta aresta_t;

typedef struct vertice {
  int id; /* não sei ainda se é necessário */
  int excesso;
  int altura;
  int curr_aresta;
  int total_arestas;
  aresta_t *ligacoes;
  char visited;
} vertice_t;

typedef struct aresta{
  int id; /* não sei ainda se é necessário */
  int capacidade;
  int fluxo;
  char type;
  vertice_t *origem;
  vertice_t *destino;
  struct aresta *next;
  char corte;
} aresta_t;

/* - - - - - -VARIÁVEIS GLOBAIS - - - - - */
int num_fornecedores;
int num_estacoes;
int num_lig;
vertice_t **vertices;
aresta_t *arestas;
/* - - - - - - - -FUNÇÕES - - - - - - - - */
void lerInput();
void addHiper();
void addFornecedor();
void addEstacao();
void criaLigacao();
void addSource();
aresta_t *getAresta(int u, int v);
void push(int u, int v);
void relabel(int u);
void discharge(int u);
void moveToFront(int i, int *qeue);
int pushRelabel(int source, int sink);
void DFS();
void visit();
void addAresta();

/* - - - - - - -MAIN E CÓDIGO - - - - - - */
int main() {
  int maxF;
  int i;
  aresta_t *a, *tmp;

  lerInput();
  maxF = pushRelabel(0,1);
  printf("%d\n", maxF);
  DFS();
  for(i = 2 + num_fornecedores; i < 2 + num_fornecedores + num_estacoes; i++) {
    if(vertices[i]->visited != vertices[i + num_estacoes]->visited) printf("%d ", i);
  }
  printf("\n");

  for(i=1; i<2 + num_estacoes*2 + num_fornecedores; i++) {
    a = vertices[i]->ligacoes;
    while(a != NULL) {
      if(a->type == 1 && (a->origem)->visited != (a->destino)->visited && !((a->origem)->id > 1 + num_estacoes && (a->origem)->id < 2 + num_estacoes + num_fornecedores)) {
        if((a->origem)->id > 1 + num_fornecedores + num_estacoes) (a->origem)->id -= num_estacoes;
        addAresta(a);
        }
      a = a->next;
    }
  }

  /*for(i=0;i<NODES;i++){*/
    tmp = arestas;
    while(tmp!=NULL){
      /*if(tmp->origem->id == i){*/
        printf("%d %d\n", tmp->origem->id, tmp->destino->id);

      tmp = tmp->next;
    }
  /*}*/

  return 0;
}
void addAresta(aresta_t* a) {
  aresta_t *aux = a;
  aresta_t *aux2 = arestas;
  aux->next = NULL;
  if(arestas == NULL) arestas = aux;
  else {
    while(aux2->next != NULL) aux2 = aux2->next;
    aux2->next = aux;
  }
}
void lerInput() {
  int i, o, d, c;
  scanf("%d %d %d", &num_fornecedores, &num_estacoes, &num_lig);
  vertices = malloc(sizeof(vertice_t) * (2 + num_fornecedores + num_estacoes * 2));
  addSource();
  addHiper();
  for(i = 1; i <= num_fornecedores; i++) {
    addFornecedor(i+1);
  }
  for(; i <= num_fornecedores + num_estacoes; i++) {
    addEstacao(i+1);
  }
  for(i = 0; i < num_lig; i++) {
    scanf("%d %d %d", &o, &d, &c);
    criaLigacao(o, d, c, 1);
  }
}
void addSource() {
  vertice_t *s;
  s = (vertice_t*)malloc(sizeof(vertice_t));
  s->id = 0;
  s->excesso = 0;
  s->altura = num_fornecedores + num_estacoes * 2 + 2;
  s->ligacoes = NULL;
  s->curr_aresta = 0;
  s->total_arestas = 0;
  s->visited = 0;
  vertices[0] = s;
}
void addHiper() {
  vertice_t *h;
  h = (vertice_t*)malloc(sizeof(vertice_t));
  h->id = 1;
  h->excesso = 0;
  h->altura = 0;
  h->ligacoes = NULL;
  h->curr_aresta = 0;
  h->total_arestas = 0;
  h->visited = 1;
  vertices[1] = h;
}
void addFornecedor(int id) {
  int producao;
  vertice_t *f;
  scanf("%d", &producao);
  f = (vertice_t*)malloc(sizeof(vertice_t));
  f->id = id;
  f->excesso = 0;
  f->altura = 0;
  f->ligacoes = NULL;
  f->curr_aresta = 0;
  f->total_arestas = 0;
  f->visited = 0;
  vertices[id] = f;
  criaLigacao(0, id, producao, 1);
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
  e1->curr_aresta = 0;
  e1->total_arestas = 0;
  e1->visited = 0;
  e2->id = id + num_estacoes;
  e2->excesso = 0;
  e2->altura = 0;
  e2->ligacoes = NULL;
  e2->curr_aresta = 0;
  e2->total_arestas = 0;
  e2->visited = 0;
  vertices[id] = e1;
  vertices[e2->id] = e2;
  criaLigacao(e1->id, e2->id, capacidade, 0);
}
void criaLigacao(int id1, int id2, int capacidade, int y) {
  aresta_t *a1 = (aresta_t*)malloc(sizeof(aresta_t));
  aresta_t *a2 = (aresta_t*)malloc(sizeof(aresta_t));
  aresta_t *e1;
  aresta_t *e2;

  if(id1 > 1 + num_fornecedores && y) id1 += num_estacoes;

  a1->id = id1;
  a1->capacidade = capacidade;
  a1->fluxo = 0;
  a1->origem = vertices[id1];
  a1->destino = vertices[id2];
  a1->next = NULL;
  a1->type = 1;
  a1->corte = 0;
  vertices[id1]->total_arestas += 1;

  a2->id = id2;
  a2->capacidade = capacidade;
  a2->fluxo = capacidade;
  a2->origem = vertices[id2];
  a2->destino = vertices[id1];
  a2->next = NULL;
  a2->type = 0;
  a2->corte = 0;
  vertices[id2]->total_arestas += 1;

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
aresta_t *getCurrentAresta(int u, int v){
  vertice_t *aux = vertices[u];
  aresta_t *a = aux->ligacoes;
  int i;

  for(i = 0; i < v; i++) a = a->next;
  return a;
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

}

void relabel(int u) {
  int v, min_height = INF;
  aresta_t *aresta;
  int NODES = vertices[u]->total_arestas;

  for (v = 0; v < NODES; v++) {
    aresta = getCurrentAresta(u, v);
    if(aresta != NULL){
      if (aresta->capacidade - aresta->fluxo > 0) {
        min_height = MIN(min_height, (aresta->destino)->altura);
        vertices[u]->altura = min_height + 1;
      }
    }
  }
}

void discharge(int u) {
  int v /*z*/;
  aresta_t *aresta;
  /*int NODES = 2 + (num_estacoes*2) + num_fornecedores;*/
  while (vertices[u]->excesso > 0) {
    if (vertices[u]->curr_aresta < vertices[u]->total_arestas) {
      v = vertices[u]->curr_aresta;
      /*z = vertices[u]->total_arestas;*/
      aresta = getCurrentAresta(u, v);
      if ((aresta->capacidade - aresta->fluxo > 0) && (vertices[u]->altura == (aresta->destino)->altura + 1)){
        push(u, (aresta->destino)->id);
        vertices[u]->curr_aresta += 1;
      }
      else vertices[u]->curr_aresta += 1;
    }
    else {
      relabel(u);
      vertices[u]->curr_aresta = 0;
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
  int i, p, u, old_height;
  int NODES = 2 + num_estacoes*2 + num_fornecedores;
  int list[NODES-2];

  for (i = 0, p = 0; i < NODES; i++){
    if((i != source) && (i != sink)) {
      list[p] = i;
      p++;
    }
  }

  vertices[source]->altura = NODES;
  vertices[source]->excesso = INF;
  /*Pre-flow*/
  for (i = 0; i < NODES; i++) push(source, i);


  p = 0;
  while (p < NODES - 2) {
    u = list[p];
    old_height = vertices[u]->altura;
    discharge(u);
    if (vertices[u]->altura > old_height) {
      moveToFront(p,list);
      p=0;
    }
    else
      p += 1;
  }
  return vertices[1]->excesso;
}
void DFS() {
  visit(1);
}
void visit(int id) {
  vertice_t *v = vertices[id];
  aresta_t *a = v->ligacoes;
  vertice_t *filho;
  if(id == 0) return;
  while(1) {
    if(a == NULL) break;
    filho = a->destino;
    if(filho->visited == 0) {
      if(a->type == 0 && a->fluxo != 0 && (a->destino)->id != 0) {
        filho->visited = 1;
        visit(filho->id);
      }
      else if(a->type == 1 && a->fluxo > 0) {
        filho->visited = 1;
        visit(filho->id);
      }
    }
    a = a->next;
  }
}
