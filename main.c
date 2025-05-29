#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Estrutura base da matriz
 */
typedef struct {
    int value;
    bool marked;
    struct TreeNode *left, *right;
} TreeNode;

/*
 * Inserir na arvore
 */
TreeNode* insert(TreeNode *root, int num) {
    // adiciona se for null a raiz
    if(root == NULL) {
        TreeNode *node = (TreeNode*) malloc(sizeof(TreeNode));
        node->value = num;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    if(num < root->value){
        root->left = insert(root->left, num);
    }
    else {
        root->right = insert(root->right, num);
    }
    return root;
}

int main(void) {

    return 0;
}