#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AL3.h"

ListNote *addListNode(ListNote *ln, int data) {
    ListNote *t = (ListNote*)malloc(sizeof(ListNote));
    t->data = data;
    t->next = ln;
    return t; 
}

ListNote *delListNode(ListNote *ln, int data){
    ListNote *prev = NULL;
    ListNote *next = NULL;
    ListNote *t = ln;
    while (t && (t->data != data)) 
    {
        prev = t;
        t = t->next;
    }
    
    if (!t) return ln;
    
    if (!prev) {
        next = t->next;
        free(t);
        return next;
    } else {
        prev -> next = t->next;
        free(t);
        return ln;
    }
}

ListNote *removeDuplicates(ListNote *ln) {
    if (!ln || !ln->next) return ln;
    
    ListNote *current = ln;
    
    while (current) {
        ListNote *runner = current;
        int count = 0;
        
        // Считаем вхождения текущего элемента
        while (runner) {
            if (runner->data == current->data) {
                count++;
            }
            runner = runner->next;
        }
        
        // Если элемент повторяется - удаляем все его вхождения
        if (count > 1) {
            int value = current->data;
            ListNote *temp = ln;
            
            // Удаляем все элементы с этим значением
            while (temp) {
                if (temp->data == value) {
                    ln = delListNode(ln, value);
                    temp = ln; // Начинаем обход заново
                } else {
                    temp = temp->next;
                }
            }
            current = ln; // Начинаем обход заново
        } else {
            current = current->next;
        }
    }
    
    return ln;
}

void freeList(ListNote* ln) {
    ListNote *t;
    while (ln)
    {
        t = ln->next;
        free(ln);
        ln = t;
    }
}

void printList(ListNote *ln) {
    ListNote *t = ln;
    while (t)
    {
        printf("%d ", t->data);
        t = t->next;
    }
    printf("\n");
}

int AL3(int argc, char const *argv[])
{
   ListNote *ln = NULL;
    int n;          // Количество элементов
    int number;     // Вводимое число
    
    printf("=== СОЗДАНИЕ СПИСКА ===\n");
    
    // Ввод количества элементов
    printf("Введите количество элементов списка: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Количество элементов должно быть больше 0!\n");
        return 1;
    }
    
    // Ввод каждого элемента
    printf("Введите %d элементов списка:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Элемент %d: ", i + 1);
        scanf("%d", &number);
        ln = addListNode(ln, number);
    }
    
    // Показываем исходный список
    printf("\nИсходный список:\n");
    printList(ln);
    
    // Удаляем дубликаты
    printf("\nУдаляем дубликаты...\n");
    ln = removeDuplicates(ln);
    
    // Показываем результат
    printf("Список после удаления дубликатов:\n");
    printList(ln);
    
    // Освобождаем память
    freeList(ln);
    
    printf("\nПрограмма завершена.\n\n\n\n");
    return 0;
}