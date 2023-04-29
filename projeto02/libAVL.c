#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "libAVL.h"
#include "libSubAVL.h"

struct tArvore *criaArvore() {
    struct tArvore *tree;
    if (! (tree = malloc(sizeof(struct tArvore)))) {
        fprintf (stderr, "Erro ao alocar memória");
        exit (1);
    }

    tree->raiz = NULL;
    tree->ponteiro = NULL;

    return tree;
}

struct tNo *destroiGalhos(struct tNo *no) {
    if(no->esq != NULL)
        destroiGalhos(no->esq);
    if(no->dir != NULL)
        destroiGalhos(no->dir);

    destroiNumArvore(no->subTree);
    free(no);

    return NULL;
}

struct tNo *destroiArvore(struct tArvore *tree) {
    destroiGalhos(tree->raiz);
    free(tree);

    return NULL;
}

struct tNo *busca(struct tNo *no, int chave) {
    if (no == NULL)
        return NULL;
    if (no->chave == chave)
        return no;
    if (chave < no->chave)
        return busca(no->esq, chave);
    return busca(no->dir, chave);
}

struct tNo *rotEsquerda (struct tArvore *tree, struct tNo *no) {
    struct tNo *aux;
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

struct tNo *rotDireita (struct tArvore *tree, struct tNo *no) {
    struct tNo *aux;
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

struct tNo *criaNo(wchar_t chave) {
    struct tNo *no;
    if (! (no = malloc(sizeof(struct tNo)))) {
        fprintf (stderr, "Erro ao alocar memória");
        exit (1);
    }

    no->chave = chave;
    no->esq = NULL;
    no->dir = NULL;
    no->pai = NULL;
    no->subTree = NULL;
    no->equilibrio = 0;

    return no;
}

int altura(struct tNo *no) {
    int alturaEsq, alturaDir;
    if (no == NULL)
        return -1;

    alturaEsq = altura(no->esq);
    alturaDir = altura(no->dir);

    if (alturaEsq < alturaDir)
        return alturaDir + 1;
    return alturaEsq + 1;
}

struct tNo *ajustaArvore(struct tArvore *tree, struct tNo *no, int *controle) {
    struct tNo *aux;
    if (no->equilibrio == -2) {
        if (no->esq != NULL && no->esq->equilibrio > 0)
            no->esq = rotEsquerda(tree, no->esq);
        aux = rotDireita(tree, no);
    }
    else {
        if (no->dir != NULL && no->dir->equilibrio < 0)
            no->dir = rotDireita(tree, no->dir);
        aux = rotEsquerda(tree, no);
    }

    aux->equilibrio = 0;
    aux->esq->equilibrio = altura(aux->esq->dir) - altura(aux->esq->esq);
    aux->dir->equilibrio = altura(aux->dir->dir) - altura(aux->dir->esq);
    (*controle) = 0;

    return aux;
}

struct tNo *adicionaChave(struct tArvore *tree, struct tNo *no, wchar_t chave, int *controle, int pos) {
    if (no == NULL) {
        int aux;
        no = criaNo(chave);
        (*controle) = 1;
        aux = 0;
        no->subTree = criaNumArvore();
        no->subTree->raiz = adicionaPos(no->subTree, no->subTree->raiz, pos, &aux);
        return no;
    }

    if (chave == no->chave) {
        int aux;
        aux = 0;
        adicionaPos(no->subTree, no->subTree->raiz, pos, &aux);
    }
    else {
        if (chave < no->chave) {
            no->esq = adicionaChave(tree, no->esq, chave, controle, pos);
            no->esq->pai = no;
            if (*controle)
                (no->equilibrio)--;
        }
        else {
            no->dir = adicionaChave(tree, no->dir, chave, controle, pos);
            no->dir->pai = no;
            if (*controle)
                (no->equilibrio)++;
        }
    }
    if (*controle) {
        if (no->equilibrio == 0)
            (*controle) = 0;
        if (no->equilibrio < -1 || no->equilibrio > 1)
            no = ajustaArvore(tree, no, controle);
    }

    return no;
}

void guardaDado(struct tArvore *tree, wchar_t chave, int pos) {
    int controle;

    controle = 0;
    tree->raiz = adicionaChave(tree, tree->raiz, chave, &controle, pos);
}

void imprimeEmOrdem(struct tNo *no) {
    if (no == NULL)
        return;

    imprimeEmOrdem(no->esq);
    printf("%lc: ", no->chave);
    imprimeInverso(no->subTree->raiz);
    printf("\n");
    imprimeEmOrdem(no->dir);
}

void imprimeDados(struct tArvore *tree) {
    imprimeEmOrdem(tree->raiz);
}

struct tNo *min(struct tNo *no) {
    if (no->esq == NULL)
        return no;
    return min(no->esq);
}

wchar_t chaveNodo(struct tNo *no) {
    return no->chave;
}

void incrementaIterador(struct tArvore *tree) {
    struct tNo *prox;
    if (tree->ponteiro->dir != NULL)
        tree->ponteiro = min(tree->ponteiro->dir);
    else {
        prox = tree->ponteiro->pai;
        while (prox != NULL && tree->ponteiro == prox->dir) {
            tree->ponteiro = prox;
            prox = prox->pai;
        }
        tree->ponteiro = prox;
    }
}

void inicializaIterador(struct tArvore *tree) {
    tree->ponteiro = min(tree->raiz);
}
