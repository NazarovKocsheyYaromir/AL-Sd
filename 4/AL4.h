// Структура узла дерева
struct Node {
    int key;            // данные
    struct Node* left;  // левый сын
    struct Node* right; // правый сын
    int height;         // высота узла (для АВЛ-дерева)
};

// Функции для "красивого" отображения двоичных деревьев
#define true    1
#define false   0

struct Trunk {
    struct Trunk* prev;
    char* str;
};
int AL4();