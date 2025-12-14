#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "AL5.h"

// Функции для работы с графом
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    
    for (int i = 1; i <= vertices; i++) {
        graph->edges[i] = NULL;
    }
    
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = graph->edges[src];
    graph->edges[src] = newEdge;
}

// Вспомогательные функции для алгоритма Дейкстры
MinHeapNode* newMinHeapNode(int vertex, int distance) {
    MinHeapNode* node = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    node->vertex = vertex;
    node->distance = distance;
    return node;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->pos = (int*)malloc((capacity + 1) * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (MinHeapNode**)malloc((capacity + 1) * sizeof(MinHeapNode*));
    return heap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx;
    int right = 2 * idx + 1;
    
    if (left <= heap->size && heap->array[left]->distance < heap->array[smallest]->distance) {
        smallest = left;
    }
    
    if (right <= heap->size && heap->array[right]->distance < heap->array[smallest]->distance) {
        smallest = right;
    }
    
    if (smallest != idx) {
        MinHeapNode* smallestNode = heap->array[smallest];
        MinHeapNode* idxNode = heap->array[idx];
        
        heap->pos[smallestNode->vertex] = idx;
        heap->pos[idxNode->vertex] = smallest;
        
        swapMinHeapNode(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest);
    }
}

int isEmpty(MinHeap* heap) {
    return heap->size == 0;
}

MinHeapNode* extractMin(MinHeap* heap) {
    if (isEmpty(heap)) {
        return NULL;
    }
    
    MinHeapNode* root = heap->array[1];
    MinHeapNode* lastNode = heap->array[heap->size];
    
    heap->array[1] = lastNode;
    heap->pos[root->vertex] = heap->size;
    heap->pos[lastNode->vertex] = 1;
    
    heap->size--;
    minHeapify(heap, 1);
    
    return root;
}

void decreaseKey(MinHeap* heap, int vertex, int distance) {
    int i = heap->pos[vertex];
    heap->array[i]->distance = distance;
    
    while (i > 1 && heap->array[i]->distance < heap->array[i/2]->distance) {
        heap->pos[heap->array[i]->vertex] = i/2;
        heap->pos[heap->array[i/2]->vertex] = i;
        swapMinHeapNode(&heap->array[i], &heap->array[i/2]);
        i = i/2;
    }
}

int isInMinHeap(MinHeap* heap, int vertex) {
    return heap->pos[vertex] <= heap->size;
}

// Алгоритм Дейкстры
void dijkstra(Graph* graph, int src, int dest) {
    int V = graph->numVertices;
    int dist[MAX_VERTICES + 1];
    int parent[MAX_VERTICES + 1];
    
    MinHeap* heap = createMinHeap(V);
    
    for (int v = 1; v <= V; v++) {
        dist[v] = INF;
        parent[v] = -1;
        heap->array[v] = newMinHeapNode(v, dist[v]);
        heap->pos[v] = v;
    }
    
    heap->array[src] = newMinHeapNode(src, dist[src]);
    heap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);
    
    heap->size = V;
    
    while (!isEmpty(heap)) {
        MinHeapNode* minNode = extractMin(heap);
        int u = minNode->vertex;
        
        Edge* edge = graph->edges[u];
        while (edge != NULL) {
            int v = edge->dest;
            
            if (isInMinHeap(heap, v) && dist[u] != INF && edge->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + edge->weight;
                parent[v] = u;
                decreaseKey(heap, v, dist[v]);
            }
            edge = edge->next;
        }
        free(minNode);
    }
    
    // Вывод результата
    printf("Алгоритм Дейкстры:\n");
    printf("Кратчайшее расстояние от %d до %d: %d\n", src, dest, dist[dest]);
    
    if (dist[dest] == INF) {
        printf("Путь не существует\n");
    } else {
        // Восстановление пути
        int path[MAX_VERTICES + 1];
        int pathLength = 0;
        int current = dest;
        
        while (current != -1) {
            path[pathLength++] = current;
            current = parent[current];
        }
        
        printf("Кратчайший путь: ");
        for (int i = pathLength - 1; i >= 0; i--) {
            printf("%d", path[i]);
            if (i > 0) printf(" → ");
        }
        printf("\n");
    }
    
    free(heap->pos);
    free(heap->array);
    free(heap);
}

