#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

Fila_t *cria_fila(unsigned int limite_fila) {
    Fila_t *aux;

    if(! (aux = malloc(sizeof(Fila_t))))
        return NULL;

    aux->head = NULL;
    aux->tail = NULL;
    aux->tam = 0;
    aux->maxTam = limite_fila;

    return aux;
}

int insere (Elemento_t *o, Fila_t *f) {
    if (ehVazia(f)) {
        f->head = o;
        f->tail = o;
        (f->tam)++;

        return 1;
    }

    if(! ehCheia(f)) {
        f->tail->prox = o;
        f->tail = o;
        (f->tam)++;
    } else {
        fprintf(stderr, "Não foi possível inserir. A fila está cheia.\n");
        return 0;
    }
    
    return 1;
}

int retira (Elemento_t **o, Fila_t *f) {
    Elemento_t *aux;

    if(ehVazia(f))
        return 0;

    **o = f->head->valor;
    aux = f->head;
    f->head = f->head->prox;
    (f->tam)--;

    free(aux);
    return 1;
}

int ehVazia(Fila_t *f) {
    return f->tam == 0;
}

int ehCheia (Fila_t *f) {
    return f->tam >= f->maxTam;
}

void imprime (Fila_t *f) {
    Elemento_t *aux;
    
    if(ehVazia(f)) {
        printf("Fila vazia.\n");
        return;
    }

    aux = f->head;
    aux->prox = f->head->prox;

    while (aux != NULL) {
        printf ("%d ", aux->valor);
        aux = aux->prox;
    }
    printf("\n");
}
