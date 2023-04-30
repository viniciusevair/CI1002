#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "libSubAVL.h"

struct tNumArvore *criaNumArvore() {
    struct tNumArvore *tree;
    if (! (tree = malloc(sizeof(struct tNumArvore)))) {
        fprintf (stderr, "Erro ao alocar memória para uma subarvore.");
        exit (1);
    }

    tree->raiz = NULL;
    tree->ponteiro = NULL;

    return tree;
}

/*
 * Funcao interna.
 * Desaloca recursivamente os nos da arvore. Tambem desaloca a subarvore de cada
 * no. Sempre retorna NULL.
 */
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

/* Funcao interna. Busca e retorna o ponteiro para uma chave da arvore. */
struct tNumNo *buscaPos(struct tNumNo *no, int pos) {
    if (no == NULL)
        return NULL;
    if (no->pos == pos)
        return no;
    if (pos < no->pos)
        return buscaPos(no->esq, pos);
    return buscaPos(no->dir, pos);
}

/*
 * Funcao interna para gerenciamento de dados.
 * Reorganiza os nos da arvore fazendo uma rotacao para a esquerda.
 * Diminui em 1 a altura da arvore no processo.
 */
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

/*
 * Funcao interna para gerenciamento de dados.
 * Reorganiza os nos da arvore fazendo uma rotacao para a direita.
 * Diminui em 1 a altura da arvore no processo.
 */
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

/* Cria e aloca memoria para um no. */
struct tNumNo *criaNumNo(int pos) {
    struct tNumNo *no;
    if (! (no = malloc(sizeof(struct tNumNo)))) {
        fprintf (stderr, "Erro ao alocar memória para um no da subarvore");
        exit (1);
    }

    no->pos = pos;
    no->esq = NULL;
    no->dir = NULL;
    no->pai = NULL;
    no->equilibrio = 0;

    return no;
}

/* Calcula a altura da arvore. */
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

/*
 * Funcao interna para gerenciamento de dados.
 * Calcula e realiza as rotacoes necessarias para manter o balanceamento.
 * Tambem atualiza a variavel de equilibrio dos nodos afetados.
 */
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
    printf(" %d", no->pos);
    imprimeInverso(no->esq);
}

void imprimeInversoEmArq(FILE *arq, struct tNumNo *no) {
    if (no == NULL)
        return;

    imprimeInversoEmArq(arq, no->dir);
    fwprintf(arq, L" %d", no->pos);
    imprimeInversoEmArq(arq, no->esq);
}

/*
 * Retorna a quantidade de nos na arvore.
 * Ou seja, a quantidade de dados atualmente guardados na estrutura.
 */
int quantidadeNos(struct tNumNo *no) {
    if (no == NULL)
        return 0;

    return 1 + quantidadeNos(no->esq) + quantidadeNos(no->dir);
}

/*
 * Funcao auxiliar da funcao de pos aleatoria.
 * Caminha em preordem pela estrutura e retorna o no apos qtd movimentos.
 */
struct tNumNo *nodoAleatorio(struct tNumNo *no, int qtd, int *aux) {
    if (no == NULL)
        return NULL;

    (*aux)++;
    if ((qtd - *aux) == 0)
        return no;        
    else {
        struct tNumNo *esq = nodoAleatorio(no->esq, qtd, aux);
        if (esq != NULL)
            return esq;
        return nodoAleatorio(no->dir, qtd, aux);
    }

    return NULL;
}

int posAleatoria(struct tNumArvore *tree) {
    int qtdNos, aux, qtdMovimento;
    struct tNumNo *nodoAux;

    qtdNos = quantidadeNos(tree->raiz);
    nodoAux = NULL;
    while (nodoAux == NULL) {
        qtdMovimento = rand() % qtdNos;
        aux = 0;
        nodoAux = nodoAleatorio(tree->raiz, qtdMovimento, &aux);
    }

    return nodoAux->pos;
}