// Алгоритм Флойда-Уоршелла
void floydWarshall(int graph[MAX_VERTICES + 1][MAX_VERTICES + 1], int src, int dest, int n) {
    int dist[MAX_VERTICES + 1][MAX_VERTICES + 1];
    int next[MAX_VERTICES + 1][MAX_VERTICES + 1];
    
    // Инициализация
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            dist[i][j] = graph[i][j];
            if (graph[i][j] != INF && i != j) {
                next[i][j] = j;
            } else {
                next[i][j] = -1;
            }
        }
    }
    
    // Алгоритм Флойда-Уоршелла
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    
    // Вывод результата
    printf("\nАлгоритм Флойда-Уоршелла:\n");
    printf("Кратчайшее расстояние от %d до %d: %d\n", src, dest, dist[src][dest]);
    
    if (dist[src][dest] == INF) {
        printf("Путь не существует\n");
    } else {
        // Восстановление пути
        printf("Кратчайший путь: ");
        
        if (next[src][dest] == -1) {
            printf("Нет пути");
        } else {
            int u = src;
            printf("%d", u);
            
            while (u != dest) {
                u = next[u][dest];
                printf(" → %d", u);
            }
        }
        printf("\n");
    }
}

// Функция для освобождения памяти графа
void freeGraph(Graph* graph) {
    for (int i = 1; i <= graph->numVertices; i++) {
        Edge* edge = graph->edges[i];
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(graph);
}

int AL5() {
    int vertices = 9;
    int edges = 15;
    
    // Создание графа для алгоритма Дейкстры
    Graph* graph = createGraph(vertices);
    
    // Матрица смежности для алгоритма Флойда
    int adjMatrix[MAX_VERTICES + 1][MAX_VERTICES + 1];
    
    // Инициализация матрицы
    for (int i = 1; i <= vertices; i++) {
        for (int j = 1; j <= vertices; j++) {
            if (i == j) {
                adjMatrix[i][j] = 0;
            } else {
                adjMatrix[i][j] = INF;
            }
        }
    }
    
    // Ввод рёбер (данные из условия)
    int edgeData[15][3] = {
        {1, 7, 1},
        {1, 9, 20},
        {2, 3, 5},
        {2, 5, 4},
        {2, 8, 3},
        {2, 1, 6},
        {3, 7, 2},
        {4, 9, 6},
        {4, 1, 6},
        {5, 6, 9},
        {5, 7, 6},
        {6, 7, 2},
        {7, 8, 4},
        {7, 9, 1},
        {8, 5, 4}
    };
    
    printf("Исходный граф (вершины пронумерованы с 1 до 9):\n");
    printf("Ребра (источник, назначение, вес):\n");
    
    for (int i = 0; i < edges; i++) {
        int src = edgeData[i][0];
        int dest = edgeData[i][1];
        int weight = edgeData[i][2];
        
        printf("%d %d %d\n", src, dest, weight);
        
        // Добавление ребра в граф для Дейкстры
        addEdge(graph, src, dest, weight);
        
        // Заполнение матрицы для Флойда
        adjMatrix[src][dest] = weight;
    }
    
    printf("\n");
    
    // Поиск кратчайшего пути от 2 до 7
    int start = 2;
    int end = 7;
    
    // Алгоритм Дейкстры
    dijkstra(graph, start, end);
    
    // Алгоритм Флойда-Уоршелла
    floydWarshall(adjMatrix, start, end, vertices);
    
    // Очистка памяти
    freeGraph(graph);
    
    return 0;
}