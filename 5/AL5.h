#define MAX_VERTICES 9
#define INF 1000000  // "Бесконечность типо"

// Структура для хранения ребра (для алгоритма Дейкстры через список смежности)
typedef struct Edge {
    int dest;
    int weight;
    struct Edge* next;
} Edge;

// Структура для хранения графа
typedef struct Graph {
    Edge* edges[MAX_VERTICES + 1];  // индексация с 1
    int numVertices;
} Graph;

// Структура для приоритетной очереди (упрощенная реализация)
typedef struct MinHeapNode {
    int vertex;
    int distance;
} MinHeapNode;

typedef struct MinHeap {
    int size;
    int capacity;
    int* pos;
    MinHeapNode** array;
} MinHeap;

int AL5();