#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AL4.h"
#include "printtree.h"


// Функции для АВЛ-дерева

// Максимум из двух чисел
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Получить высоту узла
int height(struct Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Создать новый узел
struct Node* newNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // новый узел имеет высоту 1
    return node;
}

// Правый поворот
struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    // Выполняем поворот
    x->right = y;
    y->left = T2;

    // Обновляем высоты
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Левый поворот
struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    // Выполняем поворот
    y->left = x;
    x->right = T2;

    // Обновляем высоты
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Получить баланс-фактор узла
int getBalance(struct Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Вставка узла в АВЛ-дерево
struct Node* insert(struct Node* node, int key) {
    // 1. Обычная вставка в двоичное дерево поиска
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // Дубликаты ключей не допускаются
        return node;

    // 2. Обновляем высоту текущего узла
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Получаем баланс-фактор этого узла
    int balance = getBalance(node);

    // Если узел несбалансирован, то есть 4 случая

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // возвращаем неизмененный указатель узла
    return node;
}

// Поиск узла с минимальным ключом
struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Удаление узла из АВЛ-дерева
struct Node* deleteNode(struct Node* root, int key) {
    // 1. Обычное удаление из двоичного дерева поиска
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // узел с одним или без детей
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node* temp = root->left ? root->left : root->right;

            // нет детей
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else // один ребенок
                *root = *temp; // копируем содержимое

            free(temp);
        } else {
            // узел с двумя детьми: получаем преемника (минимальный в правом поддереве)
            struct Node* temp = minValueNode(root->right);

            // копируем данные преемника в этот узел
            root->key = temp->key;

            // удаляем преемника
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // если дерево имело только один узел
    if (root == NULL)
        return root;

    // 2. Обновляем высоту текущего узла
    root->height = 1 + max(height(root->left), height(root->right));

    // 3. Получаем баланс-фактор этого узла
    int balance = getBalance(root);

    // Если узел несбалансирован, то есть 4 случая

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Функция для удаления всего дерева
void deleteTree(struct Node* root) {
    if (root == NULL) return;
    
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

// Функция для определения глубины дерева
int treeDepth(struct Node* root) {
    if (root == NULL)
        return 0;
    else {
        int leftDepth = treeDepth(root->left);
        int rightDepth = treeDepth(root->right);
        
        return (leftDepth > rightDepth) ? (leftDepth + 1) : (rightDepth + 1);
    }
}

// Функция для подсчета листьев на каждом уровне
void countLeavesPerLevel(struct Node* root, int level, int* count) {
    if (root == NULL)
        return;
    
    // Если узел является листом (нет детей)
    if (root->left == NULL && root->right == NULL) {
        count[level]++;
        return;
    }
    
    // Рекурсивно обходим левое и правое поддеревья
    countLeavesPerLevel(root->left, level + 1, count);
    countLeavesPerLevel(root->right, level + 1, count);
}

// Основная функция для определения количества листьев на каждом уровне
void printLeavesPerLevel(struct Node* root) {
    if (root == NULL) {
        printf("Дерево пустое!\n");
        return;
    }
    
    int depth = treeDepth(root);
    int* leafCount = (int*)calloc(depth, sizeof(int));
    
    countLeavesPerLevel(root, 0, leafCount);
    
    printf("\nКоличество листьев на каждом уровне:\n");
    for (int i = 0; i < depth; i++) {
        printf("Уровень %d: %d листьев\n", i, leafCount[i]);
    }
    
    free(leafCount);
}

// Функция для чтения данных из файла и построения дерева
struct Node* buildTreeFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла %s\n", filename);
        return NULL;
    }
    
    struct Node* root = NULL;
    int key;
    
    printf("Чтение данных из файла...\n");
    while (fscanf(file, "%d", &key) == 1) {
        printf("Вставка ключа: %d\n", key);
        root = insert(root, key);
    }
    
    fclose(file);
    return root;
}

// Функция для создания тестового файла
void createTestFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка создания файла %s\n", filename);
        return;
    }
    
    // Тестовые данные
    int testData[] = {63, 4, 892, 134, 217, 45, 719, 33, 27, 91, 654, 8, 402, 56, 288, 74};
    int size = sizeof(testData) / sizeof(testData[0]);
    
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", testData[i]);
    }
    
    fclose(file);
    printf("Создан тестовый файл %s\n", filename);
}

// Основная функция
int AL4() {
    const char* filename = "tree_data_AL4.txt";
    
    // Создаем тестовый файл, если его нет
    createTestFile(filename);
    
    // Строим дерево из файла
    struct Node* root = buildTreeFromFile(filename);
    
    if (root == NULL) {
        printf("Не удалось построить дерево\n");
        return 1;
    }
    
    // Отображаем дерево
    printf("\nДвоичное дерево поиска (АВЛ-дерево):\n");
    printTree(root, NULL, false);
    
    // Определяем количество листьев на каждом уровне
    printLeavesPerLevel(root);
    
    // Очищаем память
    deleteTree(root);
    printf("\nЛисточки посчитаны.\n\n\n\n\n\n");
    return 0;
}