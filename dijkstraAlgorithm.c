//
//  main.c
//  DijkstraAlgorithm
//
//  Created by João Victor Almeida Santana on 04/12/2017.
//  Copyright © 2017 João Victor Almeida Santana. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

struct edge{
    int index;
    double weight;
    struct edge *next;
};
typedef struct edge *Edge;

struct graph{
    int vertex;
    int edges;
    Edge *nodes;
};
typedef struct graph *Graph;

int length = 0;

Graph createGraph(int);
void insertEdges(Graph, int, int, int);
Edge newNode(int, int, Edge);
void deleteGraph(Graph);
int **dijkstraAlgorithm(Graph, int);
void iniciation(Graph, int*, int*, int);
int existOpens(Graph, int*);
int shorterDistance(Graph, int*, int*);
void relaxVertex(Graph, int*, int*, int, int);
int calculateCost(int*, int*);
void printCost(int*, int*, int, int);
int getPreceders(int*, int*, int, int);

int main(int argc, const char * argv[]) {
    Graph graph = createGraph(6);
    
    insertEdges(graph, 0, 1, 10);
    insertEdges(graph, 0, 2, 5);
    insertEdges(graph, 1, 3, 1);
    insertEdges(graph, 2, 1, 3);
    insertEdges(graph, 2, 3, 8);
    insertEdges(graph, 2, 4, 2);
    insertEdges(graph, 3, 4, 4);
    insertEdges(graph, 3, 5, 4);
    insertEdges(graph, 4, 5, 6);
    
    int inicialVertex = 0;
    int finalVertex = 3;
    
    int **results = dijkstraAlgorithm(graph, inicialVertex);
    
    printCost(results[0], results[1], inicialVertex, finalVertex);
    
    for(int i = 0; i < 2; i ++)
        free(results[i]);
    free(results);
    deleteGraph(graph);
    
    return 0;
}

Graph createGraph(int vertex){
    Graph graph = (Graph)malloc(sizeof(Graph));
    graph->vertex = vertex;
    graph->edges = 0;
    
    graph->nodes = (Edge*)malloc(graph->vertex * sizeof(Edge));
    for(int i = 0; i < graph->vertex; i ++)
        graph->nodes[i] = NULL;
    
    return graph;
}

void insertEdges(Graph graph, int vertexA, int vertexB, int weight){
    for(Edge node = graph->nodes[vertexA]; node != NULL; node = node->next)
        if(node->index == vertexB) return;
    
    graph->nodes[vertexA] = newNode(vertexB, weight, graph->nodes[vertexA]);
    graph->edges ++;
    
}

Edge newNode(int vertex, int weight, Edge next){
    Edge node = (Edge)malloc(sizeof(Edge));
    node->index = vertex;
    node->weight = weight;
    node->next = next;
    
    return node;
}

void deleteGraph(Graph graph){
    for(int i = 0; i < graph->vertex; i ++)
        free(graph->nodes[i]);
    
    free(graph->nodes);
    free(graph);
}

int **dijkstraAlgorithm(Graph graph, int inicialVertex){
    int *distances = (int*)malloc(graph->vertex * sizeof(int));
    int *preceders = (int*)malloc(graph->vertex * sizeof(int));
    int *opens = (int*)malloc(graph->vertex * sizeof(int));
    
    iniciation(graph, distances, preceders, inicialVertex);
    
    for(int i = 0; i < graph->vertex; i ++)
        opens[i] = 1;
    
    while(existOpens(graph, opens) == 1){
        int u = shorterDistance(graph, distances, opens);
        opens[u] = 0;
        
        for(Edge aux = graph->nodes[u]; aux != NULL; aux = aux->next){
            relaxVertex(graph, distances, preceders, u, aux->index);
        }
    }
    
    free(opens);
    
    int **results = (int**)malloc(2 * sizeof(int*));
    
    results[0] = distances;
    results[1] = preceders;
    
    return results;
}

void iniciation(Graph graph, int* distances, int* preceders, int inicialVertex){
    for(int i = 0; i < graph->vertex; i ++){
        distances[i] = 999999;
        preceders[i] = -1;
    }
    
    distances[inicialVertex] = 0;
}

int existOpens(Graph graph, int* opens){
    for(int i = 0; i < graph->vertex; i ++)
        if(opens[i] == 1) return 1;
    
    return 0;
}

int shorterDistance(Graph graph, int* distances, int* opens){
    int less = 99999;
    int position = 0;
    
    for(int i = 0; i < graph->vertex; i ++){
        if(opens[i] == 1){
            if(distances[i] < less){
                less = distances[i];
                position = i;
            }
        }
    }
    return position;
}

void relaxVertex(Graph graph, int* distances, int* preceders, int u, int index){
    Edge aux;
    for(aux = graph->nodes[u]; aux != NULL && aux->index != index; aux = aux->next);
    
    if(aux){
        if(distances[index] > distances[u] + aux->weight){
            distances[index] = distances[u] + aux->weight;
            preceders[index] = u;
        }
    }
}

void printCost(int* distances, int* preceders, int inicialVertex, int finalVertex){
    int *result = (int*)malloc(10 * sizeof(int));
    
    length = 1;
    
    result[0] = finalVertex;
    
    getPreceders(preceders, result, inicialVertex, finalVertex);
    
    printf("Vetor resultado com os vertices -> ");
    for(int i = length; i >= 0; i --)
        printf("%i ", result[i]);
    printf("\n");
    
    printf("Custo do caminho -> ");
    printf("%i\n", calculateCost(distances, result));
    
    free(result);
}

int calculateCost(int* distances, int* results){
    int value = 0;
    
    for(int i = 0; i < length; i ++)
        value += distances[results[i]];
    
    return value;
}

int getPreceders(int* preceders, int* result, int inicialVertex, int finalVertex){
    result[length] = preceders[finalVertex];

    if(result[length] == inicialVertex)
        return 1;

    length += 1;

    getPreceders(preceders, result, inicialVertex, preceders[finalVertex]);

    return 0;
}

