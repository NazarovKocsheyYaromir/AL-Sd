#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "AL1.h"


// символ частью идентификатора?
int is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int AL1(int argc, char *argv[]) {
    if (argc != 2) { //проверка кол-ва аргумент.
        printf("Использование: %s <filename.c>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r"); //открываем файлик
    if (file == NULL) { //если пустой
        printf("Ошибка: Не удалось открыть файл '%s'\n", argv[1]);
        return 1;
    }
    
    printf("Содержимое файла '%s' с подсветкой float:\n\n", argv[1]);
    
    char line[256];
    int line_number = 1;
    ParserState state = STATE_NORMAL;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        printf("%3d: ", line_number++);
        
        char *ptr = line;
        while (*ptr != '\0') {
            switch (state) {
                case STATE_NORMAL:
                    if (*ptr == '/' && *(ptr + 1) == '/') {
                        printf("//");
                        ptr += 2;
                        state = STATE_SINGLE_LINE_COMMENT;
                        continue;
                    } else if (*ptr == '/' && *(ptr + 1) == '*') {
                        printf("/*");
                        ptr += 2;
                        state = STATE_MULTI_LINE_COMMENT;
                        continue;
                    } else if (*ptr == '"') {
                        putchar('"');
                        ptr++;
                        state = STATE_STRING_LITERAL;
                        continue;
                    }
                    
                    // проверка границ слова
                    if (strncmp(ptr, "float", 5) == 0) {
                        // что это отдельное слово
                        int is_word_boundary = 1;
                        
                        //проверяем символ перед "float"
                        if (ptr != line) {  // Если не начало строки
                            if (is_identifier_char(*(ptr - 1))) {
                                is_word_boundary = 0;  // Не граница слова
                            }
                        }
                        
                        // проверяем символ после "float"
                        if (is_identifier_char(*(ptr + 5))) {
                            is_word_boundary = 0;  // Не граница слова
                        }
                        
                        if (is_word_boundary) {
                            printf(RED "float" RESET);
                            ptr += 5;
                        } else {
                            putchar(*ptr);
                            ptr++;
                        }
                    } else {
                        putchar(*ptr);
                        ptr++;
                    }
                    break;
                    
                case STATE_SINGLE_LINE_COMMENT:
                    putchar(*ptr);
                    ptr++;
                    break;
                    
                case STATE_MULTI_LINE_COMMENT:
                    if (*ptr == '*' && *(ptr + 1) == '/') {
                        printf("*/");
                        ptr += 2;
                        state = STATE_NORMAL;
                    } else {
                        putchar(*ptr);
                        ptr++;
                    }
                    break;
                    
                case STATE_STRING_LITERAL:
                    if (*ptr == '"' && *(ptr - 1) != '\\') {
                        putchar('"');
                        ptr++;
                        state = STATE_NORMAL;
                    } else {
                        putchar(*ptr);
                        ptr++;
                    }
                    break;
            }
        }
        
        if (state == STATE_SINGLE_LINE_COMMENT) {
            state = STATE_NORMAL;
        }
        
        printf("\n");
    }
    
    fclose(file);
    return 0;
}