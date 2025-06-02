#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

// CONFIGURAÇÕES DO JOGO
#define TAMANHO_CARTELA 3           // Tamanho da cartela (3x3)
#define TOTAL_CASAS 9           // Tamanho da cartela (3x3)
#define NUMERO_MAXIMO 30            // Números vão de 1 a 30
#define MAX_CARTELAS 100            // Número máximo de cartelas permitidas
#define MIN_CARTELAS 2              // Número mínimo de cartelas

typedef struct No {
    int value;
    bool isMarked;
    int index;
    int height;
    struct No *left, *right;
} NoArvore;

NoArvore* createNode(int value, int index) {
    NoArvore* node = (NoArvore*)malloc(sizeof(NoArvore));
    node->value = value;
    node->index = index;
    node->isMarked = false;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}


// funcao para retornar a maior subarvore de um determinado no
int largest_subtree(int a, int b){
    return (a > b) ? a : b; //ternario que significa se a maior que b retorne a
}                          //se nao for, retorna b


/*
 * Retorna a altura de um nó. Se o nó for NULL, retorna 0.
 */
int height(NoArvore* n) {
    return (n == NULL) ? 0 : n->height;
}

NoArvore* rightRotate(NoArvore* y) {
    NoArvore* x = y->left;
    NoArvore* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = largest_subtree(height(y->left), height(y->right)) + 1;
    x->height = largest_subtree(height(x->left), height(x->right)) + 1;

    return x;
}

NoArvore* leftRotate(NoArvore* x) {
    NoArvore* y = x->right;
    NoArvore* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = largest_subtree(height(x->left), height(x->right)) + 1;
    y->height = largest_subtree(height(y->left), height(y->right)) + 1;

    return y;
}

/*
 * A AVL usa esse balanceador para decidir se precisa de rotação.
 */
//funcao que calcula o fator de balanceamento a partir de determinado no
int balance_factor(NoArvore *node){

    if (node){
        return height(node->left) - height(node->right);
    }
    else
        return 0;

}

//funcoes de rotacoes duplas
//desbalanceada para a direita
NoArvore* right_left_rotation(NoArvore *r){

    r->right = rightRotate(r->right);

    return leftRotate(r);

}

//desbalanceada para a esquerda
NoArvore* left_right_rotation(NoArvore *r){

    r->left = leftRotate(r->left);

    return rightRotate(r);

}

/*
    Função para realizar o balanceamento da árvore após uma inserção ou remoção
    Recebe o nó que está desbalanceado e retorna a nova raiz após o balanceamento
*/
NoArvore* balance(NoArvore* root){
    short bf = balance_factor(root);
    // Rotação à esquerda
    if(bf < -1 && balance_factor(root->right) <= 0)
        root = leftRotate(root);
    // Rotação à direita
    else if(bf > 1 && balance_factor(root->left) >= 0)
        root = rightRotate(root);
    // Rotação dupla à esquerda
    else if(bf > 1 && balance_factor(root->left) < 0)
        root = left_right_rotation(root);
    // Rotação dupla à direita
    else if(bf < -1 && balance_factor(root->right) > 0)
        root = right_left_rotation(root);
    return root;
}

