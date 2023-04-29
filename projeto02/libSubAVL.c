#include <stdlib.h>
#include <stdio.h>
#include "libSubAVL.h"

struct tNumArvore *criaNumArvore() {
    struct tNumArvore *tree;
    if (! (tree = malloc(sizeof(struct tNumArvore)))) {
        fprintf (stderr, "Erro ao alocar memória");
        exit (1);
    }

    tree->raiz = NULL;
    tree->ponteiro = NULL;

    return tree;
}

struct tNumNo *destroiNumGalhos(struct tNumNo *no) {
    if (no == NULL)
        return NULL;

    if(no->esq != NULL)
        destroiNumGalhos(no->esq);
    if(no->dir != NULL)
        destroiNumGalhos(no->dir);

    free(no);

    return NULL;
}

struct tNumNo *destroiNumArvore(struct tNumArvore *tree) {
    tree->raiz = destroiNumGalhos(tree->raiz);
    free(tree);

    return NULL;
}

struct tNumNo *buscaPos(struct tNumNo *no, int pos) {
    if (no == NULL)
        return NULL;
    if (no->pos == pos)
        return no;
    if (pos < no->pos)
        return buscaPos(no->esq, pos);
    return buscaPos(no->dir, pos);
}

struct tNumNo *rotacionaEsquerda (struct tNumArvore *tree, struct tNumNo *no) {
    struct tNumNo *aux;
    aux = no->dir;
    no->dir = aux->esq;
    aux->pai = no->pai;
    if (aux->esq != NULL)
        aux->esq->pai = no;
    if (no->pai == NULL)
        tree->raiz = aux;
    else 
        if (no == no->pai->esq)
            no->pai->esq = aux;
        else
            no->pai->dir = aux;

    aux->esq = no;
    no->pai = aux;

    return aux;
}

struct tNumNo *rotacionaDireita(struct tNumArvore *tree, struct tNumNo *no) {
    struct tNumNo *aux;
    aux = no->esq;
    no->esq = aux->dir;
    aux->pai = no->pai;
    if (aux->dir != NULL)
        aux->dir->pai = no;
    if (no->pai == NULL)
        tree->raiz = aux;
    else 
        if (no == no->pai->dir)
            no->pai->dir = aux;
        else
            no->pai->esq = aux;

    aux->dir = no;
    no->pai = aux;

    return aux;
}

struct tNumNo *criaNumNo(int pos) {
    struct tNumNo *no;
    if (! (no = malloc(sizeof(struct tNumNo)))) {
        fprintf (stderr, "Erro ao alocar memória");
        exit (1);
    }

    no->pos = pos;
    no->esq = NULL;
    no->dir = NULL;
    no->pai = NULL;
    no->equilibrio = 0;

    return no;
}

int alturaNumNo(struct tNumNo *no) {
    int alturaEsq, alturaDir;
    if (no == NULL)
        return -1;

    alturaEsq = alturaNumNo(no->esq);
    alturaDir = alturaNumNo(no->dir);

    if (alturaEsq < alturaDir)
        return alturaDir + 1;
    return alturaEsq + 1;
}

struct tNumNo *ajustaNumArvore(struct tNumArvore *tree, struct tNumNo *no, int *controle) {
    struct tNumNo *aux;
    if (no->equilibrio == -2) {
        if (no->esq != NULL && no->esq->equilibrio > 0)
            no->esq = rotacionaEsquerda(tree, no->esq);
        aux = rotacionaDireita(tree, no);
    }
    else {
        if (no->dir != NULL && no->dir->equilibrio < 0)
            no->dir = rotacionaDireita(tree, no->dir);
        aux = rotacionaEsquerda(tree, no);
    }

    aux->equilibrio = 0;
    aux->esq->equilibrio = alturaNumNo(aux->esq->dir) - alturaNumNo(aux->esq->esq);
    aux->dir->equilibrio = alturaNumNo(aux->dir->dir) - alturaNumNo(aux->dir->esq);
    (*controle) = 0;

    return aux;
}

struct tNumNo *adicionaPos(struct tNumArvore *tree, struct tNumNo *no, int pos, int *controle) {
    if (no == NULL) {
        no = criaNumNo(pos);
        (*controle) = 1;
        return no;
    }

    if (pos < no->pos) {
        no->esq = adicionaPos(tree, no->esq, pos, controle);
        no->esq->pai = no;
        if (*controle)
            (no->equilibrio)--;
    }
    else {
        no->dir = adicionaPos(tree, no->dir, pos, controle);
        no->dir->pai = no;
        if (*controle)
            (no->equilibrio)++;
    }

    if (*controle) {
        if (no->equilibrio == 0)
            (*controle) = 0;
        if (no->equilibrio < -1 || no->equilibrio > 1)
            no = ajustaNumArvore(tree, no, controle);
    }

    return no;
}

void imprimeInverso(struct tNumNo *no) {
    if (no == NULL)
        return;

    imprimeInverso(no->dir);
    printf("%d ", no->pos);
    imprimeInverso(no->esq);
}

struct tNumNo *max(struct tNumNo *no) {
    if (no->dir == NULL)
        return no;
    return max(no->dir);
}

int visitaSubNodo(struct tNumNo *no) {
    return no->pos;
}

void incrementaSubIterador(struct tNumArvore *tree) {
    struct tNumNo *prox;
    if (tree->ponteiro->esq != NULL)
        tree->ponteiro = max(tree->ponteiro->esq);
    else {
        prox = tree->ponteiro->pai;
        while (prox != NULL && tree->ponteiro == prox->esq) {
            tree->ponteiro = prox;
            prox = prox->pai;
        }
        tree->ponteiro = prox;
    }
}

void inicializaSubIterador(struct tNumArvore *tree) {
    tree->ponteiro = max(tree->raiz);
}
