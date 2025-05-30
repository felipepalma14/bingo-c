#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

// CONFIGURAÇÕES DO JOGO
#define TAMANHO_CARTELA 3           // Tamanho da cartela (3x3)
#define NUMERO_MAXIMO 30            // Números vão de 1 a 30
#define MAX_CARTELAS 100            // Número máximo de cartelas permitidas
#define MIN_CARTELAS 2              // Número mínimo de cartelas

typedef struct no {
    int value;
    bool marked;
    int index; //todo
    struct no *left, *right;
} NoArvore;

NoArvore* insert(NoArvore* root, int value, int index) {
    if (root == NULL) {
        NoArvore* node = malloc(sizeof(NoArvore));
        node->value = value;
        node->index = index;
        node->marked = false;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    if (value < root->value)
        root->left = insert(root->left, value, index);
    else if (value > root->value)
        root->right = insert(root->right, value, index);
    return root;
}

/*
 * AQUI VAI PERCORRER A ARVORE BINDO USANDO "EM ORDEM"
 * A MATRIZ SERÁ PREENCHIDA DE ACORDO COM O INDICE
 * O INDICE É UTILIZADO PARA DEFINIR A PROXIMA DO NÓ NA MATRIZ
 * O INDICE SERÁ INCREMENTADO ATÉ PASSAR POR TODOS OS ITENS DA MATRIZ, OU SEJA 9 NUMERO, 3X3
 */
void preencherEmOrdem(NoArvore* root, NoArvore* matrix[TAMANHO_CARTELA][TAMANHO_CARTELA], int* index) {
    if (root == NULL) return;
    preencherEmOrdem(root->left, matrix, index);
    matrix[*index / TAMANHO_CARTELA][*index % TAMANHO_CARTELA] = root;
    (*index)++;
    preencherEmOrdem(root->right, matrix, index);
}

/*
 * EMBARALHA O VETOR
 * GERA NUMEROS ALEATORIOS
 * rand() % (i + 1); -> OBRIGA O RETORNO SER ENTRE 1 E i
 */

void shuffle(int* arr, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

/*
 * ADICIONA NUMEROS NO VETOR POOL ATÉ MAX_NUMBER
 * EMBARALHA O VETOR
 * EVITA NUMERO DUPLICADOS NAS CARTELAS
 */
void generateUniqueNumbers(int* pool, int* dest, int count, int* used) {
    for (int i = 0; i < NUMERO_MAXIMO; i++) pool[i] = i + 1;
    shuffle(pool, NUMERO_MAXIMO);

    int idx = 0;
    for (int i = 0; i < NUMERO_MAXIMO && idx < count; i++) {
        if (!used[pool[i]]) {
            dest[idx++] = pool[i];
            used[pool[i]] = 1;
        }
    }
}

void mostrarCartela(NoArvore* matrix[TAMANHO_CARTELA][TAMANHO_CARTELA], int row, int col) {
    if (row == TAMANHO_CARTELA) return;
    NoArvore* node = matrix[row][col];
    if (node->marked)
        printf("[%2d X] ", node->value);
    else
        printf("[%2d  ] ", node->value);

    if (col == TAMANHO_CARTELA - 1) {
        printf("\n");
        mostrarCartela(matrix, row + 1, 0);
    } else {
        mostrarCartela(matrix, row, col + 1);
    }
}


int main() {

    NoArvore* root1 = NULL;
    NoArvore* root2 = NULL;
    NoArvore* matrix1[TAMANHO_CARTELA][TAMANHO_CARTELA];
    NoArvore* matrix2[TAMANHO_CARTELA][TAMANHO_CARTELA];

    int pool[NUMERO_MAXIMO];

    int usedNumbers[NUMERO_MAXIMO + 1] = {0};
    int nums1[TAMANHO_CARTELA * TAMANHO_CARTELA], nums2[TAMANHO_CARTELA * TAMANHO_CARTELA];

    /*
     * GERAR NUMEROS UNICOS POR CARTELA
     **/
    generateUniqueNumbers(pool,nums1, TAMANHO_CARTELA * TAMANHO_CARTELA, usedNumbers);
    generateUniqueNumbers(pool,nums2, TAMANHO_CARTELA * TAMANHO_CARTELA, usedNumbers);

    /**
     * PREENCHER CARTELA
     */
    for (int i = 0; i < TAMANHO_CARTELA * TAMANHO_CARTELA; i++) {
        root1 = insert(root1, nums1[i], i);
        root2 = insert(root2,  nums2[i], i);
    }

    /*
     * MOSTRAR CARTELA
     */
    int idx1 = 0, idx2 = 0;
    preencherEmOrdem(root1, matrix1, &idx1);
    preencherEmOrdem(root2, matrix2, &idx2);

    printf("\n🎴 CARTELA 1:\n");
    mostrarCartela(matrix1, 0, 0);

    printf("\n🎴 CARTELA 2:\n");
    mostrarCartela(matrix2, 0, 0);
    return 0;
}