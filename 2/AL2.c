#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "AL2.h"

int maxim(int *buffer, int size);
void printMassiv(int *buffer, int size);
int* input_from_keyboard(int *size);
int* generate_random(int *size);
int* read_from_file(int *size);

int maxim(int *buffer, int size) {  
    int max = buffer [0];
    for (int i = 0; i < size; i++) {
        if (buffer[i] > max)
        {
            max = buffer[i];
        }
    }
    return max;
}

void printMassiv(int *buffer, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", buffer[i]);
    }
}

void swap(int *j, int *k){
    int tmp = *j;
    *j = *k;
    *k = tmp;
}

void SortPuzirek(int *buffer, int size){
    for (int j = 0; j < (size - 1); j++) {
        int f = 0;
        for (int i = 0; i < (size - 1); i++) {
            if (buffer[i] > buffer[i+1])
            {
                swap(&buffer[i], &buffer[i+1]);
                f = 1;
            }
        }
        if (f == 0)
        {
            break;
        }
        
    }
}

void SortPodbor(int *buffer, int size) {
    int max = maxim(buffer, size);
    int tbuf[max + 1];

    for (int i = 0; i < max + 1; i++) {
        tbuf[i] = 0;
    }

    for (int i = 0; i < size; i++) {
        tbuf[buffer[i]] += 1;
    }
    
    // Восстанавливаем отсортированный массив
    int index = 0;
    for (int i = 0; i < max + 1; i++) {
        while (tbuf[i] > 0) {
            buffer[index++] = i;
            tbuf[i]--;
        }
    }
}

int* input_from_keyboard(int *size) {
    printf("Введите размер массива: ");
    scanf("%d", size);
    
    // Выделяем память
    int *arr = (int*)malloc(*size * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    
    printf("Введите %d элементов:\n", *size);
    for (int i = 0; i < *size; i++) {
        printf("arr[%d] = ", i);
        scanf("%d", &arr[i]);
    }
    
    return arr;
}

int* generate_random(int *size) {
    printf("Введите размер массива: ");
    scanf("%d", size);
    
    int *arr = (int*)malloc(*size * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    
    int max_value;
    printf("Введите максимальное значение (например, 100): ");
    scanf("%d", &max_value);
    
    // Инициализация генератора
    srand(time(NULL));
    
    // Заполняем случайными числами
    for (int i = 0; i < *size; i++) {
        arr[i] = rand() % (max_value + 1);  // от 0 до max_value
    }
    
    printf("Сгенерирован массив:\n ");
    printMassiv(arr, *size);
    printf("\n");
    return arr;
}

int* read_from_file(int *size) {
    char filename[100];
    printf("Введите имя файла: ");
    scanf("%s", filename);
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла %s!\n", filename);
        return NULL;
    }
    
    // Сначала подсчитываем количество чисел в файле
    int count = 0;
    int temp;
    while (fscanf(file, "%d", &temp) == 1) {
        count++;
    }
    
    if (count == 0) {
        printf("Файл пуст или содержит нечисловые данные!\n");
        fclose(file);
        return NULL;
    }
    
    // Возвращаемся в начало файла
    rewind(file);
    
    // Выделяем память
    int *arr = (int*)malloc(count * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        fclose(file);
        return NULL;
    }
    
    // Читаем данные
    for (int i = 0; i < count; i++) {
        fscanf(file, "%d", &arr[i]);
    }
    
    *size = count;
    fclose(file);
    
    printf("Прочитано %d элементов из файла\n", count);
    return arr;
}


int AL2(void) {
    char ch;
    int *massort = NULL;
    int size = 0;

    //метод сортировки
    printf("    ВЫБЕРЕТЕ СПОСОБ ЗАДАНИЯ МАССИВА:\n(1)Ввод с клавиатуры\n(2)Рандом\n(3)Чтение из файла\n");
    scanf(" %c", &ch);
    if (ch == '1') {
        printf("Ввод с клавиатуры\n");
        massort = input_from_keyboard(&size);
    }
    else if (ch == '2') {
        printf("Рандом\n");
        massort = generate_random(&size);
    }
    else if (ch == '3') {
        printf("Чтение из файла\n");
        massort = read_from_file(&size);
    }
    else {
        printf("Неверный выбор!\n");
        return 1;
    }

    printf("Исходный массив:\n");
    printMassiv(massort, size);
    printf("\n");

    getchar();
    
    printf("    ВЫБЕРЕТЕ МЕТОД СОРТИРОВКИ:\n(1)Пузырек\n(2)Посчетом\n");
    scanf(" %c", &ch);
    if (ch == '1') {
        printf("Сортировка пузырьком\n");
        SortPuzirek(massort, size);
        printMassiv(massort, size);
    }
    else if (ch == '2') {
        printf("Сортировка подсчетом\n");
        SortPodbor(massort, size);
        printMassiv(massort, size);
    }
    else {
        printf("Неверный выбор!\n");
        return 1;
    }

    //способ задания массива (длина, ввод с клавы, рандом, чтение из файла)


    printf("\n");
    free(massort);
    return 0;
}