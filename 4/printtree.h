// Вспомогательная функция для печати ветвей дерева
void showTrunks(struct Trunk* p) {
    if (p == NULL) {
        return;
    }
    showTrunks(p->prev);
    printf("%s", p->str);
}

void printTree(struct Node* root, struct Trunk* prev, int isLeft) {
    if (root == NULL) {
        return;
    }

    char* prev_str = "     ";
    struct Trunk* trunk = (struct Trunk*)malloc(sizeof(struct Trunk));
    trunk->prev = prev;
    trunk->str = prev_str;

    printTree(root->right, trunk, true);

    if (!prev) {
        trunk->str = "-----";
    }
    else if (isLeft) {
        trunk->str = ".----";
        prev_str = "    |";
    }
    else {
        trunk->str = "`----";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    printf(" %d\n", root->key);

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "    |";

    printTree(root->left, trunk, false);
    free(trunk);
}