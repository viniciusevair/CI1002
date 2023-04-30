#include <stdio.h>
struct tNumNo {
    int pos;
    int equilibrio;
    struct tNumNo *esq, *dir, *pai;
};

struct tNumArvore {
    struct tNumNo *raiz;
    struct tNumNo *ponteiro;
};

/* Cria uma árvore vazia. */
struct tNumArvore *criaNumArvore();

/* Destroi uma árvore e retorna NULL. */
struct tNumNo *destroiNumArvore(struct tNumArvore *tree);

struct tNumNo *adicionaPos(struct tNumArvore *tree, struct tNumNo *no, int pos, int *controle);

struct tNumNo *buscaPos(struct tNumNo *no, int pos);

int posAleatoria(struct tNumArvore *tree);

/* Imprime os valores guardados na estrutura */
void imprimeInverso(struct tNumNo *no);

void imprimeInversoEmArq(FILE *arq, struct tNumNo *no);

int visitaSubNodo(struct tNumNo *no);

int incrementaSubIterador(struct tNumArvore *tree);

void inicializaSubIterador(struct tNumArvore *tree);
