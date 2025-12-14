#include <stdio.h>
#include <stdlib.h>

#include "1/AL1.h"
#include "2/AL2.h"
#include "3/AL3.h"
#include "4/AL4.h"
#include "5/AL5.h"


void clear();
void print_quest(char* filename);

int main() {
    clear();
    printf("Choose number of AL work:\n");
    printf("  1 - Основы работы с консольным приложением и синтаксический разбор текста\n");
    printf("  2 - Сортировка одномерных массивов\n");
    printf("  3 - Линейные списки\n");
    printf("  4 - Двоичные деревья поиска\n");
    printf("  5 - Поиск кратчайших путей в ориентированном взвешенном графе\n");
    printf("\n");
    
    int AL_num;
    if (scanf("%d", &AL_num) != 1) {
        printf("n/a\n");
        return 1;
    }

    switch (AL_num) {
    case 1:
char filename[256];
char command[512];

clear();
printf("Enter file name: ");
scanf("%255s", filename);

// Формируем команду для запуска ВНЕШНЕЙ программы
snprintf(command, sizeof(command), "./AL/1/AL1 %s", filename);

// Запускаем внешнюю программу
system(command);
        break;
    case 2:
        clear();
        

        AL2();
        break;
    case 3:
        clear();
        

        AL3();
        break;
    case 4:
        clear();
        

        AL4("src/AL4/input.txt");
        break;
    case 5:
        clear();
        

        AL5("src/AL5/graph.txt");
        break;
    default:
        printf("Incorrect number!\n");
        return 1;
    }
    

    printf("\n\nПосмотрим еще или спать AL? (1 - Yes)\n");
    int answer;
    if (scanf("%d", &answer) != 1) {
        return 0;
    }
    if (answer == 1) main();
}

void clear() {
#ifdef _WIN32
    system("cls");
#else 
    system("clear");
#endif
}

/* void print_quest(char* filename) {
    FILE *file = fopen(filename, "rt");
    char buffer[1000];
    printf("\033[93m");
    printf("--==QUEST==--\n");
    while (fgets(buffer, 1000, file)) {
        printf("%s", buffer);
    }
    printf("\n--=========--\n\n");
    // printf(reset);
} */