NoArvore* insert(NoArvore* root, int value, int index) {
    if (root == NULL) {
        return createNode(value, index);
    }
    if (value < root->value)
        root->left = insert(root->left, value, index);
    else if (value > root->value)
        root->right = insert(root->right, value, index);

    //recalcula a altura de todos os nós entre a raiz e o novo nó inserido
    root->height = largest_subtree(height(root->left), height(root->right)) + 1;


    //verifica a necessidade de rebalancear a árvore
    root = balance(root);
    return root;
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
void gerarNumeros(int* numeros) {
    int pool[NUMERO_MAXIMO];
    for (int i = 0; i < NUMERO_MAXIMO; i++) pool[i] = i + 1;
    shuffle(pool, NUMERO_MAXIMO);
    for (int i = 0; i < TOTAL_CASAS; i++) numeros[i] = pool[i];
}

/*
 * AQUI VAI PERCORRER A ARVORE BINDO USANDO "EM ORDEM"
 * A MATRIZ SERÁ PREENCHIDA DE ACORDO COM O INDICE
 * O INDICE É UTILIZADO PARA DEFINIR A PROXIMA DO NÓ NA MATRIZ
 * O INDICE SERÁ INCREMENTADO ATÉ PASSAR POR TODOS OS ITENS DA MATRIZ, OU SEJA 9 NUMERO, 3X3
 */
void preencherEmOrdem(NoArvore* root, NoArvore* matriz[TOTAL_CASAS], int* index) {
    if (root == NULL) return;
    preencherEmOrdem(root->left, matriz, index);
    matriz[*index] = root;
    (*index)++;
    preencherEmOrdem(root->right, matriz, index);
}

void mostrarCartela(NoArvore* matriz[TOTAL_CASAS], int linha, int coluna) {
    if (linha == TAMANHO_CARTELA) return;
    int index = linha * TAMANHO_CARTELA + coluna;
    if (matriz[index]->isMarked)
        printf("[%2d X] ", matriz[index]->value);
    else
        printf("[%2d  ] ", matriz[index]->value);

    if (coluna == TAMANHO_CARTELA - 1) {
        printf("\n");
        mostrarCartela(matriz, linha + 1, 0);
    } else {
        mostrarCartela(matriz, linha, coluna + 1);
    }
}

void marcarNumero(NoArvore* root, int numero) {
    if (root == NULL) return;
    if (root->value == numero) {
        root->isMarked = true;
        return;
    }
    if (numero < root->value)
        marcarNumero(root->left, numero);
    else
        marcarNumero(root->right, numero);
}

bool verificarHorizontal(bool m[], int linha) {
    if (linha >= TAMANHO_CARTELA) return false;
    int i = linha * TAMANHO_CARTELA;
    if (m[i] && m[i + 2] && m[i + 1]) return true;
    return verificarHorizontal(m, linha + 1);
}

bool verificarVertical(bool m[], int col) {
    if (col >= TAMANHO_CARTELA) return false;
    if (m[col] && m[col + 3] && m[col + 6]) return true;
    return verificarVertical(m, col + 1);
}

bool verificarDiagonal(bool m[]) {
    return (m[0] && m[4] && m[8]) || (m[2] && m[4] && m[6]);
}

bool verificarBingoRecursiva(NoArvore* matriz[TOTAL_CASAS]) {
    bool marcados[TOTAL_CASAS];
    for (int i = 0; i < TOTAL_CASAS; i++) marcados[i] = matriz[i]->isMarked;

    return verificarHorizontal(marcados, 0) ||
           verificarVertical(marcados, 0) ||
           verificarDiagonal(marcados);
}


int main() {

    int numeros[TOTAL_CASAS];
    gerarNumeros(numeros);

    /*
     * GERAR NUMEROS UNICOS POR CARTELA
     **/
    /**
     * PREENCHER CARTELA
     */

    NoArvore* root = NULL;
    for (int i = 0; i < TOTAL_CASAS; i++)
        root = insert(root, numeros[i], i);


    NoArvore* matriz[TOTAL_CASAS];
    int index = 0;
    preencherEmOrdem(root, matriz, &index);

    int numero;
    while (1) {
        printf("\nDigite um número sorteado (ou 0 para sair): ");
        scanf("%d", &numero);

        if (numero == 0) break;

        marcarNumero(root, numero);
        printf("\n🎴 CARTELA:\n");
        mostrarCartela(matriz, 0, 0);

        if (verificarBingoRecursiva(matriz)) {
            printf("\n🎉 BINGO!\n");
            break;
        }
    }
    return 0;
}