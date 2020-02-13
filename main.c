/*
Instituto superior tecnico
2019

90732 Joao Lopes
90782 Tomas Gomes
*/
/******************************************************************************/
/*INCLUDES*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/******************************************************************************/
/*ESTRUTURAS*/

typedef struct edge edge_t;

typedef struct vertex{
  int id;         /* Identificador */
  int production; /* Producao ou -1(caso o vertice seja uma estacao de abastecimento) */
  int capacity;   /* Capacidade caso o vertice seja estacao de abastecimento ou -1 (hiper = -2) */
  int value;      /* Quantidade atual */
  int excess;     /* Excesso que precisa de ser mandado para tras */

  edge_t *outEdges;
  //edge_t **outEdges; /* Arestas que partem do vertice */
} vertex_t;

typedef struct edge{
  int weight; /* Capacidade da aresta */
  int flow;   /* Fluxo atual na aresta */

  vertex_t *source;  /* Vertice que esta na origem da aresta */
  vertex_t *target; /* Vertice que esta no fim da aresta */

  struct edge *next;
} edge_t;
/******************************************************************************/
/*FUNCOES*/

void readInput();
void addHiper();
void addSuppliers();
void addStations();
void addLinks();
/******************************************************************************/
/*GLOBAIS*/

int suppliers; /*Numero de fornecedores*/
int stations;  /*Numero de estacoes*/
int links;     /*Numero de ligacoes*/

vertex_t **vertexes; /* Vertices do grafo */
/*edge_t **edges;      /* Arestas do grafo */
/******************************************************************************/
/*PROGRAMA*/

void debug(){
  printf("SW DEBUGGER >>> ");
}

void print(){
  int i;
  edge_t *tmp;

  debug();
  printf("Hiper %d ---> id = %d; value = %d\n", 0, vertexes[0]->id, vertexes[0]->value);
  for(i = 1; i<suppliers + 1;i++){
    debug();
    printf("Suplier %d ---> id = %d; production = %d\n", i, vertexes[i]->id, vertexes[i]->production);
  }
  for(i = suppliers + 1; i<suppliers + stations + 1;i++){
    debug();
    printf("Station %d ---> id = %d; capacity = %d\n", i, vertexes[i]->id, vertexes[i]->capacity);
  }

  for(i = 0; i<1+suppliers+stations;i++){
    if(vertexes[i]->outEdges == NULL){
      debug();
      printf("Vertex with id <%d> has no links!\n", vertexes[i]->id);
      continue;
    }

    tmp = vertexes[i]-> outEdges;
    while (tmp->next != NULL) {
      debug();
      printf("Edge from %d to %d\n", tmp->source->id, tmp->target->id);
      tmp = tmp->next;
    }
    printf("Edge from %d to %d\n", tmp->source->id, tmp->target->id);
  }
}

int main(int argc, char **argv){
  readInput();
  print();

  return 0;
}

void readInput(){
  int i;

  scanf("%d %d %d", &suppliers, &stations, &links);
  vertexes = malloc(sizeof(vertex_t) * (1 + suppliers + stations));
  edges = malloc(sizeof(edge_t) * links);
  addHiper();
  addSuppliers();
  addStations();
  addLinks();
}

void addHiper() { /* adiciona hipermercado (target) a rede */
  vertex_t *hiper;
  hiper = (vertex_t*)malloc(sizeof(vertex_t));
  hiper->id = 1;
  hiper->production = -1;
  hiper->capacity = -2;
  hiper->value = 0;
  hiper->excess = 0;
  vertexes[0] = hiper;
}

void addSuppliers(){ /* Adiciona os fornecedores a rede*/
  int i;

  for(i = 0; i < suppliers; i++){
    vertex_t *supplier;
    supplier = (vertex_t*)malloc(sizeof(vertex_t));
    supplier->id = i + 2;
    scanf("%d", &(supplier->production));
    supplier->capacity = -1;
    supplier->value = supplier->production;
    supplier->excess = 0;
    vertexes[i + 1] = supplier;
  }
}

void addStations(){ /* Adiciona os fornecedores a rede*/
  int i;

  for(i = 0; i < stations; i++){
    vertex_t *station;
    station = (vertex_t*)malloc(sizeof(vertex_t));
    station->id = i + suppliers + 2;
    scanf("%d", &(station->capacity));
    station->production = -1;
    station->value = 0;
    station->excess = 0;
    vertexes[i + 1 + suppliers] = station;
  }
}

void addLinks(){
  int i, source, target, capacity;
  edge_t *link, *tmp;

  for(i = 0; i<links; i++){
    scanf("%d %d %d", &source, &target, &capacity);
    link = (edge_t*)malloc(sizeof(edge_t));
    link->weight = capacity;
    link->flow = 0;
    link->source = vertexes[source - 1];
    link->target = vertexes[target - 1];
    link->next = NULL;

    /*Adiciona ligacao ao fim da lista de ligacoes. Cada vertice sabe quais as
    ligacoes que partem dele*/
    if(vertexes[source-1]-> outEdges == NULL){
      vertexes[source-1]-> outEdges = link;
      continue;
    }
    tmp = vertexes[source-1]-> outEdges;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = link;
  }
}
