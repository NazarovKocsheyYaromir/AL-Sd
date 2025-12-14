#define RED     "\033[91m"
#define RESET   "\033[0m"

// Парсеры, состояния
typedef enum {
    STATE_NORMAL, //  норм строка
    STATE_SINGLE_LINE_COMMENT, //одиночный коммент
    STATE_MULTI_LINE_COMMENT, //многострочный комент
    STATE_STRING_LITERAL //в строке
} ParserState;
int AL1();