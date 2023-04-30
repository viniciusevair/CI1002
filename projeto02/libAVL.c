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

    return tree;
}

/*
 * Funcao interna.
 * Desaloca recursivamente os nos da arvore. Tambem desaloca a subarvore de cada
 * no. Sempre retorna NULL.
 */
struct tNo *destroiGalhos(struct tNo *no) {
    if(no->esq != NULL)
        destroiGalhos(no->esq);
    if(no->dir != NULL)
        destroiGalhos(no->dir);

    destroiNumArvore(no->subTree);
    free(no);

    return NULL;
}

struct tArvore *destroiArvore(struct tArvore *tree) {
    destroiGalhos(tree->raiz);
    free(tree);

    return NULL;
}

/* Funcao interna. Busca e retorna o ponteiro para uma chave da arvore. */
struct tNo *busca(struct tNo *no, wchar_t chave) {
    if (no == NULL)
        return NULL;
    if (no->chave == chave)
        return no;
    if (chave < no->chave)
        return busca(no->esq, chave);
    return busca(no->dir, chave);
}

/* Busca qual no possui o valor pos guardado. */
struct tNo *buscaNumAux(struct tNo *no, int pos) {
    struct tNo *verificador;
    if (no == NULL)
        return NULL;

    if (buscaPos(no->subTree->raiz, pos))
        return no;
    verificador = buscaNumAux(no->esq, pos);
    if (verificador != NULL)
        return verificador;
    verificador = buscaNumAux(no->dir, pos);
    if (verificador != NULL)
        return verificador;

    return NULL;
}

wchar_t buscaNum(struct tArvore *tree, int pos) {
    struct tNo *noEncontrado;
    if(! (noEncontrado = buscaNumAux(tree->raiz, pos)))
        return L'\0';

    return noEncontrado->chave;
}

/*
 * Funcao interna para gerenciamento de dados.
 * Reorganiza os nos da arvore fazendo uma rotacao para a esquerda.
 * Diminui em 1 a altura da arvore no processo.
 */
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

/*
 * Funcao interna para gerenciamento de dados.
 * Reorganiza os nos da arvore fazendo uma rotacao para a direita.
 * Diminui em 1 a altura da arvore no processo.
 */
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

/* Cria e aloca memoria para um no. */
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

/* Calcula a altura da arvore. */
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

/*
 * Funcao interna para gerenciamento de dados.
 * Calcula e realiza as rotacoes necessarias para manter o balanceamento.
 * Tambem atualiza a variavel de equilibrio dos nodos afetados.
 */
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

/*
 * Recebe uma chave e uma posicao. Busca o no da chave e adiciona a posicao
 * na subarvore do no. Se o no da chave nao estiver presente, cria o no e a sua
 * subarvore.
 */
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

/* Funcao auxiliar da funcao de debugging */
void imprimeEmOrdem(struct tNo *no) {
    if (no == NULL)
        return;

    imprimeEmOrdem(no->esq);
    printf("%lc:", no->chave);
    imprimeInverso(no->subTree->raiz);
    printf("\n");
    imprimeEmOrdem(no->dir);
}

void imprimeDados(struct tArvore *tree) {
    imprimeEmOrdem(tree->raiz);
}

/* Funcao auxiliar da funcao de impressao. */
void imprimeOrdemEmArq(FILE *arq, struct tNo *no) {
    if (no == NULL)
        return;

    imprimeOrdemEmArq(arq, no->esq);
    fwprintf(arq, L"%s:", &no->chave);
    imprimeInversoEmArq(arq, no->subTree->raiz);
    fwprintf(arq, L"\n");
    imprimeOrdemEmArq(arq, no->dir);
}

void imprimeDadosEmArq(FILE *arq, struct tArvore *tree) {
    imprimeOrdemEmArq(arq, tree->raiz);
}

int buscaDadoAleatorio(wchar_t chave, struct tArvore *tree) {
    struct tNo *aux;

    aux = busca(tree->raiz, chave);

    return posAleatoria(aux->subTree);
}
